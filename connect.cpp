#include "connect.h"
#include "qthread.h"

#include <QDebug>
#include <fstream>
#include <filesystem> //C++ 17 to solve fstream wchar problem.

#include <regex>

#include "./dependences/extern_lib/httplib.h"
#include "./dependences/sizeTextHandler.h"

#include <QTimer>
#include <ctime>

using namespace httplib;

//C++ 11 using alias for class/template
using string = std::string;

extern std::string DownloadPath;
extern std::vector<std::string> UploadVector;

extern QString FullIP;
extern int Port;

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

void WriteToFile(std::string DownloadPath,std::string& FileName,std::string& Data); //预声明

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
        qDebug("err code:%d, Information:%s",err,to_string(err).c_str());
    }

    return false;

}

std::string Connect::cliFileSurfing(){
    std::string body;
    Client cli(FullIP.toStdString(),Port);

    auto res = cli.Get("/file",
        [&](const char *data, size_t data_length) {
          body.append(data, data_length);
          return true;
        });

    return body;

}

std::string Connect::cliFileSurfing(QString& Postition){
    std::string body;
    Client cli(FullIP.toStdString(),Port);
    auto res = cli.Get(Postition.toStdString(),
      [&](const char *data, size_t data_length) {
        body.append(data, data_length);
        return true;
      });
    return body;
}



void WriteToFile(std::string path,std::string& FileName,std::string& Data){ //overload
    qDebug("Connect.cpp Line 91: path:%s",path.c_str());

    //方案一 将string转成wchar_t
    //方案二(C++ 17 add,C++ 20 deprecated) 使用std::filesystem::u8path(path) 来使用utf-8编码对path进行操作

    std::ofstream writeFile(std::filesystem::u8path(path), std::ios::binary);

    writeFile<<Data;
    writeFile.close();

}


void Connect::cliFileDownload(QString& itemName,QString& itemSize,QString& itemLink){

    qDebug() << "Connect.cpp Line 108: thread cliFileDownload" << QThread::currentThreadId();

    Client cli(FullIP.toStdString(),Port);

    std::string fileName = itemName.toStdString();
    std::string Fullpath = DownloadPath;
    std::string fileSize = itemSize.toStdString();

    qDebug("download trigger Link:%s",itemLink.toStdString().c_str());


//File Precreate Prevent DownloadPath Change Error.
    Fullpath.append(fileName.c_str());
    qDebug("Precreate empty File:%s",Fullpath.c_str());

    std::ofstream newFile(std::filesystem::u8path(Fullpath), std::ios::binary);

    if(!newFile.is_open()){
        qDebug()<<"create new File failed";
    }

    double fliterSize = StringToSize(fileSize);
    qDebug("sizeFliterr:%f",fliterSize);

    float FProgress;
    float RecordProgress = 0;
    double SpeedValue = 0;

    //一般 string 的处理 因为其成员函数是append 则天生为复制一份再写入

    /*
        对于大型字符串，这可能会导致较大的性能开销。
        这涉及到内存复制和内存分配，可能导致动态内存分配和数据的复制。
    */
    std::string DownloadContent;

    //那么如果我用emplace_back呢?
    std::vector<std::string> DownloadContentVector;

    start_t = clock();

    QTimer::singleShot(0,this,[=]{
        qDebug() << "UpdateProgressTimer Start ID:" << QThread::currentThreadId();
        UpdateProgressTimer->start(500);
    });

    double totalSize = 0;
    int VectorSize = 0;

    auto res = cli.Get(itemLink.toStdString(),
      [&](const char *data, size_t data_length) {

        //原型: std::string& append(const char* data, size_t data_length);
        //意思是将 data的 前data_length位 添加到末尾

        DownloadContentVector.emplace_back(std::move(data), data_length);
        QString itemSpeed;

        if(UpdateProgressFlag){

            if(!VectorSize){
                for (const std::string& str : DownloadContentVector) {
                    totalSize += str.size();
                }
                VectorSize = DownloadContentVector.size();   //end Size
                qDebug("totalSize:%f , VectorSize:%d",totalSize,VectorSize);
            }

            else{
                for (int Loop = VectorSize; Loop < DownloadContentVector.size(); ++Loop) {
                    totalSize += DownloadContentVector[VectorSize].size();
                }
                VectorSize = DownloadContentVector.size();
                qDebug("totalSize:%f , VectorSize:%d",totalSize,VectorSize);
            }

            FProgress = (totalSize*100/fliterSize);

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

            itemSpeed = QString::fromStdString(SizeToString(SpeedValue));

            emit ProgressUpdate(itemName,FProgress,itemSpeed);

        }

        UpdateProgressFlag = false;


       return true;
    });

    //迁移回std::string
    for(std::string& Data:DownloadContentVector){
        DownloadContent.append(std::move(Data));
    }

    DownloadContentVector.shrink_to_fit();

    finish_t = clock();
    double total_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;//将时间转换为秒
    qDebug("CPU 占用的总时间:%f\n", total_t);

    emit ProgressUpdate(itemName,100,"—");

    qDebug("Connect.cpp Line 186:FileSize:%zu",DownloadContent.size());

    QTimer::singleShot(0,this,[=]{
        qDebug() << "UpdateProgressTimer stop ID:" << QThread::currentThreadId();
        UpdateProgressTimer->stop();
        m_ui->textBrowser_log->append(R"(<span style=" color:#ffffff;">)"+itemName+R"( Download Succ.</span>)");
    });

    WriteToFile(Fullpath,fileName,DownloadContent);

}


void Connect::cliFileUpload(QString& QTargetPosition){
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

void Connect::cliFileRename(QString& oldItem,QString& newFileName){
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


void Connect::cliFileDelete(QList<QString>& TargetPosition){
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

void ReadTheFile(QString &Qpath,std::string &Information){
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

