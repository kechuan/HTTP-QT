#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "./dependences/HTMLFliter.h"
#include "FileList.h"

#include "connect.h"
#include "ui_propertieswidget.h"

#include <string>
#include <vector>

#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QKeyEvent>
#include <QtConcurrent/QtConcurrent>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDropEvent>
#include <QDragLeaveEvent>

enum FileList_itemNumber{
    iconList,
    nameList,
    sizeList,
    linkList
};

std::vector<std::string> LinkVector = {};
std::vector<std::string> PathVector = {};
std::vector<std::string> NameVector = {};
std::vector<std::string> SizeVector = {};

std::vector<std::string> UploadVector = {};

QString FullIP;int Port;

QString rootPath = "/file";
QString SurfingPath;
QString ParentPath;
std::string DownloadPath = "./downloads/";

bool m_status = false;

short SplitterRecord;

//全局对象 创立
Connect *Client1;
FileList *SurfingFile;

QList<QTreeWidgetItem*> selectedFileList;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); //this指向UI自己
    this->setWindowTitle("HTTP-QT"); //标题定义

    //声明其他ui类
    AboutWindow = new About();
    // About *AboutWindow = new About();
    AboutWindow->hide();

    IP_controlPanelWindow = new IP_controlPanel(nullptr,ui);
    Client1 = new Connect(ui);


    //延迟弹出 以便弹窗后置在主程序
    QTimer::singleShot(800,this,[&](){qDebug()<<"IP_show ID:"<<QThread::currentThreadId();IP_controlPanelWindow->show();});

    SurfingFile = new FileList(ui->widget_FileList); //构造阶段时 已经为SurfingFile 分配好内存地址

    QSplitter *FileShare_Splitter = ui->PropTools;

    //inital FileList/TaskQueue

    qDebug("FileShare_Splitter width:%d",FileShare_Splitter->size().width());
    qDebug("FileShare_Splitter Height:%d",FileShare_Splitter->size().height());

    ui->widget_FileList->setGeometry(0,0,908,517);
    SurfingFile->setGeometry(0,0,908,517);

    DockWidget = new PropertiesWidget(ui->statusShow,ui); //显示在statusShow里

    QObject::connect(FileShare_Splitter,&QSplitter::splitterMoved,this,[=](){
        ui->widget_FileList->setGeometry(0,0,FileShare_Splitter->widget(0)->size().width(),FileShare_Splitter->widget(0)->size().height());
        SurfingFile->setGeometry(0,0,FileShare_Splitter->widget(0)->size().width(),FileShare_Splitter->widget(0)->size().height());
        DockWidget->setGeometry(0,0,FileShare_Splitter->widget(1)->size().width(),FileShare_Splitter->widget(1)->size().height());

        qDebug("Prop 1 Height:%d",FileShare_Splitter->widget(0)->size().height());
        qDebug("Prop 1 Width:%d",FileShare_Splitter->widget(0)->size().width());

        qDebug("SurfingFile Height:%d",SurfingFile->size().height());
        qDebug("SurfingFile Width:%d",SurfingFile->size().width());

        qDebug("Prop 2 Height:%d",FileShare_Splitter->widget(1)->size().height());
        qDebug("Prop 2 Width:%d",FileShare_Splitter->widget(1)->size().width());

        SplitterRecord = FileShare_Splitter->widget(1)->size().height();


    });

    QObject::connect(SurfingFile,&FileList::Upload,this,[this](QList<QUrl> DropList){
        Upload(DropList);
        Refresh();
    });

   //第一个值:其默认值本来就是 QWidget *parent = nullptr 即父级 也就是所谓的Mainwindow身上 重新指向nullptr意思就和上方的About()含义一致
   //第二个arg:获取ui信息 谁的? MainWindow的

    QTabWidget *tabWidget_contentShow = ui->tabWidget_contentShow;
    tabWidget_contentShow->setMovable(true);

    QTreeWidget *Filelist = SurfingFile;

    Filelist->setStyleSheet("QHeaderView::section{background:#A3C99FFF;}");
    Filelist->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QLineEdit *DownloadPathInput = ui->DownloadPath;
    QPushButton *Button_DownloadSurfingPath = ui->DownloadSurfingPath;

    std::string PlaceHolderText = std::filesystem::current_path().string(); //get Default Link

    //Declaration
    PlaceHolderText.insert(0,"Default Path:");
    PlaceHolderText.append("\\downloads");

    DownloadPathInput->setPlaceholderText(QString::fromStdString(std::move(PlaceHolderText)));

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

        QString currentPath = QDir::currentPath();

        QUrl DownloadPathInputSelect = QFileDialog::getExistingDirectoryUrl(
            nullptr,
            tr("SurfingPath for DownloadPathInput"),
            currentPath
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
    QObject::connect(TaskQueue,&QPushButton::clicked,this,[=](){
        showStatus(m_status);

        //disable下
        if(!ui->statusShow->isEnabled()){
            ui->widget_FileList->setGeometry(0,0,FileShare_Splitter->width(),FileShare_Splitter->height());
            SurfingFile->setGeometry(0,0,FileShare_Splitter->width(),FileShare_Splitter->height());

        }

        else{
            qDebug("Prop2.1 Height:%d", FileShare_Splitter->widget(1)->size().height());
            qDebug("Prop2.2 Height:%d", DockWidget->height());
            qDebug("Prop2.3 Height:%d", ui->statusShow->height());
            SurfingFile->setGeometry(0,0,FileShare_Splitter->width(),FileShare_Splitter->widget(0)->size().height()-SplitterRecord);
        }

    }); //item按下判断触发

    QObject::connect(ui->tabWidget_contentShow,&QTabWidget::tabBarClicked,this,&MainWindow::LostSelection);

    QObject::connect(this,&MainWindow::connetPressed,DockWidget,&PropertiesWidget::clearStatusList);
    QObject::connect(IP_controlPanelWindow,&IP_controlPanel::connetPressed,DockWidget,&PropertiesWidget::clearStatusList);

    //QObject是一个抽象类 似乎因为如此 connect需要用引用符号获得它的地址来进行操作
    //我真的特别谢谢QT还保留了地址的重载写法。。

    QObject::connect(DockWidget,&PropertiesWidget::Download,this,&MainWindow::itemAccess);
    QObject::connect(this,&MainWindow::DockProgressCreate,DockWidget,&PropertiesWidget::ProgressCreate);

    //对于静态对象 想融入进QObject的*sender 只需要继承了QObject之后 使用取地址符变成指向地址就好
    QObject::connect(Client1,&Connect::DownloadSpeedUpdate,this,[this](float bodySize){
        ui->label_DownloadSpeedValue->setText(
            QString::fromStdString(
                std::to_string(bodySize).substr(0,6) // 123.45
            )
        );
    });


//    QLabel *label_DownloadSpeedValue = ui->label_DownloadSpeedValue;

    QPushButton *pushButton_MaxThreadCount = ui->pushButton_MaxThreadCount;
    QSlider *ThreadsSlider = ui->horizontalSlider_MaxThreadCount;
    QLabel *label_MaxThreadCountValue = ui->label_MaxThreadCountValue;


    pushButton_MaxThreadCount->setStyleSheet(R"(
        QPushButton{
            border:none;
            background:none
        }

        QPushButton:pressed{
            border: none;
            background: none;
        }
    )");

    pushButton_MaxThreadCount->setCursor(Qt::PointingHandCursor);


    QParallelAnimationGroup *MaxThreadAnimation = new QParallelAnimationGroup(this);



    //初始位置:绝对
//    QPoint InitalAbsButtonPos = pushButton_MaxThreadCount->pos();
//    QPoint InitalAbsLabelPos = label_MaxThreadCountValue->pos();

//    //初始位置:相对于父级widget 绝对->相对
//    QPoint InitalRelButtonPos = pushButton_MaxThreadCount->mapFromGlobal(InitalAbsButtonPos);
//    QPoint InitalRelLabelPos = label_MaxThreadCountValue->mapFromGlobal(InitalAbsLabelPos);

    //目标运动结束后的相对位置 在Qpoint里默认以左上角为起始地址计算
    QPoint TargetRelButtonPos = QPoint(159,12);
    QPoint TargetRelLabelPos = QPoint(235,9);

    //相对位置转化为绝对目标位置 相对->绝对
    QPoint TargetAbsThreadButtonPos = pushButton_MaxThreadCount->mapToParent(TargetRelButtonPos);
    QPoint TargetAbsThreadLabelPos = label_MaxThreadCountValue->mapToParent(TargetRelLabelPos);

    QPropertyAnimation *FadingButton = new QPropertyAnimation(pushButton_MaxThreadCount,"pos");
    QPropertyAnimation *FadingLabel = new QPropertyAnimation(label_MaxThreadCountValue,"pos");


    QGraphicsOpacityEffect *SliderOpacity = new QGraphicsOpacityEffect(ThreadsSlider);
    SliderOpacity->setOpacity(1.0);                                                             //give value
    ThreadsSlider->setGraphicsEffect(SliderOpacity);                                            //apply opacity in QGraphicsOpacityEffect
    QPropertyAnimation *FadingSlider = new QPropertyAnimation(SliderOpacity,"opacity",this);    //QPropertyAnimation:Add opacity prop to uiWidget[QObject](ThreadsSlider)



    MaxThreadAnimation->addAnimation(FadingButton);
    MaxThreadAnimation->addAnimation(FadingLabel);
    MaxThreadAnimation->addAnimation(FadingSlider);

    FadingButton->setDuration(500);FadingButton->setEasingCurve(QEasingCurve::Linear);
    FadingLabel->setDuration(500);FadingLabel->setEasingCurve(QEasingCurve::Linear);
    FadingSlider->setDuration(500);FadingSlider->setEasingCurve(QEasingCurve::Linear);

//inital Status:
//    label_ThreadRange->hide();

    QObject::connect(pushButton_MaxThreadCount,&QPushButton::clicked,this,[=]{
        //pos改变之后 其他所有pos全部都跟着变 导致原有设置的hide show 以及动画带来的pos修改 全部重置

        FadingSlider->setEndValue(1.0);
        FadingButton->setEndValue(QPoint(4,12));
        FadingLabel->setEndValue(QPoint(80,9));

        MaxThreadAnimation->start();


    });

    QObject::connect(ThreadsSlider,&QSlider::sliderMoved,this,[=](int number){
        //神奇的是 在setText的过程中 似乎触发了。。类似DOM刷新的机制 让UI重绘了字体 导致字体的位置变回去了
        label_MaxThreadCountValue->setText(QString::fromStdString(std::to_string(number)));
    });

    QObject::connect(ThreadsSlider,&QSlider::sliderReleased,this,[=]{
        FadingSlider->setEndValue(0.0);
        FadingButton->setEndValue(TargetAbsThreadButtonPos);
        FadingLabel->setEndValue(TargetAbsThreadLabelPos);

        MaxThreadAnimation->start();
        pushButton_MaxThreadCount->raise();
    });





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

    QString itemName = listItem->text(nameList);
    QString itemSize = listItem->text(sizeList);
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

    if(ParentPath == rootPath) Refresh->setEnabled(false); //不要在磁盘界面选择刷新

    //signal Trigger add.
    QObject::connect(
        Refresh,
        &QAction::triggered,
        this,
        [this](){   //捕获this 以引入函数执行主体 this->mainwindow
            MainWindow::Refresh();
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
            MainWindow::Refresh();
        }
    );

    QObject::connect(NewDir,SIGNAL(triggered(bool)),this,SLOT(NewDir()));


    QObject::connect(
        Upload,
        &QAction::triggered,
        this,
        [this](){
            MainWindow::Upload();
            MainWindow::Refresh();
        }
    );

    if(itemSize!="—"){
        //file area
        qDebug("right clicked the file");
        QAction *Download = new QAction("Download");

        //奇怪的思路 居然是先指定action2然后再让action1插入到action2的前面
        //而不是直接声明action1 插入到 action2之前

        //在弹出之前 先过一个纯File判断

        QTreeWidget *Filelist = SurfingFile;
        selectedFileList = Filelist->selectedItems();

        if(selectedFileList.size()>1){
            for(auto &item:selectedFileList){
                QString selectedFileListsName = item->text(nameList);
                QString selectedFileListsSize = item->text(sizeList);


                if(selectedFileListsSize!="—"){
                    qDebug("you selected the %s,which size is:%s",item->text(nameList).toStdString().c_str(),item->text(sizeList).toStdString().c_str());
                }

                else{
                    qDebug("you select %s which is not a File!",selectedFileListsName.toStdString().c_str());
                    //此处也应弹出Download 但是是不可点击状态(灰色) 所以还是别直接return false完事
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

        

    }

    else{
        //dir area
        qDebug("right clicked the Dir");

        //Action add.
        QAction *Open = new QAction("Open");

        std::smatch match;

        FileList_popmenu->insertAction(Refresh,Open);
        std::string tempString = itemName.toStdString();

        std::regex_match(tempString, match, DiskReg);


        if(match[0] != ""){
            qDebug("you selected a Disk:%s",match[0].str().c_str());
            Delete->setEnabled(false);
            Rename->setEnabled(false);
            NewDir->setEnabled(false);
            Upload->setEnabled(false);
        }

        //signal Trigger add.
        //二度lambda 需要 通过this以及手动capture来捕获
        QObject::connect(Open,&QAction::triggered,this,[listItem,&column,this](){itemAccess(listItem,column);});


    }

     FileList_popmenu->move(SurfingFile->cursor().pos());    //菜单显示在鼠标点击的位置
     FileList_popmenu->show();

     return true;

}

void MainWindow::itemAccess(QTreeWidgetItem *listItem,int column){

//这里的column代表这一行里点击的位置判定(Icon->0/FileName->1/Size->2...) 不过我并没有对column作出什么更改需求

//  QString selectedFileListsIcon = listItem->text(iconList);
    QString selectedFileListsName = listItem->text(nameList);
    QString selectedFileListsSize = listItem->text(sizeList);
    QString selectedFileListsLink = listItem->text(linkList);

    if(selectedFileListsSize!="—"){
        QTreeWidget *Filelist = SurfingFile;
        selectedFileList = Filelist->selectedItems();

        QEventLoop DownloadLoop;
        QFutureWatcher<void> DownloadWatcher;


        if(selectedFileList.size()>1){

            QThreadPool DownloadPool;

            qDebug("ui->label_MaxThreadCountValue->text():%d",ui->label_MaxThreadCountValue->text().toInt());
            DownloadPool.setMaxThreadCount(ui->label_MaxThreadCountValue->text().toInt()); //额外最多允许3个线程

            int batch = 0;
            QList<QTreeWidgetItem *> TempList;


            for(auto &Item:selectedFileList){
                QTreeWidget *treeWidgetTaskQueue = DockWidget->ui->treeWidgetTaskQueue;
                QList matchList = treeWidgetTaskQueue->findItems(Item->text(nameList),Qt::MatchExactly,1);

                if (!matchList.empty()){
                    QMessageBox ReDownloadPrompt;

                    ReDownloadPrompt.setWindowTitle("重复下载确认");
                    ReDownloadPrompt.setText("你的下载列表已存在该文件:"+Item->text(nameList)+"\n是否要重新下载?");

                    QPushButton *AcceptButton = ReDownloadPrompt.addButton("Accept",QMessageBox::AcceptRole);
                    QPushButton *DeniedButton = ReDownloadPrompt.addButton("Cancel",QMessageBox::NoRole);

                    ReDownloadPrompt.exec();

                    if(ReDownloadPrompt.clickedButton() == DeniedButton){
                        selectedFileList.remove(selectedFileList.indexOf(Item));
                        continue;
                    }

                    if(ReDownloadPrompt.clickedButton() == AcceptButton){
                        emit DockProgressCreate(Item);
                    }

                }

            }


            while(batch*DownloadPool.maxThreadCount()<selectedFileList.size()){
                int residual = selectedFileList.size()-batch*DownloadPool.maxThreadCount();

                if(selectedFileList.size()-batch*DownloadPool.maxThreadCount()<DownloadPool.maxThreadCount()){
                    if(residual>1){
                        for(int i = 0;i<residual;i++){
                            TempList.append(selectedFileList.at(i+batch*DownloadPool.maxThreadCount()));
                        }

                        DownloadWatcher.setFuture(QtConcurrent::map(TempList,[&](QTreeWidgetItem *selectedItem){
                            QString selectedName = selectedItem->text(nameList);
                            QString selectedSize = selectedItem->text(sizeList);
                            QString selectedLink = selectedItem->text(linkList);

                            Client1->cliFileDownload(selectedName,selectedSize,selectedLink);
                        }));

                        QObject::connect(&DownloadWatcher,&QFutureWatcher<void>::finished,&DownloadLoop,&QEventLoop::quit);
                        TempList.clear();
                        batch+=1;

                    }

                    else{

                        TempList.append(selectedFileList.at(batch*DownloadPool.maxThreadCount()));

                        DownloadWatcher.setFuture(QtConcurrent::map(TempList,[&](QTreeWidgetItem *selectedItem){
                            QString selectedName = selectedItem->text(nameList);
                            QString selectedSize = selectedItem->text(sizeList);
                            QString selectedLink = selectedItem->text(linkList);
                            Client1->cliFileDownload(selectedName,selectedSize,selectedLink);
                        }));

                        QObject::connect(&DownloadWatcher,&QFutureWatcher<void>::finished,&DownloadLoop,&QEventLoop::quit);

                        TempList.clear();
                        batch+=1;
                    }
                }

                else{

                    QEventLoop insideLoop;

                    QFutureWatcher<void> insideWatcher;

                    for(int i = 0;i<DownloadPool.maxThreadCount();i++){
                        TempList.append(selectedFileList.at(i+batch*DownloadPool.maxThreadCount()));
                    }


                    for(auto List:TempList){
                        qDebug("number List:%s",List->text(nameList).toStdString().c_str());
                    }

                    insideWatcher.setFuture(QtConcurrent::map(TempList,[&](QTreeWidgetItem *selectedItem){
                        QString selectedName = selectedItem->text(nameList);
                        QString selectedSize = selectedItem->text(sizeList);
                        QString selectedLink = selectedItem->text(linkList);
                        Client1->cliFileDownload(selectedName,selectedSize,selectedLink);
                    }));

                    QObject::connect(&insideWatcher,&QFutureWatcher<void>::finished,&insideLoop,&QEventLoop::quit);

                    insideLoop.exec();

                    TempList.clear();
                    batch+=1;

                }

            }

            DownloadLoop.exec();
        }

        else{

            QTreeWidget *treeWidgetTaskQueue = DockWidget->ui->treeWidgetTaskQueue;
            QList matchList = treeWidgetTaskQueue->findItems(listItem->text(nameList),Qt::MatchExactly,1);

            if (!matchList.empty()){
                QMessageBox ReDownloadPrompt;

                ReDownloadPrompt.setWindowTitle("重复下载确认");
                ReDownloadPrompt.setText("你的下载列表已存在该文件:"+listItem->text(nameList)+"\n是否要重新下载?");
                ReDownloadPrompt.addButton("Accept",QMessageBox::AcceptRole);

                QPushButton *DeniedButton = ReDownloadPrompt.addButton("Cancel",QMessageBox::NoRole);

                ReDownloadPrompt.exec();

                if(ReDownloadPrompt.clickedButton() == DeniedButton) return;

            }


            emit DockProgressCreate(listItem);

            qDebug("you selected the %s,which size is:%s",selectedFileListsName.toStdString().c_str(),selectedFileListsSize.toStdString().c_str());
            DownloadWatcher.setFuture(QtConcurrent::run(&Connect::cliFileDownload,Client1,std::ref(selectedFileListsName),std::ref(selectedFileListsSize),std::ref(selectedFileListsLink)));
            QObject::connect(&DownloadWatcher,&QFutureWatcher<void>::finished,&DownloadLoop,&QEventLoop::quit);
            DownloadLoop.exec();

        }

        
    }

   else{
       qDebug("double clicked the itemName %s, it linked to:%s",selectedFileListsName.toStdString().c_str(),selectedFileListsLink.toStdString().c_str()); //colnmun指代 子信息
       std::string Information = Client1->cliFileSurfing(selectedFileListsLink);

       HTMLExtract(Information,LinkVector,PathVector,NameVector,SizeVector);

       if(PathVector.size()>1){

            /*
             *  D:
             *  D:/Downloads         -2
                D:/Downloads/xxx.jpg .back() = index.last()

            */

            qDebug("上一级Path:%s",PathVector.at(PathVector.size()-2).c_str());
            ParentPath = PathVector.at(PathVector.size()-2).c_str();
            SurfingPath = PathVector.back().c_str();
        }

        else{
            qDebug("上一级Path:/files");
            ParentPath = rootPath;
        }

        // qDebug("%s",Information.c_str());

        //Filelist update part.
        SurfingFile->clear();

        if(selectedFileListsName==".."&&selectedFileListsLink==rootPath){
           qDebug("redirect to the disk select.");

           Information = Client1->cliFileSurfing(rootPath);
           HTMLExtract(Information,LinkVector,NameVector);

           for(int index = 0;index<=NameVector.size()-1;++index){
               QList<QString> DiskInformation{"-",NameVector.at(index).c_str(),"—",LinkVector.at(index).c_str()};
               QTreeWidgetItem *newItem = new QTreeWidgetItem(DiskInformation);
               SurfingFile->addTopLevelItem(newItem);
            }

        }



        else{

            QList<QString> newItemInformation{"-","..","—",ParentPath};
            SurfingFile->addTopLevelItem(new QTreeWidgetItem(newItemInformation));

            if(!NameVector.size()){
                qDebug("empty File Open");
            }

            else{
                for(int index = 0;index<=NameVector.size()-1;++index){
                    QList<QString> newItemInformation{"-",NameVector.at(index).c_str(),SizeVector.at(index).c_str(),LinkVector.at(index).c_str()};
                    QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
                    SurfingFile->addTopLevelItem(newItem);
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

    QString oldItem = listItem->text(linkList);

    QString Renametitle = tr("远程重命名确认");
    QString RenameContext = "\n将该文件名重命名为";
    RenameContext.insert(0,listItem->text(nameList)); //当前listItem->Name

    QString newFileName = QInputDialog::getText(this, Renametitle, RenameContext, QLineEdit::Normal, listItem->text(nameList));
    //这里的最后一个arg为placeholder

    qDebug("Ask Server to Rename item:%s->%s",listItem->text(nameList).toStdString().c_str(),newFileName.toStdString().c_str());

    if(!newFileName.isEmpty()){
        Client1->cliFileRename(oldItem,newFileName);
    }

    else{
        qDebug("Empty Rename Quest Input,Rejected.");
    }
}

void MainWindow::Delete(){
    QList<QString> LinkList;

    selectedFileList = SurfingFile->selectedItems();

    QMessageBox RemoteDeletePrompt;

    RemoteDeletePrompt.setStyleSheet(
        R"(
            QLabel {
                min-width:300px;
                min-height:40px;
                font-size:12px;
            }
        )"
    );

    RemoteDeletePrompt.setWindowTitle("远端删除确认");
    RemoteDeletePrompt.setText("将要向服务器发送请求以删除下列项目:");

    QString DeleteFileList = "";

    for(auto& List:selectedFileList){
        qDebug("Ask for server to Delete item:%s",List->text(nameList).toStdString().c_str());
        LinkList.emplaceBack(List->text(linkList));
        DeleteFileList.append("\t"+List->text(nameList)+"\n");
    }

    RemoteDeletePrompt.setInformativeText(DeleteFileList);

    QPushButton *DeleteButton = RemoteDeletePrompt.addButton("Delete",QMessageBox::AcceptRole);
    QPushButton *DiscardButton = RemoteDeletePrompt.addButton("Discard",QMessageBox::RejectRole);

    RemoteDeletePrompt.exec();

    if(RemoteDeletePrompt.clickedButton() == DeleteButton){
        for(auto& List:selectedFileList){
            delete List;
        }
        Client1->cliFileDelete(LinkList);
    }

    else if(RemoteDeletePrompt.clickedButton() == DiscardButton){
       return;
    }

}

void MainWindow::Refresh(){
    SurfingFile->clear();
    qDebug("Surfingpath: %s",SurfingPath.toStdString().c_str());
    QList<QString> newItemInformation{"-","..","—",ParentPath};
    SurfingFile->addTopLevelItem(new QTreeWidgetItem(newItemInformation));

    std::string Information = Client1->cliFileSurfing(SurfingPath);

    HTMLExtract(Information,LinkVector,PathVector,NameVector,SizeVector);

    for(int index = 0;index<=SizeVector.size()-1;++index){
        QList<QString> newItemInformation{"-",NameVector.at(index).c_str(),SizeVector.at(index).c_str(),LinkVector.at(index).c_str()};
        QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
        SurfingFile->addTopLevelItem(newItem);
    }

}

void MainWindow::Upload(){
    UploadVector.clear();
    UploadVector.shrink_to_fit();

    qDebug("Upload File.");

    //说起来多集合的QString 得用官方提供的QStringList来装载
    //然而内部方法与vector却没什么不同

    QStringList UploadFiles = QFileDialog::getOpenFileNames(
        nullptr,
        tr("choose Files to upload"),
        "D:/All Local Downloads",
        tr("texts(*.txt *.ini *.log *.md);;images(*.jpg *.jpeg *.png *.bmp);;video files(*.mp4 *.avi *.flv *.mkv);;All files(*.*)")
    );

    if(UploadFiles.length()!=0){
    qDebug("UploadFiles length:%zu",UploadFiles.length());

    for(auto &File:UploadFiles){
        qDebug("Upload Prompt FileLink:%s",File.toStdString().c_str());
        UploadVector.emplace_back(File.toStdString());
    }

    QString UploadPath = SurfingPath.split("?path=").at(1);
    Client1->cliFileUpload(UploadPath); //注意 SurfingPath => http://....?path= 是不能直接用在cliFileUpload的req.params里的

    }
}

void MainWindow::Upload(QList<QUrl>& DropList){
    UploadVector.clear();
    UploadVector.shrink_to_fit();

    for(auto& DropItem:DropList){

        // file:///D:/cpp/app/HTTP-UI/HTTP-QT/connect.h erase -> D:/cpp/app/HTTP-UI/HTTP-QT/connect.h
        UploadVector.emplace_back(DropItem.toString().toStdString().erase(0,8));
    }

    QString UploadPath = SurfingPath.split("?path=").at(1);
    Client1->cliFileUpload(UploadPath); //注意 SurfingPath => http://....?path= 是不能直接用在cliFileUpload的req.params里的

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
    SurfingFile->clearSelection();
}

void MainWindow::showStatus(bool &m_status){
    if(m_status == false){
        ui->statusShow->setVisible(true);
        ui->statusShow->setEnabled(true);
        m_status = true;
    }

    else{
        ui->statusShow->setVisible(false);
        ui->statusShow->setEnabled(false);
        m_status = false;
    }
    
}

void MainWindow::clearStatusList(){
    qDebug("clear your List");
}

//事件定义区
//事件 不同于 信号与槽的高度封装 事件通常需要手动去配置 但同时自由度也比信号与槽高的多

void MainWindow::closeEvent(QCloseEvent *closeEvent){
    //question方法 会临时返回一个新的MessageBox对象 无需自己再去定义
    int choice = QMessageBox::question(this,tr("Quit"),tr("Are you sure to quit this application?"),QMessageBox::Yes|QMessageBox::No);

    switch(choice){
        case QMessageBox::Yes: {closeEvent->accept();qApp->quit();break;}
        case QMessageBox::No: {closeEvent->ignore();break;}
    }

}

void MainWindow::resizeEvent(QResizeEvent *resizeEvent){

    QSplitter *FileShare_Splitter = ui->PropTools;

    ui->widget_FileList->setGeometry(0,0,FileShare_Splitter->widget(0)->size().width(),FileShare_Splitter->widget(0)->size().height());
    SurfingFile->setGeometry(0,0,FileShare_Splitter->widget(0)->size().width(),FileShare_Splitter->widget(0)->size().height());

    ui->statusShow->setGeometry(0,0,FileShare_Splitter->widget(1)->size().width(),FileShare_Splitter->widget(1)->size().height());
    DockWidget->setGeometry(0,0,FileShare_Splitter->widget(1)->size().width(),FileShare_Splitter->widget(1)->size().height());
    SplitterRecord = FileShare_Splitter->widget(1)->size().height();

}


//快捷键定义区

void MainWindow::keyPressEvent(QKeyEvent *event){
    QList<QTreeWidgetItem*> selectedTreeWidgetItems = SurfingFile->selectedItems();

    switch(event->key()){
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            emit SurfingFile->itemDoubleClicked(selectedTreeWidgetItems.at(0), 0); //因为判断方式已经变更成selectedTreeWidgetItems了 因此简化掉for循环
            break;
        }

        case Qt::Key_Delete:{
                Delete();
            break;
        }

        case Qt::Key_Backspace:{
            if(LinkVector.size()!=0){
                QTreeWidgetItem *UpperFolder = SurfingFile->topLevelItem(0);
                emit SurfingFile->itemDoubleClicked(UpperFolder, 0); //退格->表示双击上层
                break;
            }
            
        }


    }

}


