#ifndef CONNECT_H
#define CONNECT_H

#include <string>

#include <QDebug>
#include <QObject>
#include "./dependences/extern_lib/httplib.h"


class Connect:public QObject{
    Q_OBJECT
public:
    bool cliPing(QString& IP,int& Port);

    std::string cliFileSurfing(QString& IP,int& Port);
    std::string cliFileSurfing(QString& IP,int& Port,QString& Postition);

    
    // int cliFileDownload(QString& IP,int& Port,QString& Postition,QString& itemName);
    void cliFileDownload(QString& IP,int& Port,QString& Postition,QString& itemName);
    void cliFileDownload(QString& IP,int& Port,QString& Postition,QString& itemName,QString& itemSize);
//    void cliFileDownload(QString& IP,int& Port,QString& Postition,QString& itemName,std::string& currentName,std::vector<std::string> &TaskVector);
    void cliFileUpload(QString& IP,int& Port,QString& TargetPosition,httplib::MultipartFormDataItems &items);

    // void DownloadSpeed(QString& itemName, float& FProgress);

    std::string ReadTheFile(QString &Qpath,std::string &Information);

private:
    void Abort();

signals:
//    void Downloading(int& progress);

    void testSignal(QString& itemName,float& FProgress);
    void Downloading(std::map<std::string,int> &TaskMap,std::string& Name,int &Progress);

};

#endif
