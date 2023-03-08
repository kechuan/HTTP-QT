#include "connect.h"
#include <QDebug>
#include <future>

#include "./dependences/httplib.h"
#include "./dependences/json.hpp"

#define readyStatus std::future_status::ready
#define timeoutStatus std::future_status::timeout

using namespace httplib;

//C++ 11 using alias for class/template

using string = std::string;
using json = nlohmann::json;

std::future_status status;


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


void Connect::cliFileDownload(QString& IP,int& Port,QString& Postition){
    qDebug("download trigger Link:%s",Postition.toStdString().c_str());
    std::string body;
    Client cli(IP.toStdString(),Port);

    auto res = cli.Get(Postition.toStdString().c_str(),
      [&](const char *data, size_t data_length) {
        body.append(data, data_length);
        return true;
     });

    qDebug("%s",body.c_str());

}



void Connect::cliPost(QString& Position){
    qDebug()<<"Post Finished.";
    
}



bool Connect::cliPing(QString& IP,int& Port){
    Client cli(IP.toStdString(),Port);
    qDebug("Target IP %s created Port:%d.",IP.toStdString().c_str(),Port);
    if(auto res = cli.Post("/ping")){
        if( res->status == 200){
            qDebug()<<"succ";
            //httplib的res返回就两种 status与body
            qDebug("%s",res->body.c_str());
            return true;
        }

        else{
            qDebug()<<"cli ping failed.";
            return false;
        }
    }

    else{
        qDebug()<<res->status;
        qDebug()<<"server no respone";
        return false;
    }

    
    
}





