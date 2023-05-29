#include "propertieswidget.h"
#include "ui_propertieswidget.h"
#include "DelegateProgressBar.h"

#include "connect.h"

#include <QtWidgets>
#include <QThread>

enum QueueList{
    StatusList,
    FilenameList,
    ProgressList,
    SizeList,
    SpeedList,
    DateTimeList,
    StoragePathList,
};

enum StatusList{
    Downloading,
    Uploading,
    Paused,
    Finished,
    Failed,
    Pending,
};

extern Connect Client1;
extern std::string DownloadPath;

QList<QTreeWidgetItem*> selectedTreeWidgetItems;

PropertiesWidget::PropertiesWidget(QWidget *parent,Ui::MainWindow *m_ui) :
    QWidget(parent),
    ui(new Ui::PropertiesWidget),
    m_ui(m_ui)
{
    ui->setupUi(this);

    qDebug()<<"Status created";

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;
    treeWidgetTaskQueue->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QObject::connect(treeWidgetTaskQueue,&QTreeWidget::itemPressed,this,&PropertiesWidget::TaskList_Menu); //item按下判断触发
    QObject::connect(treeWidgetTaskQueue,&QTreeWidget::itemDoubleClicked,this,&PropertiesWidget::OpenFile);

    QObject::connect(&Client1,&Connect::ProgressUpdate,this,&PropertiesWidget::ProgressUpdate);

    QPushButton *Continued_Button = ui->pushButton_Continue;
    QPushButton *Paused_Button = ui->pushButton_Pause;
    QPushButton *Remove_Button = ui->pushButton_Remove;

    QObject::connect(Continued_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);
    QObject::connect(Paused_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);
    QObject::connect(Remove_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);

}

PropertiesWidget::~PropertiesWidget(){
    delete ui;
}


//slots:
bool PropertiesWidget::TaskList_Menu(QTreeWidgetItem *listItem, int column){

    selectedTreeWidgetItems = ui->treeWidgetTaskQueue->selectedItems();
    if(listItem==nullptr) return false;

    if(qApp->mouseButtons() == Qt::RightButton){
        QString Status = listItem->text(StatusList);

        QMenu *TaskList_popmenu = new QMenu;

        QAction *Pause = new QAction("Pause");
        QAction *Resume = new QAction("Resume");

        //Finished
        QAction *Open = new QAction("Open");
        QAction *Open_From_Folder = new QAction("Open From FileExplorer");

        //Failed
        QAction *Remove = new QAction("Remove");


        //generalArea
        TaskList_popmenu->addAction(Open);

        TaskList_popmenu->addAction(Pause);
        TaskList_popmenu->addAction(Resume);

        TaskList_popmenu->addAction(Open_From_Folder);

        TaskList_popmenu->addAction(Remove);

        //working Status
        if(Status!="Finished"){
            Open->setEnabled(false);
        }

        else if(Status=="Downloading"){
            Resume->setEnabled(false);
        }


        else if(Status=="Paused"){
            Pause->setEnabled(false);
        }

        //finished Status
        else{
            Open->setEnabled(true);
            Resume->setEnabled(false);
            Pause->setEnabled(false);
        }

        TaskList_popmenu->move(ui->treeWidgetTaskQueue->cursor().pos());    //菜单显示在鼠标点击的位置
        TaskList_popmenu->show();


        QObject::connect(Pause,&QAction::triggered,this,[listItem,this](){StatusChanged(Paused,listItem);});
        QObject::connect(Resume,&QAction::triggered,this,[listItem,this](){StatusChanged(Downloading,listItem);});

        //Action Start
        QObject::connect(
            Open,
            &QAction::triggered,
            this,
            [listItem,this](){
                return OpenFile(listItem,0);
            }
        );

        QObject::connect(Open_From_Folder,&QAction::triggered,this,[listItem,this](){return OpenFileFromFolder(listItem,0);});

        QObject::connect(Remove,&QAction::triggered,this,[this](){
            deletePrompt(selectedTreeWidgetItems);
        });
    }

    return false;
}

