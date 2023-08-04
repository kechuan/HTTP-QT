#include "propertieswidget.h"
#include "ui_propertieswidget.h"
#include "DelegateProgressBar.h"

#include "connect.h"
#include "./dependences/sizeTextHandler.h"

#include <QtWidgets>
#include <QThread>
#include <QTimer>

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
    Pending,
    Downloading,
    Uploading,
    Paused,
    Finished,
    Failed,
};

int PropTaskCount;
double SpeedCount;

extern Connect *Client1;
extern std::string DownloadPath;
QList<QString> DownloadSpeedList;

QList<QTreeWidgetItem*> selectedTaskList;
extern QList<QTreeWidgetItem*> selectedFileList;

QTimer *DownloadWatcher;
bool DownloadingStatus = false;

PropertiesWidget::PropertiesWidget(QWidget *parent,Ui::MainWindow *m_ui) :
    QWidget(parent),
    ui(new Ui::PropertiesWidget),
    m_ui(m_ui)
{
    ui->setupUi(this);
    this->setAcceptDrops(true); //prop控件允许被拖拽事件响应

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;
    treeWidgetTaskQueue->setStyleSheet("QHeaderView::section{background:#B1EA14;}");

    treeWidgetTaskQueue->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //疑似自适应长度
    treeWidgetTaskQueue->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeWidgetTaskQueue->header()->setMinimumSectionSize(80);

    QObject::connect(treeWidgetTaskQueue,&QTreeWidget::itemPressed,this,&PropertiesWidget::TaskList_Menu); //item按下判断触发
    QObject::connect(treeWidgetTaskQueue,&QTreeWidget::itemDoubleClicked,this,&PropertiesWidget::OpenFile);

    QObject::connect(Client1,&Connect::ProgressUpdate,this,&PropertiesWidget::ProgressUpdate);

    QPushButton *Continued_Button = ui->pushButton_Continue;
    QPushButton *Paused_Button = ui->pushButton_Pause;
    QPushButton *Remove_Button = ui->pushButton_Remove;

    QObject::connect(Continued_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);
    QObject::connect(Paused_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);
    QObject::connect(Remove_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);

    QTimer *DownloadWatcher = new QTimer(this);

    QObject::connect(treeWidgetTaskQueue,&QTreeWidget::itemChanged,this,[=]{
        //不知道为什么 但总之itemChanged不会监听delete删除事件

        if(DownloadingStatus) return; //当处在下载状态时 多次触发该事件 跳过

        if(!PropTaskCount){
            qDebug("new Task Listen Start.");
            PropTaskCount = treeWidgetTaskQueue->topLevelItemCount();
            DownloadWatcher->start(500); //开启监听 0.5s刷新

            DownloadingStatus = true;
        }

        else{
            if(treeWidgetTaskQueue->topLevelItemCount()>PropTaskCount){
                qDebug("Append Task Listen Start.");
                PropTaskCount = treeWidgetTaskQueue->topLevelItemCount();
                DownloadWatcher->start(500); //开启监听 0.5s刷新

                DownloadingStatus = true;
            }

        }

    });


    QObject::connect(DownloadWatcher,&QTimer::timeout,this,[=]{
        m_ui->label_DownloadSpeedValue->setText("0"); //default value: 0

        QTreeWidgetItem *currentTask;

        if(DownloadingStatus){
            DownloadingStatus = false; //默认状态为false 但是这样操作 线程大概不安全

            for(int TaskIndex = 0;TaskIndex<PropTaskCount;TaskIndex++){
                currentTask = treeWidgetTaskQueue->topLevelItem(TaskIndex);

                if(currentTask->text(StatusList)=="Downloading" || "Pending"){
                    if(currentTask->text(SpeedList)!="—"){
                        std::string SpeedText = currentTask->text(SpeedList).toStdString();
                        qDebug("currentTask Name:%s,itemSpeed:%s",currentTask->text(FilenameList).toStdString().c_str(),currentTask->text(SpeedList).toStdString().c_str());
                        SpeedCount+=StringToSize(SpeedText);
                        DownloadingStatus = true; //只要有任一一项还在活跃的任务 监控继续
                    }
                }
            }

            qDebug("totalSpeed: %fMB/s",SpeedCount/1024/1024);

            m_ui->label_DownloadSpeedValue->setText(QString::fromStdString(SizeToString(SpeedCount)));

        }

        else{
            DownloadWatcher->stop();
            qDebug("下载监控终止");
        }

    });

    qDebug()<<"Status created";

}

