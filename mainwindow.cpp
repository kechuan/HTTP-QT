#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "connect.h"

#include "./dependences/HTMLFliter.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <string>
#include <QKeyEvent>

#include <vector>

#include <future>

#include <QtConcurrent/QtConcurrent>

#define readyStatus std::future_status::ready
#define timeoutStatus std::future_status::timeout

// std::future_status result_status;

std::vector<std::string> LinkVector = {};
std::vector<std::string> PathVector = {};
std::vector<std::string> NameVector = {};
std::vector<std::string> SizeVector = {};

QString FullIP;int Port;

QString rootPath = "/file";
QString SurfingPath;
QString ParentPath;

std::string DownloadPath = "./downloads/";

bool m_status = false;

Connect Client1;
QList<QTreeWidgetItem*> selectedList;


//void mapItem(QTreeWidgetItem *selectedItem){
//    QString selectedName = selectedItem->text(1);
//    QString selectedSize = selectedItem->text(2);
//    QString selectedLink = selectedItem->text(3);
//    Client1.cliFileDownload(selectedName,selectedSize,selectedLink);
//}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); //this指向UI自己
    this->setWindowTitle("test Window"); //标题定义

    //声明其他ui类
    AboutWindow = new About();
    // About *AboutWindow = new About();
    AboutWindow->hide();

    IP_controlPanelWindow = new IP_controlPanel(nullptr,ui);
    IP_controlPanelWindow->show();

    DockWidget = new PropertiesWidget(ui->statusShow,ui); //显示在statusShow里
    DockWidget->show();


   //第一个值:其默认值本来就是 QWidget *parent = nullptr 即父级 也就是所谓的Mainwindow身上 重新指向nullptr意思就和上方的About()含义一致
   //第二个arg:获取ui信息 谁的? MainWindow的

    QTabWidget *tabWidget_contentShow = ui->tabWidget_contentShow;
    tabWidget_contentShow->setMovable(true);

    QTreeWidget *Filelist = ui->Filelist;
    Filelist->setStyleSheet("QHeaderView::section{background:#A3C99FFF;}"); //???QHeaderView