//generalFunction:

void PropertiesWidget::deletePrompt(QList<QTreeWidgetItem*> selectedTreeWidgetItems){
    qDebug()<<"deletePrompt";

    if(!selectedTreeWidgetItems.length()) return;

    //*prompt 删除本地文件 确认
    QMessageBox DeleteConfirm;

    //窗体基础样式设置
    DeleteConfirm.setStyleSheet(
        R"(
            QLabel {
                min-width:100px;
                min-height:40px;
                font-size:1ProgressBarpx;
            }
        )"
    );

    //tr -> translate
    DeleteConfirm.setWindowTitle(tr("删除文件"));
    DeleteConfirm.setText(tr("是否与文件一起删除"));

    std::string Text = "已选中0个文件";

    if(selectedTreeWidgetItems.length()>1){
        //UTF-8 汉字编码为3字节 "已选中" 然后再减去索引的1得出需要修改的目标index:9
        Text.replace(9,1,std::to_string(selectedTreeWidgetItems.length()));
        DeleteConfirm.setText(QString::fromStdString(Text));
    }

    QPushButton *DeleteButton = DeleteConfirm.addButton("Delete",QMessageBox::AcceptRole);
    QPushButton *DeniedButton = DeleteConfirm.addButton("ListItem Only",QMessageBox::NoRole);
    QPushButton *DiscardButton = DeleteConfirm.addButton("Discard",QMessageBox::RejectRole);

    DeleteConfirm.exec();

    if(DeleteConfirm.clickedButton() == DeleteButton){
        for(auto CurrentItem:selectedTreeWidgetItems){
            QString FullPath = CurrentItem->text(StoragePathList)+CurrentItem->text(FilenameList);
            //correctWay delete u8
            qDebug("deletePath:%s",FullPath.toStdString().c_str());
            std::filesystem::remove(std::filesystem::u8path(FullPath.toStdString().c_str()));
            delete CurrentItem;
        }

    }

    else if(DeleteConfirm.clickedButton() == DeniedButton){
        for(auto CurrentItem:selectedTreeWidgetItems){
            delete CurrentItem;
        }
    }

    else if(DeleteConfirm.clickedButton() == DiscardButton){
        return;
    }

}

//signals:

void PropertiesWidget::clearStatusList(){
    qDebug()<<"clearStatusList";
    ui->treeWidgetTaskQueue->clear();
}

void PropertiesWidget::ProgressCreate(QTreeWidgetItem* Item){

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;

    // Status,Filename,Progress,Size,Speed,DateTime,storagePath
    QList<QString> newItemInformation{"Pending",Item->text(1),"null",Item->text(2),"null","DateTime",QString::fromStdString(DownloadPath)};

    ProgressBarDelegate* progressBar = new ProgressBarDelegate(treeWidgetTaskQueue);

    treeWidgetTaskQueue->addTopLevelItem(new QTreeWidgetItem(newItemInformation));
    treeWidgetTaskQueue->setItemDelegateForColumn(ProgressList, progressBar);

}

void PropertiesWidget::ProgressUpdate(const QString& itemName,const QString& itemSize,QString& itemLink,float& Progress){

    qDebug() << "from thread slot:" << QThread::currentThreadId();

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;
    Qt::MatchFlags flag = Qt::MatchExactly;

    QList matchList = treeWidgetTaskQueue->findItems(itemName,flag,1);

    if(!matchList.size()){

        // Status,Filename,Progress,Size,Speed,DateTime,storagePath
        QList<QString> newItemInformation{"Downloading",itemName,"null",itemSize,"null","DateTime",QString::fromStdString(DownloadPath)};
        ProgressBarDelegate* progressBar = new ProgressBarDelegate(treeWidgetTaskQueue);

        treeWidgetTaskQueue->addTopLevelItem(new QTreeWidgetItem(newItemInformation));
        treeWidgetTaskQueue->setItemDelegateForColumn(ProgressList, progressBar);

        qDebug("topLevelItemCount:%d",treeWidgetTaskQueue->topLevelItemCount());
    }

    else{
        QTreeWidgetItem *currentItem = matchList.at(0);

        QMetaObject::invokeMethod(this,[&](){
            currentItem->setData(ProgressList, Qt::UserRole, Progress); //数据更新
            StatusChanged(Downloading,currentItem);

            if(static_cast<int>(round(Progress))==100){
                qDebug("%s Download Complete!",itemName.toStdString().c_str());
                StatusChanged(Finished,currentItem);
                currentItem->setData(SpeedList, Qt::UserRole, ""); //数据更新
            }

        });

        qDebug("itemName:%s,Progress:%f",itemName.toStdString().c_str(),Progress);
    }

}

