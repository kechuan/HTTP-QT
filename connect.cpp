#include "connect.h"
#include "qthread.h"
#include "propertieswidget.h"

#include <QDebug>
#include <QFileInfo>
#include <fstream>
#include <filesystem> //C++ 17 to solve fstream wchar problem.

#include "./dependences/extern_lib/httplib.h"
#include "./dependences/sizeTextHandler.h"
#include "./dependences/enumIndex.h"

#include <QTimer>
#include <ctime>

using namespace httplib;

//C++ 11 using alias for class/template
using string = std::string;

extern std::string storagePath;
extern std::vector<std::string> UploadVector;

extern PropertiesWidget *DockWidget;

extern QString FullIP;
extern int Port;
extern int AccessLevel;

QTimer *UpdateProgressTimer;
bool UpdateProgressFlag = false;

clock_t start_t;
clock_t finish_t;

Connect::Connect(Ui::MainWindow *m_ui):m_ui(m_ui){
    qDebug() << "Connect ID:" << QThread::currentThreadId();
    UpdateProgressTimer = new QTimer(this);

    QObject::connect(UpdateProgressTimer,&QTimer::timeout,this,[]{
        UpdateProgressFlag = true;
    });

}

void Connect::Abort(){
    qDebug()<<"connect Abort.";
}

bool Connect::cliPing(){
    Client cli(FullIP.toStdString(),Port);

    //timeout handler prevent crash
    cli.set_connection_timeout(0,3000); //3s 0,3000 0前置则判断为ms 0后置则判断为s 这设计思路挺厉害的
    cli.set_read_timeout(3,0);
    cli.set_write_timeout(3,0);

    qDebug("Target FullIP %s created Port:%d.",FullIP.toStdString().c_str(),Port);
    if(auto res = cli.Post("/ping")){ //用if包裹其中 让其变为自动的0/1判断返回 因为res超时响应就会被赋予"0"
        if( res->status == 200){
            qDebug()<<"succ";
            //httplib的res返回就两种 status与body
            qDebug("%s",res->body.c_str());
            return true;
        }

    }

    else{
        auto err = res.error();
        //httplib.h 重载了to_string的功能 使其从枚举转化对应为 string
        qDebug("err code:%d, Information:%s",err,to_string(err).c_str());
    }

    return false;

}

std::string Connect::cliFileSurfing(){
    std::string body;
    Client cli(FullIP.toStdString(),Port);

    auto res = cli.Get("/file",
        [&](const char *data, size_t buffer_length) {
          body.append(data, buffer_length);
          return true;
        });

    return body;

}

std::string Connect::cliFileSurfing(const QString& Postition){
    std::string body;
    Client cli(FullIP.toStdString(),Port);
    auto res = cli.Get(Postition.toStdString(),
      [&](const char *data, size_t buffer_length) {
        body.append(data, buffer_length);
        return true;
      });
    return body;
}


