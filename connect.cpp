#include "connect.h"

#include <QDebug>
#include <fstream>
#include <filesystem> //C++ 17 to solve fstream wchar problem.

#include <future>
#include <chrono> //c++ 11计时库

#include <regex>

#define readyStatus std::future_status::ready
#define timeoutStatus std::future_status::timeout


#include "./dependences/extern_lib/httplib.h"

using namespace httplib;

//C++ 11 using alias for class/template

using string = std::string;

extern std::map<std::string,int> TaskMap;

void WriteToFile(std::string& FileName,std::string& Data); //预声明

void Connect::Abort(){
    qDebug()<<"connect Abort.";
}

std::string Connect::cliFileSurfing(QString& IP,int& Port){
    std::string body;
    Client cli(IP.toStdString(),Port);

    auto res = cli.Get("/file",
        [&](const char *data, size_t data_length) {
          body.append(data, data_length);
          return true;
        });

    return body;
    
}

std::string Connect::cliFileSurfing(QString& IP,int& Port,QString& Postition){
    std::string body;
    Client cli(IP.toStdString(),Port);
    auto res = cli.Get(Postition.toStdString(),
      [&](const char *data, size_t data_length) {
        body.append(data, data_length);
        return true;
      });
    return body;
}

void Connect::cliFileDownload(QString& IP,int& Port,QString& Postition,QString& itemName,QString& itemSize){
    Client cli(IP.toStdString(),Port);
    std::string fileName = itemName.toStdString();

    qDebug("download trigger Link:%s",Postition.toStdString().c_str());

    std::string path = "./downloads/";
    path.append(fileName);
    qDebug("FileName:%s",path.c_str());

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

    emit testSignal(itemName,FProgress);//进度条初始化

    auto res = cli.Get(Postition.toStdString(),
      [&](const char *data, size_t data_length) {
        body.append(std::move(data), data_length);
        
        FProgress = (body.size()*100/fliterSize);
        if(intervalflag == 1000){
            intervalflag = 0;
            emit testSignal(itemName,FProgress);
        }

        ++intervalflag;
       return true;
    });

    emit testSignal(itemName,FProgress);
    qDebug("Connect.cpp Line 84:FileSize:%zu",body.size());
    WriteToFile(fileName,body);

}



void Connect::cliFileUpload(QString& IP,int& Port,QString& QTargetPosition,httplib::MultipartFormDataItems &items){
    Client cli(IP.toStdString(),Port);
    qDebug("the TargetPosition is:%s",QTargetPosition.toStdString().c_str());

    std::string TargetPosition = QTargetPosition.toStdString().c_str();

    if (auto ping = cli.Post("/ping")){

        if(ping->status==200){
            qDebug()<<"server ok";
            auto upload = cli.Post("/upload/"+TargetPosition, items);
        }
    }
}



bool Connect::cliPing(QString& IP,int& Port){
    Client cli(IP.toStdString(),Port);

    //timeout handler prevent crash
    cli.set_connection_timeout(0,3000); //3s 0,3000 0前置则判断为ms 0后置则判断为s 这设计思路挺厉害的
    cli.set_read_timeout(3,0);
    cli.set_write_timeout(3,0);

    qDebug("Target IP %s created Port:%d.",IP.toStdString().c_str(),Port);
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


void WriteToFile(std::string& FileName,std::string& Data){ //overload
    std::string path = "./downloads/";
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
