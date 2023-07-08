#ifndef CONNECT_H
#define CONNECT_H

#include <string>

#include <QDebug>
#include <QObject>
#include <QFileInfo>


class Connect:public QObject{
    Q_OBJECT
public:
    bool cliPing();

    std::string cliFileSurfing();
    std::string cliFileSurfing(QString& Postition);
    
    void cliFileDownload(QString& itemName,QString& itemSize,QString& itemLink);
    void cliFileUpload(QString& TargetPosition);

    void cliFileDelete(QList<QString>& TargetPosition);
    void cliFileRename(QString& oldItem,QString& newFileName);

    std::string ReadTheFile(QString &Qpath,std::string &Information);


private:
    void Abort();

signals:
    void ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSize,const QString& itemSpeed,const QString& itemLink);
    void DownloadSpeedUpdate(float& bodySize);
    void Downloading(std::map<std::string,int> &TaskMap,std::string& Name,int &Progress);

};

#endif