PropertiesWidget::~PropertiesWidget(){
    delete ui;
}


//slots:
bool PropertiesWidget::TaskList_Menu(QTreeWidgetItem *listItem, int column){

    selectedTaskList = ui->treeWidgetTaskQueue->selectedItems();
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
            deletePrompt(selectedTaskList);
        });
    }

    return false;
}

//generalFunction:

void PropertiesWidget::deletePrompt(QList<QTreeWidgetItem*> selectedTaskList){
    qDebug()<<"deletePrompt";

    if(!selectedTaskList.length()) return;

    //*prompt 删除本地文件 确认
    QMessageBox DeleteConfirm;



    //窗体基础样式设置
    DeleteConfirm.setStyleSheet(
        R"(
            QLabel {
                min-width:120px;
                max-height:180px;
                font-size:12px;
            }
        )"
    );

    //tr -> translate
    DeleteConfirm.setWindowTitle(tr("删除文件"));
    DeleteConfirm.setText(tr("是否与文件一起删除"));

    std::string Text = "已选中0个文件";

    if(selectedTaskList.length()>1){
        //UTF-8 汉字编码为3字节 "已选中" 然后再减去索引的1得出需要修改的目标index:9
        Text.replace(9,1,std::to_string(selectedTaskList.length()));

        Text.append("\n");

        for(auto currentFileName:selectedTaskList){
            Text.append(currentFileName->text(FilenameList).toStdString());
            Text.append("\n");
        }

        DeleteConfirm.setText(QString::fromStdString(Text));
        DeleteConfirm.adjustSize();
    }

    QPushButton *DeleteButton = DeleteConfirm.addButton("Delete",QMessageBox::AcceptRole);
    QPushButton *DeniedButton = DeleteConfirm.addButton("ListItem Only",QMessageBox::NoRole);
    QPushButton *DiscardButton = DeleteConfirm.addButton("Discard",QMessageBox::RejectRole);

    DeleteConfirm.exec();

    if(DeleteConfirm.clickedButton() == DeleteButton){
        for(auto CurrentItem:selectedTaskList){
            QString FullPath = CurrentItem->text(StoragePathList)+CurrentItem->text(FilenameList);
            //correctWay delete u8
            qDebug("deletePath:%s",FullPath.toStdString().c_str());
            std::filesystem::remove(std::filesystem::u8path(FullPath.toStdString().c_str()));
            delete CurrentItem;
        }

    }

    else if(DeleteConfirm.clickedButton() == DeniedButton){
        for(auto CurrentItem:selectedTaskList){
            delete CurrentItem;
        }
    }

    else if(DeleteConfirm.clickedButton() == DiscardButton){
        return;
    }

}

void PropertiesWidget::clearStatusList(){
    qDebug()<<"clearStatusList";
    ui->treeWidgetTaskQueue->clear();
}

void PropertiesWidget::ProgressCreate(QTreeWidgetItem* Item){

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;
    Qt::MatchFlags flag = Qt::MatchExactly;
    QList matchList = treeWidgetTaskQueue->findItems(Item->text(FilenameList),flag,1);

    //已存在时 跳过创建 (可选)询问是否重新下载
    if (!matchList.empty()){
        return;
    }

                                    // Status,  Filename,      Progress,    Size,      Speed,DateTime,     storagePath
    QList<QString> newItemInformation{"Pending",Item->text(FilenameList),"Progress",Item->text(2),"—","DateTime",QString::fromStdString(DownloadPath)};


    ProgressBarDelegate* progressBar = new ProgressBarDelegate(treeWidgetTaskQueue);

    treeWidgetTaskQueue->addTopLevelItem(new QTreeWidgetItem(newItemInformation));
    treeWidgetTaskQueue->setItemDelegateForColumn(ProgressList, progressBar);

}

