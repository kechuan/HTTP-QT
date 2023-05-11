#ifndef CONNECT_H
#define CONNECT_H

#include <string>

#include <QDebug>
#include <QObject>
#include "./dependences/extern_lib/httplib.h"

class Connect:public QObject{
    Q_OBJECT
public:
    bool cliPing();

    std::string cliFileSurfing();
    std::string cliFileSurfing(QString& Postition);

    
    void cliFileDownload(QString& itemLink,QString& itemName,QString& itemSize);
    void cliFileUpload(QString& TargetPosition,httplib::MultipartFormDataItems &items);

    void cliFileDelete(QList<QString>& TargetPosition);

    std::string ReadTheFile(QString &Qpath,std::string &Information);

private:
    void Abort();

signals:
    void ProgressUpdate(const QString& itemName,const QString& itemSize,QString& itemLink,float &FProgress);
    void Downloading(std::map<std::string,int> &TaskMap,std::string& Name,int &Progress);

};

#endif