//    Filelist->setContextMenuPolicy(Qt::CustomContextMenu);  //默认值 Default->0
    Filelist->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QLineEdit *DownloadPathInput = ui->DownloadPath;
    QPushButton *Button_DownloadSurfingPath = ui->DownloadSurfingPath;

    std::string FullPath = std::filesystem::current_path().string();
    FullPath.insert(0,"Default Path:");
    FullPath.append("\\downloads");

    DownloadPathInput->setPlaceholderText(QString::fromStdString(std::move(FullPath)));

    DownloadPathInput->setVisible(false);
    Button_DownloadSurfingPath->setVisible(false);


    QWidget *statusShow = ui->statusShow;
    statusShow->setVisible(false);
    statusShow->setDisabled(true);

    QPushButton *TaskQueue = ui->pushButton_TaskQueue;

    //这个所谓的menu本质上是一个enmu枚举

   //warning: connect-not-normalized 对于信号与槽 似乎并不需要特别的分配&引用符号 用上普通的方式反而能避免更多的内存开销 真是奇怪
   //规范提示传入的信号类型 最好全部都是简简单单的写个type上去 不需要多余的 类型修饰符 或者是*/&之类 否则会导致可能的内存泄露(以及难看的warning

    QObject::connect(ui->About,&QAction::triggered,this,&MainWindow::Tab_pressed);
    QObject::connect(ui->IP_controlPanel,&QAction::triggered,this,&MainWindow::Tab_pressed);

    QObject::connect(ui->DownloadPathSetting,&QAction::triggered,this,[this,DownloadPathInput,Button_DownloadSurfingPath](){
        if(DownloadPathInput->isVisible()==false){
            DownloadPathInput->setVisible(true);
            Button_DownloadSurfingPath->setVisible(true);
        }

        else{
            DownloadPathInput->setVisible(false);
            Button_DownloadSurfingPath->setVisible(false);
        }

    });

    //按下Enter或者是输入框失去焦点时触发


    QObject::connect(Button_DownloadSurfingPath,&QPushButton::clicked,this,[this,DownloadPathInput,Button_DownloadSurfingPath](){
        qDebug("you clicked the Button_DownloadSurfingPath");

        QString homePath = QDir::homePath();

        QUrl DownloadPathInputSelect = QFileDialog::getExistingDirectoryUrl(
            nullptr,
            tr("SurfingPath for DownloadPathInput"),
            homePath
        );

        if(!DownloadPathInputSelect.isEmpty()){
            //DownloadPathInputSelect 选择出来的路径是 file:/// 因此需要手动擦除一下
            std::string SplitPath = DownloadPathInputSelect.toString().toStdString().erase(0,8);
            DownloadPath = SplitPath;
            DownloadPath.append("/");
            DownloadPathInput->clear();
            DownloadPathInput->insert(QString::fromStdString(DownloadPath));
        }

        else{
            qDebug("selected no thing for DownloadPathInput");
        }

        QMessageBox ConfirmPrompt;

        ConfirmPrompt.setInformativeText(DownloadPath.c_str());
        ConfirmPrompt.setText("下载目录变更完毕");

        int exec = ConfirmPrompt.exec();

        DownloadPathInput->setVisible(false);
        Button_DownloadSurfingPath->setVisible(false);

    });

    QObject::connect(DownloadPathInput,&QLineEdit::editingFinished,this,[this,DownloadPathInput](){
        QMessageBox ConfirmPrompt;

        DownloadPath = DownloadPathInput->text().toStdString();

        ConfirmPrompt.setInformativeText(DownloadPath.c_str());
        ConfirmPrompt.setText("下载目录变更完毕");

        int exec = ConfirmPrompt.exec();

    });

    QObject::connect(Filelist,&QTreeWidget::itemDoubleClicked,this,&MainWindow::itemAccess); //item按下判断触发

   //需求将QTreeWidget界面下对每个子item添加右键菜单 课是QTreeWidget自带的方法并没有这种设置 怎么办?
   //思路:先检测对treewidget的点击 然后检测被点击的item是左键还是右键 右键时执行qMenu

    QObject::connect(Filelist,&QTreeWidget::itemPressed,this,&MainWindow::FileList_Menu); //item按下判断触发
    QObject::connect(TaskQueue,&QPushButton::clicked,this,[&](){showStatus(m_status);}); //item按下判断触发

    QObject::connect(ui->tabWidget_contentShow,&QTabWidget::tabBarClicked,this,&MainWindow::LostSelection);

    QObject::connect(this,&MainWindow::connetPressed,DockWidget,&PropertiesWidget::clearStatusList);
    QObject::connect(IP_controlPanelWindow,&IP_controlPanel::connetPressed,DockWidget,&PropertiesWidget::clearStatusList);

    //QObject是一个抽象类 似乎因为如此 connect需要用引用符号获得它的地址来进行操作
    //我真的特别谢谢QT还保留了地址的重载写法。。
    //confused

    //*UI不能即时更新
    // QObject::connect(&Client1,&Connect::testSignal,DockWidget,&PropertiesWidget::testSlot);

}

//事件声明区
MainWindow::~MainWindow()
{
    delete ui;
}


/*思路:
至少信号应该是 使用QTreeWidgetItem以主体的信号 指传递是通过QTreeWidgetItem itemPressed()
然后 通过信号触发的函数 以判断为是否右键 右键时再去执行其他行为

在这个过程中 有QTreeWidgetItem主体 也有itemPressed信号量的参与
然后是生成Qmenu中不可少的 Action添加
Action中也不可少的 头文件定义

最后 当Action被触发之后 应该有一个信号量去通知Action被触发了 以便执行Action对应的触发函数
当然 这个过程同样涉及到了 信号与槽

没想到可以直接通过槽的本身itempressed里 再额外判断mousebutton。。

不过这个qAPP是什么东西?

文档得知:qAPP 是来自于 类 QApplication 当中的一个 宏
其作用在于: qAPP == QApplication之下的 QGuiApplication这个类的单例对象指针

也就是说 它即代表当前的 QApplication(int &argc, char **argv) 构造的一个应用程序对象。

说人话 一般为主窗口背后的应用程序 无论主窗口有多少个子窗口 其所有背后都指向一个 且只有一个的
qapplication

而且 qapp无需 你特地传入qapplication实例 因为它自己就能直接指明路径

从而可以实现 在其他作用域下 直接调用 qapplication实例

不过值得一提的是 通过qAPP的传入 调用它的方法也变成了指针样式了

*/