void PropertiesWidget::ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSpeed){

    //qDebug() << "from thread slot:" << QThread::currentThreadId();

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;
    Qt::MatchFlags flag = Qt::MatchExactly;

    QList matchList = treeWidgetTaskQueue->findItems(itemName,flag,1);

    QTreeWidgetItem *currentItem = matchList.at(0);

        QMetaObject::invokeMethod(this,[&](){
            currentItem->setData(ProgressList, Qt::UserRole, Progress); //数据更新
            currentItem->setText(SpeedList,itemSpeed);
            StatusChanged(Downloading,currentItem);

            if(static_cast<int>(round(Progress))==100){
                qDebug("%s Download Complete!",itemName.toStdString().c_str());
                StatusChanged(Finished,currentItem);
            }

        });

    qDebug("itemName:%s,Progress:%f",itemName.toStdString().c_str(),Progress);


}

void PropertiesWidget::StatusChanged(int Status,QTreeWidgetItem* listItem){

    switch(Status){
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

    selectedTaskList = ui->treeWidgetTaskQueue->selectedItems();
    qDebug("selectedLength:%zu",selectedTaskList.length());

    if(button->text() == "Continue"){
        for(auto& TreeItem:selectedTaskList){
            if(TreeItem->text(0) == "Finished") continue;
            StatusChanged(Downloading,TreeItem);
        }
        return;
    }

    else if(button->text() == "Pause"){
        for(auto& TreeItem:selectedTaskList){
            if(TreeItem->text(0) == "Finished") continue;
            StatusChanged(Paused,TreeItem);
        }
        return;
    }

    else{
        deletePrompt(selectedTaskList);
        return;
    }

}


void PropertiesWidget::keyPressEvent(QKeyEvent *keyPressEvent){

    selectedTaskList = ui->treeWidgetTaskQueue->selectedItems();

    qDebug("selectedLength:%zu",selectedTaskList.length());

        switch(keyPressEvent->key()){
            case Qt::Key_Return: {
                for(auto& TreeItem:selectedTaskList){
                    qDebug()<<"listItem Address:"<<TreeItem;
                    if(TreeItem->text(StatusList)!="Finished") continue; //未在完成状态时 不要打开它
                    emit ui->treeWidgetTaskQueue->itemDoubleClicked(TreeItem, 0);
                }
                break;
            }

            case Qt::Key_Delete: {
                deletePrompt(selectedTaskList);
                break;
            }
        }
}

void PropertiesWidget::dragMoveEvent(QDragMoveEvent *dragMoveEvent){
    dragMoveEvent->acceptProposedAction();
}

void PropertiesWidget::dragEnterEvent(QDragEnterEvent *dragEnterEvent){

    if(dragEnterEvent->mimeData()->hasFormat("application/x-qwidget")){
        dragEnterEvent->acceptProposedAction();
        qDebug("Accept dragEnterEvent x-qwidget Type Drag");
    }

    else{
        dragEnterEvent->ignore();
    }
}

void PropertiesWidget::dropEvent(QDropEvent *dropEvent){

    if(dropEvent->mimeData()->hasFormat("application/x-qwidget")){
        qDebug("Accept dropEvent x-qwidget Type Drag");
        qDebug("selectedTaskList.length:%zu",selectedFileList.length());
        if(!selectedFileList.length()) return;

        emit Download(selectedFileList.at(0),0);

    }

}

//void PropertiesWidget::resizeEvent(QResizeEvent *resizeEvent){
//    qDebug("frame_TaskQueue Height:%d",this->ui->frame_TaskQueue->geometry().size().height());
//    qDebug("frame_TaskQueue Width:%d",this->ui->frame_TaskQueue->geometry().size().width());
//}
