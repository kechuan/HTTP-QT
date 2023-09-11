#include "propertieswidget.h"
#include "ui_propertieswidget.h"
#include "DelegateProgressBar.h"
#include "speedLimitPanel.h"

#include "connect.h"
#include "toaster.h"

#include "./dependences/sizeTextHandler.h"
#include "./dependences/enumIndex.h"


#include <QtWidgets>
#include <QThread>
#include <QTimer>

int PropTaskCount;
double SpeedCount;

speedLimitPanel *speedLimitDialog;
extern Connect *Client1;
extern std::string storagePath;
extern QString parentPath;
extern bool connectedFlag;

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

    treeWidgetTaskQueue->setStyleSheet(R"(
        QHeaderView::section{background:#F0DECB;}
    )");

    //列表可多选
    treeWidgetTaskQueue->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //最小长度与自适应长度
    treeWidgetTaskQueue->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //固定长度设置区域
    treeWidgetTaskQueue->header()->setSectionResizeMode(ProgressList,QHeaderView::Fixed); //这条细则可以免受上述的自适应长度影响
    treeWidgetTaskQueue->setColumnWidth(ProgressList,120); //单独设置长度

    QObject::connect(treeWidgetTaskQueue,&QTreeWidget::itemPressed,this,&PropertiesWidget::TaskList_Menu); //item按下判断触发
    QObject::connect(treeWidgetTaskQueue,&QTreeWidget::itemDoubleClicked,this,&PropertiesWidget::OpenFile);

    QObject::connect(Client1,&Connect::ProgressUpdate,this,&PropertiesWidget::ProgressUpdate);


    //Toaster
    Toaster *DragToaster = new Toaster(this,ui);
    DragToaster->move(treeWidgetTaskQueue->width()*0.55,treeWidgetTaskQueue->height());


    QFrame *frame_TaskQueue = ui->frame_TaskQueue;

    frame_TaskQueue->setStyleSheet(R"(
        QPushButton{
            border:none;
            background:none
        }

        QPushButton:pressed{
            border: none;
            background: none;
        }
    )");


    QPushButton *Continued_Button = ui->pushButton_Continue;
    QPushButton *Paused_Button = ui->pushButton_Pause;
    QPushButton *Remove_Button = ui->pushButton_Remove;

    QIcon ContinueIcon = QIcon(":/svgPack/StatusPack/icon-Continue.svg");
    QIcon PauseIcon = QIcon(":/svgPack/StatusPack/icon-Pause.svg");
    QIcon RemoveIcon = QIcon(":/svgPack/StatusPack/icon-Remove.svg");

    Continued_Button->setIconSize(QSize(24,24));
    Paused_Button->setIconSize(QSize(24,24));
    Remove_Button->setIconSize(QSize(24,24));

    Continued_Button->setIcon(ContinueIcon);
    Paused_Button->setIcon(PauseIcon);
    Remove_Button->setIcon(RemoveIcon);

    Continued_Button->setCursor(Qt::PointingHandCursor);
    Paused_Button->setCursor(Qt::PointingHandCursor);
    Remove_Button->setCursor(Qt::PointingHandCursor);

    QObject::connect(Continued_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);
    QObject::connect(Paused_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);
    QObject::connect(Remove_Button,&QPushButton::clicked,this,&PropertiesWidget::ActionPressed);

    QTimer *DownloadWatcher = new QTimer(this);

    QObject::connect(treeWidgetTaskQueue,&QTreeWidget::itemChanged,this,[=]{
        //不知道为什么 但总之itemChanged不会监听delete删除事件

        if(DownloadingStatus) return; //当处在下载状态时 多次触发该事件 跳过

        if(!PropTaskCount){
            qDebug("new Task Listen Start.");
            qDebug()<<"DownloadSpeedWatcher:"<<QThread::currentThreadId();
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
                QVariant StatusFlag = currentTask->data(FilenameList,Qt::UserRole).toInt();

                bool ActiveStatus = StatusFlag!=Paused&&StatusFlag!=Finished;

                //Active Status
                if(ActiveStatus){
                    std::string SpeedText = currentTask->text(SpeedList).toStdString();
                    qDebug("currentTask Name:%s,itemSpeed:%s",currentTask->text(FilenameList).toStdString().c_str(),currentTask->text(SpeedList).toStdString().c_str());

                    SpeedCount+=StringToSize(SpeedText);
                    DownloadingStatus = true; //只要有任一一项还在活跃的任务 监控继续
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
bool PropertiesWidget::TaskList_Menu(QTreeWidgetItem *listItem){

    selectedTaskList = ui->treeWidgetTaskQueue->selectedItems();
    if(listItem==nullptr) return false;

    if(qApp->mouseButtons() == Qt::RightButton){
        QMenu *TaskList_popmenu = new QMenu;

        QAction *Pause = new QAction("Pause");
        QAction *Resume = new QAction("Resume");
        QAction *SpeedLimit = new QAction("Speed Limit[Test]");
        QAction *SpeedLimitCancel = new QAction("Limit Cancel[Test]");

        //Finished
        QAction *Open = new QAction("Open");
        QAction *Open_From_Folder = new QAction("Open From FileExplorer");

        //Failed
        QAction *Remove = new QAction("Remove");


        //generalArea
        TaskList_popmenu->addAction(Open);

        TaskList_popmenu->addAction(Pause);
        TaskList_popmenu->addAction(Resume);
        TaskList_popmenu->addAction(SpeedLimit);
        TaskList_popmenu->addAction(SpeedLimitCancel);

        TaskList_popmenu->addAction(Open_From_Folder);
        TaskList_popmenu->addAction(Remove);

        //ActiveStatus

        QVariant Status = listItem->data(FilenameList,Qt::UserRole);



        //inActiveStatus
        if(Status==Paused||Status==Finished){
            //Pause Status
            Pause->setEnabled(false);
            Resume->setEnabled(true);
            Open->setEnabled(false);

            //finished Status
            if(Status==Finished){
                Open->setEnabled(true);
                Pause->setEnabled(false);
                Resume->setEnabled(false);
            }

        }

        //ActiveStatus
        else{
            Open->setEnabled(false);
            Pause->setEnabled(true);
            Resume->setEnabled(false);
        }

        TaskList_popmenu->move(ui->treeWidgetTaskQueue->cursor().pos());    //菜单显示在鼠标点击的位置
        TaskList_popmenu->show();


        QObject::connect(Pause,&QAction::triggered,this,[listItem,this]{StatusChanged(Paused,listItem);});
        QObject::connect(Resume,&QAction::triggered,this,[listItem,this]{StatusChanged(Downloading,listItem);});
        QObject::connect(Open,&QAction::triggered,this,[listItem,this](){return OpenFile(listItem);});
        QObject::connect(Open_From_Folder,&QAction::triggered,this,[listItem,this](){return OpenFileFromFolder(listItem);});
        QObject::connect(Remove,&QAction::triggered,this,[this]{deletePrompt(selectedTaskList);});

        QObject::connect(SpeedLimit,&QAction::triggered,this,[this, listItem](){
            qDebug("Speed Limit");
            speedLimitDialog = new speedLimitPanel(nullptr);
            speedLimitDialog->exec();

            if(!speedLimitDialog->BufferLength&&!speedLimitDialog->delayms) return;

            emit TaskSpeedLimit(listItem,speedLimitDialog->BufferLength,speedLimitDialog->delayms);

        });

        QObject::connect(SpeedLimitCancel,&QAction::triggered,this,[this, listItem](){
            qDebug("Speed Limit Cancel");
            emit TaskLimitSpeedCancel(listItem);
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
    QPushButton *ListItemOnly = DeleteConfirm.addButton("ListItem Only",QMessageBox::ActionRole);
    QPushButton *DiscardButton = DeleteConfirm.addButton("Discard",QMessageBox::RejectRole);

    DeleteConfirm.exec();

    if(DeleteConfirm.clickedButton() == DeleteButton){
        for(auto &CurrentItem:selectedTaskList){

            //任务未完成时 先暂停 再去向cliFileDownload申请取消.
            if(CurrentItem->text(DateTimeList) == "—"){
                std::condition_variable cancel_cv;
                std::mutex cancel_mtx;
                std::unique_lock<std::mutex> cancelLock(cancel_mtx);

                bool isCancel = false;
                QObject::connect(Client1,&Connect::CancelNotice,this,[&]{
                    isCancel = true;
                    cancel_cv.notify_one();
                });

                emit TaskCancel(CurrentItem);

                //最多等待1s的任务暂停 这个有意思 像js的Date.now()
                auto timeoutLine = std::chrono::steady_clock::now() + std::chrono::seconds(1);

                if(cancel_cv.wait_until(cancelLock,timeoutLine) == std::cv_status::timeout){

                    if(!isCancel){
                        qDebug("Task Cancel Succ.");
                    }

                    else{
                        qDebug("Task Cancel Failed.");
                        return;
                    }


                }

            }



            QString FullPath = CurrentItem->text(StoragePathList)+CurrentItem->text(FilenameList);
            //correctWay delete u8
            qDebug("deletePath:%s",FullPath.toStdString().c_str());
            std::filesystem::remove(std::filesystem::u8path(FullPath.toStdString().c_str()));

            if(CurrentItem != nullptr) delete CurrentItem;
        }

    }

    else if(DeleteConfirm.clickedButton() == ListItemOnly){
        for(auto &CurrentItem:selectedTaskList){

            if(CurrentItem->text(DateTimeList) == "—"){
                std::condition_variable cancel_cv;
                std::mutex cancel_mtx;
                std::unique_lock<std::mutex> cancelLock(cancel_mtx);

                bool isCancel = false;
                QObject::connect(Client1,&Connect::CancelNotice,this,[&]{
                    isCancel = true;
                    cancel_cv.notify_one();
                });

                emit TaskCancel(CurrentItem);

                //最多等待1s的任务暂停 这个有意思 像js的Date.now()
                auto timeoutLine = std::chrono::steady_clock::now() + std::chrono::seconds(1);

                if(cancel_cv.wait_until(cancelLock,timeoutLine) == std::cv_status::timeout){

                    if(!isCancel){
                        qDebug("Task Cancel Succ.");
                    }

                    else{
                        qDebug("Task Cancel Failed.");
                        return;

                    }

                }

            }

            if(CurrentItem != nullptr) delete CurrentItem;
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
    QList matchList = treeWidgetTaskQueue->findItems(Item->text(1),Qt::MatchExactly,FilenameList);

    //已存在时 跳过创建
    if (!matchList.empty()){
        return;
    }

                  //Filename from SurfingFile       Progress,            Size,    Speed, DateTime,  storagePath
    QList<QString> newItemInformation{Item->text(1),"Progress",Item->text(SizeList),"—","—",QString::fromStdString(storagePath)};

    ProgressBarDelegate* progressBar = new ProgressBarDelegate(treeWidgetTaskQueue);

    treeWidgetTaskQueue->addTopLevelItem(new QTreeWidgetItem(newItemInformation));
    treeWidgetTaskQueue->setItemDelegateForColumn(ProgressList, progressBar);

    treeWidgetTaskQueue->topLevelItem(treeWidgetTaskQueue->topLevelItemCount()-1)->setTextAlignment(FilenameList,Qt::AlignVCenter);
    treeWidgetTaskQueue->topLevelItem(treeWidgetTaskQueue->topLevelItemCount()-1)->setData(FilenameList, Qt::DecorationRole, QIcon(":/svgPack/StatusPack/icon-Pending.svg"));
    treeWidgetTaskQueue->topLevelItem(treeWidgetTaskQueue->topLevelItemCount()-1)->setData(FilenameList, Qt::UserRole, Pending);

}

void PropertiesWidget::ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSpeed){

    qDebug() << "from thread slot:" << QThread::currentThreadId();

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;
    QList matchList = treeWidgetTaskQueue->findItems(itemName,Qt::MatchExactly,FilenameList);

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
            listItem->setData(FilenameList, Qt::DecorationRole, QIcon(":/svgPack/StatusPack/icon-Downloading.svg").pixmap(QSize(20,20))); //数据更新
            listItem->setData(FilenameList, Qt::UserRole, Downloading);
            emit TaskContinue(listItem);
            break;
        }

        case Uploading: {
            listItem->setText(FilenameList,"Uploading"); break;
        }

        case Paused: {
            emit TaskPaused(listItem);
            listItem->setText(SpeedList,"—");
            listItem->setData(FilenameList, Qt::DecorationRole, QIcon(":/svgPack/StatusPack/icon-PauseStatus.svg").pixmap(QSize(20,20))); //数据更新
            listItem->setData(FilenameList, Qt::UserRole, Paused);

            break;

            //如果我要在这里发送暂停请求
            //那么我首先要到达的就是处在mainwindow.cpp的eventLoop 以命令线程暂停。。
        }

        case Failed: {
            listItem->setText(FilenameList,"Failed"); break;
        }

        case Finished:{
            qDebug("listItem:%s Status change. Status Code: %d",listItem->text(FilenameList).toStdString().c_str(),Status);
            listItem->setData(FilenameList, Qt::DecorationRole, QIcon(":/svgPack/StatusPack/icon-Completed.svg").pixmap(QSize(20,20))); //数据更新
            listItem->setData(FilenameList, Qt::UserRole, Finished);
            listItem->setText(DateTimeList,QTime::currentTime().toString());

            break;
        }

    }

}

//Slot:

void PropertiesWidget::OpenFile(QTreeWidgetItem* listItem){
    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile(listItem->text(StoragePathList)+listItem->text(FilenameList))));
}

void PropertiesWidget::OpenFileFromFolder(QTreeWidgetItem* listItem){
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

    if(button->objectName() == "pushButton_Continue"){
        for(auto& listItem:selectedTaskList){
            if(listItem->data(FilenameList,Qt::UserRole) == Finished) continue;
            StatusChanged(Downloading,listItem);
        }
        return;
    }

    else if(button->objectName() == "pushButton_Pause"){
        for(auto& TreeItem:selectedTaskList){
            if(TreeItem->text(SpeedList) == "—") continue;
            StatusChanged(Paused,TreeItem);
        }
        return;
    }

    else if(button->objectName() == "pushButton_Remove"){
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
                    if(TreeItem->text(SpeedList)!="—") continue; //未在完成状态时 不要打开它
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
    if(!connectedFlag) return;

    if(dragEnterEvent->mimeData()->hasFormat("application/x-qwidget")){
        dragEnterEvent->acceptProposedAction();
        qDebug("Accept dragEnterEvent x-qwidget Type Drag");
    }

    else{
        dragEnterEvent->ignore();
    }
}

void PropertiesWidget::dropEvent(QDropEvent *dropEvent){
    emit DropFile();
    if(dropEvent->mimeData()->hasFormat("application/x-qwidget")){
        qDebug("Accept dropEvent x-qwidget Type Drag");
        qDebug("selectedTaskList.length:%zu",selectedFileList.length());
        if(!selectedFileList.length()) return;



        // 文件/文件夹 判断处理

        //mimedata本质无法解析里面的数据类型的 Content-Type 但是 文字处理可以 这里选择Qurl与Qinfo

        /*因为这里的下载处理本质是直接读取selectedFileList的
         * 因此才会直接将selectedFileList.at(0)传达出去。
         * 本质上selectedFileList是extern传过来的 要是再传回去就感觉。。不符合逻辑
         *
         */

        emit Download(selectedFileList.at(0));

    }

}

void PropertiesWidget::resizeEvent(QResizeEvent *resizeEvent){
    qDebug("frame_TaskQueue Height:%d",this->ui->frame_TaskQueue->height());
    qDebug("frame_TaskQueue Width:%d",this->ui->frame_TaskQueue->width());
}