bool MainWindow::FileList_Menu(QTreeWidgetItem *listItem, int column){
    //那么其逻辑实际上等于 treewidgetitem作用域+全局右键判断
    if(qApp->mouseButtons() != Qt::RightButton) return false;

    qDebug()<<"right triggered";

    QString itemSize = listItem->text(2);
    QMenu *FileList_popmenu = new QMenu;

    //general way
    QAction *Refresh = new QAction("Refresh");
    QAction *Delete = new QAction("Delete");
    QAction *NewDir = new QAction("NewDir");
    QAction *Rename = new QAction("Rename");
    QAction *Upload = new QAction("Upload Files here...");


    FileList_popmenu->addAction(Refresh);
    FileList_popmenu->addAction(Delete);
    FileList_popmenu->addAction(NewDir);
    FileList_popmenu->addAction(Rename);
    FileList_popmenu->addAction(Upload);


    //signal Trigger add.
    QObject::connect(
        Refresh,
        &QAction::triggered,
        this,
        [this](){   //捕获this 以引入函数执行主体 this->mainwindow
                ui->Filelist->clear();
                qDebug("Surfingpath: %s",SurfingPath.toStdString().c_str());
                QList<QString> newItemInformation{"-","..","—",ParentPath};
                ui->Filelist->addTopLevelItem(new QTreeWidgetItem(newItemInformation));

//                std::string Information = Client1.cliFileSurfing(FullIP,Port,SurfingPath);
                std::string Information = Client1.cliFileSurfing(SurfingPath);
                HTMLExtract(Information,LinkVector,PathVector,NameVector,SizeVector);

                for(int index = 0;index<=SizeVector.size()-1;++index){
                    QList<QString> newItemInformation{"-",NameVector.at(index).c_str(),SizeVector.at(index).c_str(),LinkVector.at(index).c_str()};
                    QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
                    ui->Filelist->addTopLevelItem(newItem);
                }
            }
    );

    QObject::connect(
        Delete,
        &QAction::triggered,
        this,
        [this,listItem](){
            MainWindow::Delete();
        }
    );

    QObject::connect(
        Rename,
        &QAction::triggered,
        this,
        [this,listItem](){
            MainWindow::Rename(listItem);
        }
    );

    QObject::connect(NewDir,SIGNAL(triggered(bool)),this,SLOT(NewDir()));


    QObject::connect(
        Upload,
        &QAction::triggered,
        this,
        [](){
            qDebug("Upload File.");
            //说起来多集合的QString 得用官方提供的QStringList来装载
            //然而内部方法与vector却没什么不同

            QStringList UploadFiles = QFileDialog::getOpenFileNames(
                nullptr,
                tr("choose Files to upload"),
                "D:/All Local Downloads",
                tr("texts(*.txt *.ini *.log *.md);;images(*.jpg *.jpeg *.png *.bmp);;video files(*.mp4 *.avi *.flv *.mkv);;All files(*.*)")
            );

            httplib::MultipartFormDataItems items; //->std::vector<httplib::MultipartFormData>
            httplib::MultipartFormData FormData;

            if(UploadFiles.length()!=0){

                qDebug("UploadFiles length:%zu",UploadFiles.length());
                for(auto &File:UploadFiles){
                    QFileInfo info(File);

                    QString Filename = std::move(info.fileName());
                    QString content_type = std::move(info.suffix());

                    qDebug("Filename:%s,content_type:%s",Filename.toStdString().c_str(),content_type.toStdString().c_str());

                    FormData.name = "Files";
                    FormData.filename = std::move(Filename.toStdString());
                    FormData.content_type = std::move(content_type.toStdString());

                    std::string filecontent;

                    std::ifstream TargetFile;
                    std::string readPath = File.toStdString();

                    TargetFile.open(std::filesystem::u8path(readPath),std::ios_base::binary);
                    qDebug("open in binary way");


                    if(!TargetFile.is_open()){
                        qDebug("open failed");
                    }

                    else{
                        //STL->istreambuf_iterators
                        std::string filecontent((std::istreambuf_iterator<char>(TargetFile)), (std::istreambuf_iterator<char>()));
                        FormData.content = std::move(filecontent);
                        TargetFile.close();
                        items.emplace_back(FormData);

                    }

                }
            }


//                    qDebug()<<"please select the TargetPosition want to upload";

            QString TargetPosition = R"(D:\cpp\app\HTTP-UI\build-HTTP-QT-Desktop_Qt_6_2_4_MinGW_64_bit-Debug\downloads)";

//            Client1.cliFileUpload(FullIP,Port,TargetPosition,items);
            Client1.cliFileUpload(TargetPosition,items);


        }
    );

     if(itemSize!="—"){
        //file area
        qDebug("right clicked the file");
        QAction *Download = new QAction("Download");

        //奇怪的思路 居然是先指定action2然后再让action1插入到action2的前面
        //而不是直接声明action1 插入到 action2之前

        //在弹出之前 先过一个纯File判断

        QTreeWidget *Filelist = ui->Filelist;
        selectedList = Filelist->selectedItems();

        if(selectedList.size()>1){
            for(auto &i:selectedList){
                QString selectedListsName = i->text(1);
                QString selectedListsSize = i->text(2);

                if(selectedListsSize!="—"){
                    qDebug("you selected the %s,which size is:%s",i->text(1).toStdString().c_str(),i->text(2).toStdString().c_str());
                }

                else{
                    qDebug("you select %s which is not a File!",selectedListsName.toStdString().c_str());
                    //此处也应弹出Download 但是是不可点击状态(灰色) 所以还是别直接return false完事
                    Download->setEnabled(false);
                }


            }

        }

        FileList_popmenu->insertAction(Refresh,Download);
        //signal Trigger add. but new QT5 string

        QObject::connect(
            Download,
            &QAction::triggered,
            this,
            [listItem,&column,this](){          //为什么要加this?
                itemAccess(listItem,column); //有可能是 不用this 就无法调用itemAccess?
                //还真是 解释:因为itemAccess 是一个 非 静态成员函数 所以需要一个对象来调用
                //而在两次嵌套的lambda本身 已经没有了主体对象 所以需要引入一个主体对象来调用itemAccess 这里的this指代mainwindow

        });

        QObject::connect(Download,&QAction::triggered,this,[listItem, this](){emit DownloadPressed("Downloading",listItem);});

    }

    else{
        //dir area
        qDebug("right clicked the Dir");

        //Action add.
        QAction *Open = new QAction("Open");

        FileList_popmenu->insertAction(Refresh,Open);

        //signal Trigger add.
        //二度lambda 需要 通过this以及手动capture来捕获
        QObject::connect(Open,&QAction::triggered,this,[listItem,&column,this](){itemAccess(listItem,column);});


    }

     FileList_popmenu->move(ui->Filelist->cursor().pos());    //菜单显示在鼠标点击的位置
     FileList_popmenu->show();

     return true;

}



