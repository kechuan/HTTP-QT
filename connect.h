#ifndef CONNECT_H
#define CONNECT_H

#include <string>

#include <QDebug>
#include <QObject>
#include <QFileInfo>

/*
测试 如果仅引入ui文件 能不能令非ui体系 仅仅只是QObject能不能直接操纵引入的ui布局。
*/

#include "ui_mainwindow.h"


class Connect:public QObject{
    Q_OBJECT
public:
    Connect(Ui::MainWindow *m_ui = nullptr);

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

    Ui::MainWindow *m_ui;

signals:
//    void ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSize,const QString& itemSpeed,const QString& itemLink);
    void ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSpeed);

    void DownloadSpeedUpdate(float& bodySize);
    void Downloading(std::map<std::string,int> &TaskMap,std::string& Name,int &Progress);


};

#endif
