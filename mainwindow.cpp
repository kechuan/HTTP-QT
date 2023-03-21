#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "connect.h"

#include "./dependences/HTMLFliter.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <string>
#include <fstream>
#include <QKeyEvent>

#include <vector>

std::vector<std::string> LinkVector = {};
std::vector<std::string> PathVector = {};
std::vector<std::string> NameVector = {};
std::vector<std::string> SizeVector = {};

QString FullIP;
int Port;

QString rootPath = "/file";
QString SurfingPath;
QString ParentPath;

Connect Client1;

// void menuBind(bool pressed, const QTreeWidgetItem* listitem, int colmun){
//     qDebug("Open! I passthough the information:%s",listitem->text(2).toStdString().c_str());
// }



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); //this指向UI自己
    this->setWindowTitle("test Window"); //标题定义

    //声明其他ui类
    AboutWindow = new About();
    AboutWindow->hide();

    // IP_controlPanelWindow = new IP_controlPanel();
   IP_controlPanelWindow = new IP_controlPanel(nullptr,ui);
   //第一个值:其默认值本来就是 QWidget *parent = nullptr 即父级 也就是所谓的Mainwindow身上 重新指向nullptr意思就和上方的About()含义一致
   //第二个arg:获取ui信息 谁的? MainWindow的

    IP_controlPanelWindow->hide();

    QTabWidget *tabWidget_contentShow = ui->tabWidget_contentShow;
    tabWidget_contentShow->setMovable(true);

    QTreeWidget *Filelist = ui->Filelist;
    Filelist->setStyleSheet("QHeaderView::section{background:#A3C99FFF;}"); //???QHeaderView
    Filelist->setContextMenuPolicy(Qt::CustomContextMenu);  //默认值 Default->0
    Filelist->setSelectionMode(QAbstractItemView::ExtendedSelection);






    //这个所谓的menu本质上是一个enmu枚举

   //warning: connect-not-normalized 对于信号与槽 似乎并不需要特别的分配&引用符号 用上普通的方式反而能避免更多的内存开销 真是奇怪
   //规范提示传入的信号类型 最好全部都是简简单单的写个type上去 不需要多余的 类型修饰符 或者是*/&之类 否则会导致可能的内存泄露(以及难看的warning

    QObject::connect(ui->About,SIGNAL(triggered()),this,SLOT(Tab_pressed()));
    QObject::connect(ui->IP_controlPanel,SIGNAL(triggered()),this,SLOT(Tab_pressed()));
    QObject::connect(Filelist,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(itemAccess(QTreeWidgetItem*,int)));

   //需求将QTreeWidget界面下对每个子item添加右键菜单 课是QTreeWidget自带的方法并没有这种设置 怎么办?
   //思路:先检测对treewidget的点击 然后检测被点击的item是左键还是右键 右键时执行qMenu
    QObject::connect(Filelist,SIGNAL(itemPressed(QTreeWidgetItem*,int)),this,SLOT(TreeWidgetItem_Menu(QTreeWidgetItem*,int))); //item按下判断触发
    QObject::connect(ui->tabWidget_contentShow,SIGNAL(tabBarClicked(int)),this,SLOT(LostSelection(int)));

}

//事件声明区

//空白区域
void MainWindow::menu_blank()
{
    QMessageBox::warning(this,tr("提示"),tr("树的根节点！")); //table returning?
}

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