void MainWindow::itemAccess(QTreeWidgetItem *listItem,int column){

//这里的column代表这一行里点击的位置判定(Icon->0/FileName->1/Size->2...) 不过我并没有对column作出什么更改需求

//  QString selectedListsIcon = listItem->text(0);
    QString selectedListsName = listItem->text(1);
    QString selectedListsSize = listItem->text(2);
    QString selectedListsLink = listItem->text(3);

    if(selectedListsSize!="—"){
        QTreeWidget *Filelist = ui->Filelist;
        selectedList = Filelist->selectedItems();

        QEventLoop DownloadLoop;
        QFutureWatcher<void> DownloadWatcher;


        if(selectedList.size()>1){
//            for(auto &item:selectedList){
//                QString selectedName = item->text(1);
//                QString selectedSize = item->text(2);
//                QString selectedLink = item->text(3);

//                qDebug("you selected the %s,which size is:%s",selectedName.toStdString().c_str(),selectedSize.toStdString().c_str());




//                DownloadWatcher.setFuture(QtConcurrent::run(&Connect::cliFileDownload,&Client1,std::ref(selectedLink),std::ref(selectedName),std::ref(selectedSize)));
//                QObject::connect(&DownloadWatcher,&QFutureWatcher<void>::finished,&DownloadLoop,&QEventLoop::quit);
//                DownloadLoop.exec();

//            }



//            QtConcurrent::mapped(
//                selectedList,
//                 [&,this](QTreeWidgetItem *selectedItem){
//                    for(auto &item:selectedList){
//                         QString selectedName = selectedItem->text(1);
//                         QString selectedSize = selectedItem->text(2);
//                         QString selectedLink = selectedItem->text(3);
//                         Client1.cliFileDownload(selectedName,selectedSize,selectedLink);
//                    }
//                }
//            );

//            QList<ItemInfo> result = QtConcurrent::mapped(
//                selectedList,
//                 [](QTreeWidgetItem *selectedItem) -> ItemInfo {
//                    QString selectedName = selectedItem->text(1);
//                    QString selectedSize = selectedItem->text(2);
//                    QString selectedLink = selectedItem->text(3);
//                    ItemInfo info;
//                    info.name = selectedName;
//                    info.size = selectedSize;
//                    info.link = selectedLink;
//                    return info;
//                            }).results();

//int func(int number){
//    return number*2;
//}


//            QList<int> testList;
//            testList.append(1);
//            testList.append(2);
//            testList.append(3);

//            QtConcurrent::mapped(testList,func);

//            for(auto& num:testList){
//                qDebug("num:%d",num);
//            }


            DownloadWatcher.setFuture(QtConcurrent::map(selectedList,[](QTreeWidgetItem *selectedItem){
                QString selectedName = selectedItem->text(1);
                QString selectedSize = selectedItem->text(2);
                QString selectedLink = selectedItem->text(3);
                Client1.cliFileDownload(selectedName,selectedSize,selectedLink);
//                qDebug("selectedLink:%s,selectedName:%s,selectedSize:%s",selectedLink.toStdString().c_str(),selectedSize.toStdString().c_str(),selectedName.toStdString().c_str());
                qDebug() << "Mainwindow.cpp Line 531: thread" << QThread::currentThreadId();
            }));

            QObject::connect(&DownloadWatcher,&QFutureWatcher<void>::finished,&DownloadLoop,&QEventLoop::quit);
            DownloadLoop.exec();


        }

        else{


            qDebug("you selected the %s,which size is:%s",selectedListsName.toStdString().c_str(),selectedListsSize.toStdString().c_str());
            DownloadWatcher.setFuture(QtConcurrent::run(&Connect::cliFileDownload,&Client1,std::ref(selectedListsName),std::ref(selectedListsSize),std::ref(selectedListsLink)));
            QObject::connect(&DownloadWatcher,&QFutureWatcher<void>::finished,&DownloadLoop,&QEventLoop::quit);
            DownloadLoop.exec();

        }

        
    }

   else{
       qDebug("double clicked the itemName %s, it linked to:%s",selectedListsName.toStdString().c_str(),selectedListsLink.toStdString().c_str()); //colnmun指代 子信息
       std::string Information = Client1.cliFileSurfing(selectedListsLink);

       HTMLExtract(Information,LinkVector,PathVector,NameVector,SizeVector);

       if(PathVector.size()>1){
            qDebug("上一级Path:%s",PathVector.at(PathVector.size()-2).c_str());
            ParentPath = PathVector.at(PathVector.size()-2).c_str();
            SurfingPath = PathVector.at(PathVector.size()-1).c_str();
        }

        else{
            qDebug("上一级Path:/files");
            ParentPath = rootPath;
        }

        // qDebug("%s",Information.c_str());



        //Filelist update part.
        ui->Filelist->clear();

        QList<QString> newItemInformation{"-","..","—",ParentPath};
        ui->Filelist->addTopLevelItem(new QTreeWidgetItem(newItemInformation));

        if(selectedListsName==".."&&selectedListsLink==rootPath){
           qDebug("redirect to the disk select.");

           Information = Client1.cliFileSurfing(rootPath);
           HTMLExtract(Information,LinkVector,NameVector);



           for(int index = 0;index<=NameVector.size()-1;++index){
               QList<QString> newItemInformation{"-",NameVector.at(index).c_str(),"—",LinkVector.at(index).c_str()};
               QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
               ui->Filelist->addTopLevelItem(newItem);
            }

        }

        else{

            if(!NameVector.size()){
                qDebug("empty File Open");
            }

            else{
                for(int index = 0;index<=NameVector.size()-1;++index){
                    QList<QString> newItemInformation{"-",NameVector.at(index).c_str(),SizeVector.at(index).c_str(),LinkVector.at(index).c_str()};
                    QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
                    ui->Filelist->addTopLevelItem(newItem);
                }
            }


       }
   }


}


