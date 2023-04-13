#include "propertieswidget.h"
//#include "QtConcurrent/qtconcurrentrun.h"
//#include "QtConcurrent/qtconcurrentrun.h"
#include "ui_propertieswidget.h"

#include "DelegateProgressBar.h"
#include "connect.h"

#include <QtWidgets>
#include <QThread>

enum StatusList{
    Downloading,
    Uploading,
    Paused,
    Finished,
    Failed,

};

extern Connect Client1;

PropertiesWidget::PropertiesWidget(QWidget *parent,Ui::MainWindow *m_ui) :
    QWidget(parent),
    ui(new Ui::PropertiesWidget),
    m_ui(m_ui)
{
    ui->setupUi(this);

    qDebug()<<"Status created";

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;


    QObject::connect(treeWidgetTaskQueue,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(TaskList_Menu(QTreeWidgetItem*,int))); //item按下判断触发

    QObject::connect(&Client1,&Connect::testSignal,this,&PropertiesWidget::statusEventLoop);



}

PropertiesWidget::~PropertiesWidget(){
    delete ui;
}


//slots:
bool PropertiesWidget::TaskList_Menu(QTreeWidgetItem *listItem, int column){

    if(qApp->mouseButtons() != Qt::RightButton) return false;

    QString Status = listItem->text(0);

//    qDebug()<<"right triggered";

    QMenu *TaskList_popmenu = new QMenu;

    QAction *Pause = new QAction("Pause");
    QAction *Resume = new QAction("Resume");

    //Finished
    QAction *Open = new QAction("Open");

    //Failed
    QAction *Remove = new QAction("Remove");

    TaskList_popmenu->addAction(Pause);
    TaskList_popmenu->addAction(Resume);
    TaskList_popmenu->addAction(Open);
    TaskList_popmenu->addAction(Remove);


    if(Status!="Finished"){
        Resume->setEnabled(false);
        Open->setEnabled(false);
    }


    else{
        Pause->setEnabled(false);
    }

    TaskList_popmenu->move(ui->treeWidgetTaskQueue->cursor().pos());    //菜单显示在鼠标点击的位置
    TaskList_popmenu->show();

    
    QObject::connect(Pause,&QAction::triggered,this,[listItem,this](){StatusChanged(Paused,listItem);});
    QObject::connect(Resume,&QAction::triggered,this,[listItem,this](){StatusChanged(Downloading,listItem);});

    QObject::connect(Open,&QAction::triggered,this,[this](){ShowFromExplorer();});

    return true;

}




//signals:

void PropertiesWidget::clearStatusList(){
    qDebug()<<"clearStatusList";
    ui->treeWidgetTaskQueue->clear();
}

void PropertiesWidget::AddTaskQueue(const char* Status,QTreeWidgetItem *listItem){

    qDebug()<<"AddTaskQueue trigged";

    QString QStatus = Status;

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;

    // Status,Filename,Progress,Size,Speed,DateTime



    QList<QString> newItemInformation{QStatus,listItem->text(1),"null",listItem->text(2),"null","DateTime"};

    treeWidgetTaskQueue->addTopLevelItem(new QTreeWidgetItem(newItemInformation));

    ProgressBarDelegate* progressBar = new ProgressBarDelegate(treeWidgetTaskQueue);

    treeWidgetTaskQueue->setItemDelegateForColumn(2, progressBar);

    qDebug("topLevelItemCount:%d",treeWidgetTaskQueue->topLevelItemCount());
}

void PropertiesWidget::statusEventLoop(const QString& itemName,const QString& itemSize,float& Progress){
    QEventLoop statusLoop;

    qRegisterMetaType<float&>("float&");
    QMetaObject::invokeMethod(this, "testSlot", Q_ARG(QString, itemName), Q_ARG(QString, itemSize), Q_ARG(float&, Progress));
    qApp->processEvents(QEventLoop::AllEvents,5);


    QTimer::singleShot(20, &statusLoop, SLOT(quit())); //20ms后执行退出
    statusLoop.exec();

}

void PropertiesWidget::testSlot(const QString& itemName,const QString& itemSize,float& Progress){

//    QEventLoop statusLoop;

    qDebug() << "from thread slot:" << QThread::currentThreadId();

    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;
    Qt::MatchFlags flag = Qt::MatchExactly;

    QList matchList = treeWidgetTaskQueue->findItems(itemName,flag,1);

    if(!matchList.size()){

        // Status,Filename,Progress,Size,Speed,DateTime
        QList<QString> newItemInformation{"Downloading",itemName,"null",itemSize,"null","DateTime"};
        ProgressBarDelegate* progressBar = new ProgressBarDelegate(treeWidgetTaskQueue);

        treeWidgetTaskQueue->addTopLevelItem(new QTreeWidgetItem(newItemInformation));
        treeWidgetTaskQueue->setItemDelegateForColumn(2, progressBar);

        qDebug("topLevelItemCount:%d",treeWidgetTaskQueue->topLevelItemCount());
    }

    else{
        QTreeWidgetItem *currentItem = matchList.at(0);


            currentItem->setData(2, Qt::UserRole, Progress); //数据更新

            qApp->processEvents(QEventLoop::AllEvents,8);

            if(static_cast<int>(round(Progress))==100){
                qDebug("%s Download Complete!",itemName.toStdString().c_str());
                StatusChanged(3,currentItem);
            }



        qDebug("itemName:%s,Progress:%f",itemName.toStdString().c_str(),Progress);
    }




}

void PropertiesWidget::StatusChanged(int Status,QTreeWidgetItem* listItem){

    qDebug("listItem:%s Status change. Status Code: %d",listItem->text(1).toStdString().c_str(),Status);

    switch(Status){

        //setText(columnIndex,"String")

        case Downloading: {
            listItem->setText(0,"Downloading"); break; 
        }

        case Uploading: {
            listItem->setText(0,"Paused"); break;
        }

        case Paused: {
            listItem->setText(0,"Paused"); break;
        }

        case Failed: {
            listItem->setText(0,"Failed"); break;
        }

        case Finished:{
            listItem->setText(0,"Finished"); break;
        }

    }

}

//Slot:

void PropertiesWidget::ShowFromExplorer(){
    qDebug("open it from Download Pos.");
}
