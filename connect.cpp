#include "connect.h"
//#include "qtimer.h"
#include <QDebug>
#include <fstream>
#include <filesystem> //C++ 17 to solve fstream wchar problem.

#include <future>
#include <chrono> //c++ 11计时库

#include <regex>

//#include "updateUiThread.h"

#define readyStatus std::future_status::ready
#define timeoutStatus std::future_status::timeout


#include "./dependences/extern_lib/httplib.h"

using namespace httplib;

//C++ 11 using alias for class/template

using string = std::string;

extern std::map<std::string,int> TaskMap;
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

void Connect::cliFileDownload(QString& itemLink,QString& itemName,QString& itemSize){

    Client cli(FullIP.toStdString(),Port);
    std::string fileName = itemName.toStdString();

    qDebug("download trigger Link:%s",itemLink.toStdString().c_str());

    std::string path = "./downloads/";
    path.append(fileName);

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

//    qRegisterMetaType<float&>("float&");
    //???连我也看不懂为什么invokeMethod不会自带注册float& 但是神奇的是
    //如果直接float糊脸 md 槽函数就会认为是float 因此与 float& 不相同而不响应

    // QTimer::singleShot(0, this, SIGNAL(ProgressUpdate(itemName,FProgress)));
    //QMetaObject::invokeMethod(this, "ProgressUpdate", Q_ARG(QString, itemName), Q_ARG(QString, itemSize), Q_ARG(float&, FProgress));
    emit ProgressUpdate(itemName,itemSize,itemLink,FProgress);

    auto res = cli.Get(itemLink.toStdString(),
      [&](const char *data, size_t data_length) {
        body.append(std::move(data), data_length);
        
        FProgress = (body.size()*100/fliterSize);
        qDebug("Line 120:Progress:%f",FProgress);

        if(intervalflag == 2500){
            intervalflag = 0;
            emit ProgressUpdate(itemName,itemSize,itemLink,FProgress);
        }

        ++intervalflag;
       return true;
    });

    emit ProgressUpdate(itemName,itemSize,itemLink,FProgress);

    qDebug() << "Connect.cpp Line 124: thread cliFileDownload" << QThread::currentThreadId();

    qDebug("Connect.cpp Line 84:FileSize:%zu",body.size());
    WriteToFile(DownloadPath,fileName,body);

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

void WriteToFile(std::string path,std::string& FileName,std::string& Data){ //overload
    path.append(FileName.c_str());
    qDebug("path:%s",path.c_str());

//    std::string realPath = path.
//方案一 将string转成wchar_t
//方案二(C++ 17 add,C++ 20 deprecated) 使用std::filesystem::u8path(path) 来使用utf-8编码对path进行操作

    std::ofstream newFile(std::filesystem::u8path(path), std::ios::binary);

    newFile<<Data;
    newFile.close();
        
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
