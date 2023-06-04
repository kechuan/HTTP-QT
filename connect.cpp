#include "connect.h"
#include "qthread.h"

#include <QDebug>
#include <fstream>
#include <filesystem> //C++ 17 to solve fstream wchar problem.

#include <future>

#include <regex>

#include "./dependences/extern_lib/httplib.h"
#include <QTimer>

using namespace httplib;

//C++ 11 using alias for class/template

using string = std::string;

extern std::string DownloadPath;


extern QString FullIP;
extern int Port;


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

//    std::string realPath = path.
//方案一 将string转成wchar_t
//方案二(C++ 17 add,C++ 20 deprecated) 使用std::filesystem::u8path(path) 来使用utf-8编码对path进行操作

    std::ofstream writeFile(std::filesystem::u8path(path), std::ios::binary);

    writeFile<<Data;
    writeFile.close();

}


void Connect::cliFileDownload(QString& itemName,QString& itemSize,QString& itemLink){

    qDebug() << "Connect.cpp Line 184: thread cliFileDownload" << QThread::currentThreadId();

    Client cli(FullIP.toStdString(),Port);
    std::string fileName = itemName.toStdString();
    std::string Fullpath = DownloadPath;

    qDebug("download trigger Link:%s",itemLink.toStdString().c_str());

    std::string body;
    std::string total_size = itemSize.toStdString();

    std::regex storageReg(R"(\d{1,3}.?\d{2}?(B|KB|MB|GB)$)");
    std::smatch matches;

    std::regex_search(total_size, matches, storageReg);
    std::string storageType = std::move(matches[1]); //captureList group1

    double fliterSize;

    if(storageType=="B"){
        total_size = std::move(matches[0]);
        fliterSize = stoi(total_size);
    }

    else if(storageType=="KB"){
        total_size = std::move(matches[0]);
        fliterSize = stod(total_size);
        fliterSize*=1024;
    }

    else if(storageType=="MB"){
        total_size = std::move(matches[0]);
        fliterSize = stod(total_size);
        fliterSize = fliterSize*1024*1024;
    }

    else{
        total_size = std::move(matches[0]);
        fliterSize = stod(total_size);
        fliterSize = fliterSize*1024*1024*1024;
    }

    int intervalflag = 0;
    float FProgress;



//File Precreate Prevent DownloadPath Change Error.
    Fullpath.append(fileName.c_str());
    qDebug("Precreate empty File:%s",Fullpath.c_str());
//    emit ProgressUpdate(itemName,itemSize,itemLink,FProgress);

    std::ofstream newFile(std::filesystem::u8path(Fullpath), std::ios::binary);

    if(!newFile.is_open()){
        qDebug()<<"create new File failed";
    }


    float floatSpeed = 0;

    float RecordProgress = 0;

    auto res = cli.Get(itemLink.toStdString(),
      [&](const char *data, size_t data_length) {

//        floatSpeed += sizeof(data);

        body.append(std::move(data), data_length);
        
        FProgress = (body.size()*100/fliterSize);

        QString itemSpeed;

        //25000 tick? update
        if(intervalflag == 25000){

            if(!RecordProgress){
                RecordProgress = FProgress;
                floatSpeed = (FProgress*fliterSize)/100;
                qDebug("inital Float Speed:%f",floatSpeed);
//                itemSpeed = QString::fromStdString(std::to_string(FProgress*fliterSize));
            }

            //After

            else{
                float Residual;
                Residual = (FProgress - RecordProgress)/100;
                floatSpeed = Residual*fliterSize;
//                itemSpeed = QString::fromStdString(std::to_string(Residual*fliterSize));
            }

            if(floatSpeed>1024*1024*1024){
                floatSpeed = floatSpeed/1024/1024/1024;
                itemSpeed = QString::fromStdString(std::to_string(floatSpeed).substr(0,6))+"GB/s";
            }

            else if(floatSpeed>1024*1024){

                floatSpeed = floatSpeed/1024/1024;
                itemSpeed = QString::fromStdString(std::to_string(floatSpeed).substr(0,6))+"MB/s";

            }

            else if(floatSpeed>1024){

                floatSpeed = floatSpeed/1024;
                itemSpeed = QString::fromStdString(std::to_string(floatSpeed).substr(0,6))+"KB/s";

            }

            else{
                itemSpeed = QString::fromStdString(std::to_string(floatSpeed).substr(0,6))+"B/s";
            }


            qDebug("Second Float Speed:%f",floatSpeed);

            emit ProgressUpdate(itemName,FProgress,itemSize,itemSpeed,itemLink);

            floatSpeed = 0;
            intervalflag = 0;



        }

        ++intervalflag;

       return true;
    });


//    DownloadCounterPerSecond->stop();

    emit ProgressUpdate(itemName,FProgress,itemSize,"—",itemLink);

    qDebug("Connect.cpp Line 186:FileSize:%zu",body.size());

    WriteToFile(Fullpath,fileName,body);

}

void Connect::cliFileUpload(QString& QTargetPosition,httplib::MultipartFormDataItems &items){
    Client cli(FullIP.toStdString(),Port);
    qDebug("the TargetPosition is:%s",QTargetPosition.toStdString().c_str());

    std::string TargetPosition = QTargetPosition.toStdString().c_str();

    if (auto ping = cli.Post("/ping")){
        if(ping->status==200){
            qDebug()<<"server ok";
            auto upload = cli.Post("/upload/"+TargetPosition, items);
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