//以下是事件定义区

void MainWindow::Tab_pressed(){
   qDebug() << "action trigger";

   QAction *Action = (QAction*)sender();
   QString ActionName = Action->text();

   if(ActionName == "IP控制台"){
      IP_controlPanelWindow->show();
   }

   else{
       AboutWindow->show();
   }

}

void MainWindow::Rename(QTreeWidgetItem *listItem){
    qDebug("Rename item:%s",listItem->text(1).toStdString().c_str());
}

void MainWindow::Delete(){
    QList<QString> LinkList;

    selectedList = ui->Filelist->selectedItems();

    for(auto& List:selectedList){
        qDebug("Ask for server to Delete item:%s",List->text(1).toStdString().c_str());
        LinkList.emplaceBack(List->text(3));
        delete List;
    }

    Client1.cliFileDelete(LinkList);

}


void MainWindow::NewDir(){
    qDebug("create newDir.");
}

void MainWindow::Copy(){
    qDebug("copy the file.");
}

void MainWindow::Cut(){
    qDebug("select the file.");
}

void MainWindow::LostSelection(int column){
    ui->Filelist->clearSelection();
}

void MainWindow::showStatus(bool &m_status){
    if(m_status == false){
        ui->statusShow->setVisible(true);
        ui->statusShow->setDisabled(false);
        m_status = true;
    }

    else{
        ui->statusShow->setVisible(false);
        ui->statusShow->setDisabled(true);
        m_status = false;
    }
    
}

void MainWindow::clearStatusList(){
    qDebug("clear your List");
}

//快捷键定义区

//keyPressEvent 事件 不同于 信号与槽的高度封装 事件通常需要手动去配置 但同时自由度也比事件与槽高的多
void MainWindow::keyPressEvent(QKeyEvent *event){
    QList<QTreeWidgetItem*> selectedTreeWidgetItems = ui->Filelist->selectedItems();

    switch(event->key()){
        case Qt::Key_Return: {
//          for(auto& TreeItem:selectedTreeWidgetItems){
                emit ui->Filelist->itemDoubleClicked(selectedTreeWidgetItems.at(0), 0); //回车->选择列表的双击
//          }
            break;
        }

        case Qt::Key_Delete:{
                Delete();
            break;
        }

        case Qt::Key_Backspace:{
            if(!LinkVector.size()){
                QTreeWidgetItem *UpperFolder = ui->Filelist->topLevelItem(0);
                emit ui->Filelist->itemDoubleClicked(UpperFolder, 0); //退格->表示双击上层
                break;
            }
            
        }


    }

}

