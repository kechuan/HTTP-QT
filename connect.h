#ifndef CONNECT_H
#define CONNECT_H

#include <string>

#include <QDebug>
#include <QObject>

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

    void cliLogin(QString& username,QString& password);

    std::string ReadTheFile(QString &Qpath,std::string &Information);

    //MultiThread new content


private:
    Ui::MainWindow *m_ui;

    void Abort();

signals:
    void ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSpeed);
    void DownloadSpeedUpdate(float& bodySize);
    void ToasterShow(const QString& itemName);
    void CancelNotice();

public slots:


};

#endif
