#include "connect.h"
#include <QDebug>
#include <future>
// #include <regex>
#include <fstream>

#include "./dependences/httplib.h"
#include "./dependences/json.hpp"

#define readyStatus std::future_status::ready
#define timeoutStatus std::future_status::timeout

using namespace httplib;

//C++ 11 using alias for class/template

using string = std::string;
using json = nlohmann::json;

// std::smatch match;

std::future_status status;

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


void Connect::cliFileDownload(QString& IP,int& Port,QString& Postition,QString& itemName){

    std::string fileName = itemName.toStdString().c_str();

    qDebug("download trigger Link:%s",Postition.toStdString().c_str());

    Client cli(IP.toStdString(),Port);


    std::string path = "./downloads/";
    path.append(fileName.c_str());
    // qDebug("FileName:%s",path.c_str());

    std::string body;

    auto res = cli.Get(Postition.toStdString().c_str(),
      [&](const char *data, size_t data_length) {
        body.append(std::move(data), data_length);
        return true;
     });


    qDebug("FileSize:%zu",body.size());


    WriteToFile(fileName,body);
    

}



void Connect::cliPost(QString& Position){
    qDebug()<<"Post Finished.";
    
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
    // std::regex TextFliter(R"(txt|ini|cfg|xml|json|xml)");
    // std::string FileExt = FileName.substr(FileName.find_last_of(".")+1);

    std::string path = "./downloads/";

    path.append(FileName.c_str());
    qDebug("path:%s",path.c_str());

    std::ofstream newFile(path, std::ios::binary);

    newFile<<Data;
    newFile.close();
        
}
