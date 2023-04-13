#ifndef CONNECT_H
#define CONNECT_H

#include <string>

#include <QDebug>
#include <QObject>
#include "./dependences/extern_lib/httplib.h"

#include <QThread>

class Connect:public QObject{
    Q_OBJECT
public:
    bool cliPing(QString& IP,int& Port);

    std::string cliFileSurfing(QString& IP,int& Port);
    std::string cliFileSurfing(QString& IP,int& Port,QString& Postition);

    
    void cliFileDownload(QString& IP,int& Port,QString& Postition,QString& itemName,QString& itemSize);
    void cliFileUpload(QString& IP,int& Port,QString& TargetPosition,httplib::MultipartFormDataItems &items);

    std::string ReadTheFile(QString &Qpath,std::string &Information);

private:
    void Abort();

signals:
    void testSignal(const QString& itemName,const QString& itemSize,float &FProgress);
    void Downloading(std::map<std::string,int> &TaskMap,std::string& Name,int &Progress);

};

#endif