bool MainWindow::TreeWidgetItem_Menu(QTreeWidgetItem *listItem, int column){
    //那么其逻辑实际上等于 treewidgetitem作用域+全局右键判断
    if(qApp->mouseButtons() != Qt::RightButton) return false;

    qDebug()<<"right triggered";

    QString itemSize = listItem->text(2);
    QMenu *popmenu = new QMenu;

    //general way
    QAction *Refresh = new QAction("Refresh");
    QAction *Delete = new QAction("Delete");
    QAction *NewDir = new QAction("NewDir");
    QAction *Rename = new QAction("Rename");
    QAction *Upload = new QAction("Upload Files here...");


    popmenu->addAction(Refresh);
    popmenu->addAction(Delete);
    popmenu->addAction(NewDir);
    popmenu->addAction(Rename);
    popmenu->addAction(Upload);


    //signal Trigger add.
    // QObject::connect(Refresh,SIGNAL(triggered(bool)),this,SLOT(Refresh()));
    QObject::connect(
        Refresh,
        &QAction::triggered,
        this,
        [this](){   //捕获this 以引入函数执行主体 this->mainwindow
                ui->Filelist->clear();
                qDebug("Surfingpath%s",SurfingPath.toStdString().c_str());
                QList<QString> newItemInformation{"-","..","—",ParentPath};
                ui->Filelist->addTopLevelItem(new QTreeWidgetItem(newItemInformation));

                std::string Information = Client1.cliFileSurfing(FullIP,Port,SurfingPath);
                HTMLExtract(Information,LinkVector,PathVector,NameVector,SizeVector);

                for(int index = 0;index<=SizeVector.size()-1;++index){
                    QList<QString> newItemInformation{"-",NameVector.at(index).c_str(),SizeVector.at(index).c_str(),LinkVector.at(index).c_str()};
                    QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
                    ui->Filelist->addTopLevelItem(newItem);
                }
            }
    );

    QObject::connect(Delete,SIGNAL(triggered(bool)),this,SLOT(Delete()));
    QObject::connect(NewDir,SIGNAL(triggered(bool)),this,SLOT(NewDir()));
    QObject::connect(Rename,SIGNAL(triggered(bool)),this,SLOT(Rename()));
//    QObject::connect(Upload,SIGNAL(triggered(bool)),this,SLOT(Upload()));

    QObject::connect(
        Upload,
        &QAction::triggered,
        this,
        [](){
            qDebug("Upload File.");
            //说起来多集合的QString 得用官方提供的QStringList来装载
            //然而内部方法与vector却没什么不同

            QStringList UploadFiles =  QFileDialog::getOpenFileNames(
                nullptr,
                tr("choose Files to upload"),
                "D:/All Local Downloads",
                tr("texts(*.txt *.ini *.log *.md);;images(*.jpg *.jpeg *.png *.bmp);;video files(*.mp4 *.avi *.flv *.mkv);;All files(*.*)")
            );

            httplib::MultipartFormDataItems items; //->std::vector<httplib::MultipartFormData>
            httplib::MultipartFormData FormData;

            if(UploadFiles.length()!=0){

                qDebug("UploadFiles length:%zu",UploadFiles.length());
                for(auto &i:UploadFiles){
                    QFileInfo info(i);

                    QString Filename = std::move(info.fileName());
                    QString content_type = std::move(info.suffix());

                    qDebug("Filename:%s,content_type:%s",Filename.toStdString().c_str(),content_type.toStdString().c_str());

                    FormData.name = "Files";
                    FormData.filename = std::move(Filename.toStdString());
                    FormData.content_type = std::move(content_type.toStdString());

                    std::string filecontent;

                    std::ifstream TargetFile;
                    std::string path = i.toStdString();

                    qDebug("open in binary way");
                    TargetFile.open(path,std::ios_base::binary);


                    if(!TargetFile.is_open()){
                        qDebug("failed");
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

            Client1.cliFileUpload(FullIP,Port,TargetPosition,items);


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
        QList selectedList = Filelist->selectedItems();

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

        popmenu->insertAction(Refresh,Download);
        //signal Trigger add. but new QT5 string

        QObject::connect(
            Download,
            &QAction::triggered,
            this,
            [listItem,&column,this](){          //为什么要加this?
                itemAccess(listItem,column); //有可能是 不用this 就无法调用itemAccess?
                //还真是 解释:因为itemAccess 是一个 非 静态成员函数 所以需要一个对象来调用
                //而lambda本身 没有主体对象 所以需要引入一个主体对象来调用itemAccess 这里的this指代mainwindow

            });

    }

    else{
        //dir area
        qDebug("right clicked the Dir");

        //Action add.
        QAction *Open = new QAction("Open");

        popmenu->insertAction(Refresh,Open);

        //signal Trigger add.
        QObject::connect(Open,&QAction::triggered,this,[listItem,&column,this](){itemAccess(listItem,column);});

    }

     popmenu->move(ui->Filelist->cursor().pos());    //菜单显示在鼠标点击的位置
     popmenu->show();

     return true;

}

void MainWindow::itemAccess(QTreeWidgetItem *listItem,int column){

//   QString itemIcon = listItem->text(0);
   QString itemName = listItem->text(1);
   QString itemSize = listItem->text(2);
   QString itemLink = listItem->text(3);

   if(itemSize!="—"){
        QTreeWidget *Filelist = ui->Filelist;
        QList selectedList = Filelist->selectedItems();

        if(selectedList.size()>1){
            for(auto &i:selectedList){
                QString selectedListsName = i->text(1);
                QString selectedListsLink = i->text(3);
                qDebug("you selected the %s,which size is:%s",i->text(1).toStdString().c_str(),i->text(2).toStdString().c_str());
                Client1.cliFileDownload(FullIP,Port,selectedListsLink,selectedListsName);
            }
        }

        else{
            qDebug("you selected the %s,which size is:%s",itemName.toStdString().c_str(),itemSize.toStdString().c_str());
            Client1.cliFileDownload(FullIP,Port,itemLink,itemName);
        }
    }

   else{
       qDebug("double clicked the itemName %s, it linked is:%s",itemName.toStdString().c_str(),itemLink.toStdString().c_str()); //colnmun指代 子信息
       std::string Information = Client1.cliFileSurfing(FullIP,Port,itemLink);

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

        if(itemName==".."&&itemLink==rootPath){
           qDebug("redirect to the disk select.");

           Information = Client1.cliFileSurfing(FullIP,Port,rootPath);
           HTMLExtract(Information,LinkVector,NameVector);

           for(int index = 0;index<=NameVector.size()-1;++index){
               QList<QString> newItemInformation{"-",NameVector.at(index).c_str(),"—",LinkVector.at(index).c_str()};
               QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
               ui->Filelist->addTopLevelItem(newItem);
            }

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

void MainWindow::Rename(){
    qDebug("Rename item.");
}

void MainWindow::Delete(){
    qDebug("Delete item.");
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
//    qDebug("lost the selection because of clicked :%d",column);
    ui->Filelist->clearSelection();
}

//快捷键定义区

//keyPressEvent 重载
void MainWindow::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Return){
        QTreeWidgetItem *listItem = ui->Filelist->selectedItems().at(0);
            if (listItem != nullptr){
                emit ui->Filelist->itemDoubleClicked(listItem, 0); //回车->选择列表的双击
            }
        }
}

//void MainWindow::mousePressEvent(QMouseEvent *event){
//    QModelIndex index = ui->Filelist->indexAt(event->pos());

//    if(!index.isValid()){
//        qDebug("lost the selection");
//        ui->Filelist->clearSelection();
//    }
//}