void Connect::cliFileDownload(const QString& itemName,const QString& itemSize,const QString& itemLink){

    qDebug() << "Connect.cpp Line 101: thread cliFileDownload" << QThread::currentThreadId();

    Client cli(FullIP.toStdString(),Port);

    std::string fileName = itemName.toStdString();
    std::string Fullpath = storagePath;
    std::string fileSize = itemSize.toStdString();

    qDebug("download trigger Link:%s",itemLink.toStdString().c_str());


    //首先检验它是否真的存在 不存在直接创立该文件夹
    std::filesystem::exists(Fullpath)?:std::filesystem::create_directory(Fullpath);


//File Precreate Prevent storagePath Change Error.
    Fullpath.append(fileName.c_str());
    qDebug("Precreate empty File:%s",Fullpath.c_str());

    std::ofstream newFile(std::filesystem::u8path(Fullpath), std::ios::binary);

    if(!newFile.is_open()){
        qDebug()<<"create new File failed";
    }

    double fliterSize = StringToSize(fileSize);
    qDebug("sizeFliterr:%f",fliterSize);

    double FProgress;
    float RecordProgress = 0;
    double SpeedValue = 0;

    std::string DownloadContent;

    start_t = clock();

    QTimer::singleShot(0,this,[=]{
        qDebug() << "UpdateProgressTimer Start ID:" << QThread::currentThreadId();
        UpdateProgressTimer->start(500);
    });

    int CompletedSize = 0;

    //状态与锁
    bool isPaused = false;
    bool isCanceled = false;
    bool isLimited = false;

    int LimitedBuffer;
    char delayms;

    std::mutex Download_mtx;
    std::condition_variable Download_cv;
//    std::condition_variable Limited_cv;

    QObject::connect(DockWidget,&PropertiesWidget::TaskPaused,this,[&](QTreeWidgetItem *TaskItem){
        if(TaskItem->text(0) == itemName){
            std::unique_lock<std::mutex> Downloadlock(Download_mtx);
            isPaused = true;
            Download_cv.notify_one();
        }
    });

    QObject::connect(DockWidget,&PropertiesWidget::TaskContinue,this,[&](QTreeWidgetItem *TaskItem){
        if(TaskItem->text(0) == itemName){
            std::unique_lock<std::mutex> Downloadlock(Download_mtx);
            isPaused = false;
            Download_cv.notify_one();
        }
    });

    QObject::connect(DockWidget,&PropertiesWidget::TaskCancel,this,[&](QTreeWidgetItem *TaskItem){
        if(TaskItem->text(0) == itemName){
            std::unique_lock<std::mutex> Downloadlock(Download_mtx);
            isPaused = true;
            isCanceled = true;
            Download_cv.notify_one();
        }
    });


    QObject::connect(DockWidget,&PropertiesWidget::TaskSpeedLimit,this,[&](QTreeWidgetItem *TaskItem,int &bufferLength,char &ms){
        if(TaskItem->text(0) == itemName){
            std::unique_lock<std::mutex> Downloadlock(Download_mtx);
            isLimited = true;
            LimitedBuffer = bufferLength;
            delayms = ms;

        }
    });

    QObject::connect(DockWidget,&PropertiesWidget::TaskLimitSpeedCancel,this,[&](QTreeWidgetItem *TaskItem){
        if(TaskItem->text(0) == itemName){
            std::unique_lock<std::mutex> Downloadlock(Download_mtx);
            isLimited = false;

        }
    });

    auto res = cli.Get(itemLink.toStdString(),
      [&](const char *data, size_t buffer_length) {

        /* speedLimit Part
         * 最简单粗暴的方法 更改delay数值以及buffer长度
         * buffer_length 默认为 4096
         */

        if(isLimited){
            buffer_length = LimitedBuffer;
            std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
            //3072,1ms => 100KB/s
        }


        //Thread Pause Part

        std::unique_lock<std::mutex> DownloadLock(Download_mtx);
        Download_cv.wait(DownloadLock,[&]{
            if(isPaused){
                qDebug() << "Download: Paused, thread ID:" << QThread::currentThreadId();
                if(isCanceled){
                    return isPaused;
                }
                ToasterAction(itemName,TaskPaused);
            }
            return !isPaused;
        });

        //正常阶段到这里的isPaused一直是false 除非是由wait抛出来的true isPaused
        if(isPaused){
            qDebug() << "Download: Canceled.";
            newFile.close();
            emit CancelNotice();
            ToasterAction(itemName,TaskCanceled);
            return false;
        }


        newFile.write(data,buffer_length); //4kb缓存写入

        if(UpdateProgressFlag){
            std::ifstream sizeDetected(std::filesystem::u8path(Fullpath),std::ios::binary|std::ios::ate);
            CompletedSize = static_cast<int>(sizeDetected.tellg());
            sizeDetected.close();

            FProgress = (CompletedSize/fliterSize)*100;

            //inital Speed
            if(!RecordProgress){
                RecordProgress = FProgress;
                SpeedValue = (FProgress*fliterSize)/100;
            }

            else{
                float ProgressResidual;
                ProgressResidual = (FProgress - RecordProgress)/100;
                RecordProgress += FProgress - RecordProgress;
                SpeedValue = ProgressResidual*fliterSize;
            }

            QString itemSpeed = QString::fromStdString(SizeToString(SpeedValue));

            emit ProgressUpdate(itemName,FProgress,itemSpeed);

        }

        UpdateProgressFlag = false;

        return true;
    });

    //任务取消 直接抛出
    if(isCanceled) return;


    finish_t = clock();
    double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;//将时间转换为秒
    qDebug("CPU 占用的总时间:%f\n", total_t);

    emit ProgressUpdate(itemName,100,"—");
    emit ToasterAction(itemName,TaskCompleted);

    if(!CompletedSize){
        std::ifstream sizeDetected(std::filesystem::u8path(Fullpath),std::ios::binary|std::ios::ate);
        CompletedSize = static_cast<int>(sizeDetected.tellg());
        sizeDetected.close();
    }

    qDebug("Connect.cpp Line 186:FileSize:%d",CompletedSize);

    QTimer::singleShot(0,this,[=]{
        qDebug() << "UpdateProgressTimer stop ID:" << QThread::currentThreadId();
        UpdateProgressTimer->stop();
        m_ui->textBrowser_log->append(R"(<span style=" color:#ffffff;">)"+itemName+R"( Download Succ.</span>)");
    });

    newFile.close();

}

void Connect::cliFileUpload(const QString& QTargetPosition){
    Client cli(FullIP.toStdString(),Port);

    std::string TargetPosition = QTargetPosition.toStdString().c_str();
    qDebug("the TargetPosition is:%s",TargetPosition.c_str());

    httplib::MultipartFormData FormData;
    httplib::MultipartFormDataItems items; //->std::vector<httplib::MultipartFormData>

    for(auto& uploadFile:UploadVector){

        QFileInfo Uploadinfo(QString::fromStdString(uploadFile));
        std::ifstream TargetFile;

        TargetFile.open(std::filesystem::u8path(uploadFile),std::ios_base::binary);
        qDebug("open in binary way");

        if(!TargetFile.is_open()){
            qDebug("open failed");
        }

        else{
            //STL->istreambuf_iterators
            std::string filecontent((std::istreambuf_iterator<char>(TargetFile)), (std::istreambuf_iterator<char>()));

            FormData.name = "Files";
            FormData.filename = std::move(Uploadinfo.fileName().toStdString());
            FormData.content = std::move(filecontent);
            FormData.content_type = std::move(Uploadinfo.suffix().toStdString());

            TargetFile.close();
            items.emplace_back(FormData);

        }
        qDebug("UploadVector Size:%zu",UploadVector.size());


    }


    if (auto ping = cli.Post("/ping")){
        if(ping->status==200){
            qDebug()<<"server ok";
            auto upload = cli.Post("/upload/"+TargetPosition, items);
        }
    }

}

void Connect::cliFileRename(const QString& oldItem,const QString& newFileName){
    Client cli(FullIP.toStdString(),Port);

    httplib::Params RenameParams;

    if(auto ping = cli.Post("/ping")){
        if(ping->status==200){
            qDebug()<<"server ok";

            RenameParams.emplace("oldItem",oldItem.toStdString());
            RenameParams.emplace("newName",newFileName.toStdString());

            auto RenameAction = cli.Post("/rename",RenameParams);
        }
    }
}

void Connect::cliLogin(const QString& username,const QString& password){
    Client cli(FullIP.toStdString(),Port);

    httplib::Params LoginParams;

    if(auto ping = cli.Post("/ping")){
        if(ping->status==200){
            qDebug()<<"server ok";

            LoginParams.emplace("username",username.toStdString());
            LoginParams.emplace("password",password.toStdString());

            if(auto LoginAction = cli.Post("/apiLogin",LoginParams)){
                if( LoginAction->status == 200){
                    qDebug("AccessLevel:%s",LoginAction->body.c_str());

                    if(LoginAction->body == "Guest") AccessLevel = Guest;
                    if(LoginAction->body == "User") AccessLevel = User;
                    if(LoginAction->body == "Admin") AccessLevel = Admin;

                }
            }
        }


    }
}


void Connect::cliFileDelete(const QList<QString>& TargetPosition){
    Client cli(FullIP.toStdString(),Port);

    httplib::Params DeleteParams;

    for(auto& File:TargetPosition){
        std::string FileStr = File.toStdString().c_str();
        DeleteParams.emplace("Postition",FileStr);
    }



    if (auto ping = cli.Post("/ping")){
        if(ping->status==200){
            qDebug()<<"server ok";
            auto DeleteAction = cli.Post("/delete",DeleteParams);
        }
    }

}

void ReadTheFile(const QString &Qpath,std::string &Information){
    std::ifstream TargetFile;
    std::string path = Qpath.toStdString();

    if(path.substr(path.find_last_of(".")+1) != "txt" ){
        // cout<<"open in binary way"<<endl;
        TargetFile.open(path,std::ios_base::binary);

    }

    else{
        TargetFile.open(path);
    }


    if(!TargetFile.is_open()){
        qDebug()<<"open failed.";
    }

    else{

        //STL->istreambuf_iterators
        std::string Data((std::istreambuf_iterator<char>(TargetFile)), (std::istreambuf_iterator<char>()));
        Information = std::move(Data);
        TargetFile.close();

        qDebug("the size of the File:%zu",Information.size());

    }


}