void PropertiesWidget::StatusChanged(int Status,QTreeWidgetItem* listItem){



    switch(Status){

        //setText(columnIndex,"String")

        case Downloading: {
            listItem->setText(StatusList,"Downloading"); break; 
        }

        case Uploading: {
            listItem->setText(StatusList,"Uploading"); break;
        }

        case Paused: {
            qDebug("listItem:%s Status change. Status Code: %d",listItem->text(FilenameList).toStdString().c_str(),Status);
            listItem->setText(StatusList,"Paused"); break;

            //如果我要在这里发送暂停请求
            //那么我首先要到达的就是处在mainwindow.cpp的eventLoop 以命令线程暂停。。
        }

        case Failed: {
            listItem->setText(StatusList,"Failed"); break;
        }

        case Finished:{
            qDebug("listItem:%s Status change. Status Code: %d",listItem->text(FilenameList).toStdString().c_str(),Status);
            listItem->setText(StatusList,"Finished");
            listItem->setText(DateTimeList,QTime::currentTime().toString());

            break;
        }

    }

}

//Slot:

void PropertiesWidget::OpenFile(QTreeWidgetItem* listItem,int colmun){
    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(listItem->text(StoragePathList)+listItem->text(FilenameList))));
}


void PropertiesWidget::OpenFileFromFolder(QTreeWidgetItem* listItem,int colmun){
    qDebug("storagePath:%s",listItem->text(StoragePathList).toStdString().c_str());
    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(listItem->text(StoragePathList))));
}

void PropertiesWidget::ActionPressed(){

    //QPushButton *button = (QPushButton *)sender(); //指针函数sender?

    //等同于以下声明
    QPushButton *button = static_cast<QPushButton*>(sender());

    //似乎会自动检测信号触发的 sender 而前面的 (QPushButton*)则是类
    //也就是说 理论上。。 可以 auto *[Name] = (anyType QObject*)sender

    selectedTreeWidgetItems = ui->treeWidgetTaskQueue->selectedItems();
    qDebug("selectedLength:%zu",selectedTreeWidgetItems.length());

    if(button->text() == "Continue"){
        for(auto& TreeItem:selectedTreeWidgetItems){
            StatusChanged(Downloading,TreeItem);
        }
        return;
    }

    else if(button->text() == "Pause"){
        for(auto& TreeItem:selectedTreeWidgetItems){
            StatusChanged(Paused,TreeItem);
        }
        return;
    }

    else{
        deletePrompt(selectedTreeWidgetItems);
        return;
    }

}


void PropertiesWidget::keyPressEvent(QKeyEvent *event){

    selectedTreeWidgetItems = ui->treeWidgetTaskQueue->selectedItems();

    qDebug("selectedLength:%zu",selectedTreeWidgetItems.length());

        switch(event->key()){
            case Qt::Key_Return: {
                for(auto& TreeItem:selectedTreeWidgetItems){
                    qDebug()<<"listItem Address:"<<TreeItem;
                    if(TreeItem->text(StatusList)!="Finished") continue; //未在完成状态时 不要打开它
                    emit ui->treeWidgetTaskQueue->itemDoubleClicked(TreeItem, 0);
                }
                break;
            }

            case Qt::Key_Delete: {
                deletePrompt(selectedTreeWidgetItems);
                break;
            }
        }


}
