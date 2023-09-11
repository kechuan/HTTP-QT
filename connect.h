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
    std::string cliFileSurfing(const QString& Postition);
    
    void cliFileDownload(const QString& itemName,const QString& itemSize,const QString& itemLink);
    void cliFileUpload(const QString& TargetPosition);

    void cliFileDelete(const QList<QString>& TargetPosition);
    void cliFileRename(const QString& oldItem,const QString& newFileName);

    void cliLogin(const QString& username,const QString& password);

    std::string ReadTheFile(const QString &Qpath,std::string &Information);

    //MultiThread new content


private:
    Ui::MainWindow *m_ui;

    void Abort();

signals:
    void ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSpeed);
    void DownloadSpeedUpdate(float& bodySize);

    void ToasterAction(const QString& itemName,const int &status);

    void CancelNotice();

public slots:


};

#endif
