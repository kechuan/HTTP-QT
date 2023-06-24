#ifndef FILELIST_H
#define FILELIST_H

#include <QTreeWidget>
#include <QList>
#include <QUrl>

//#include "ui_mainwindow.h"

class FileList:public QTreeWidget{
    Q_OBJECT
public:
    explicit FileList(QWidget *parent);

    ~FileList();

    void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
    void dropEvent(QDropEvent* dropEvent);
    void dragMoveEvent(QDragMoveEvent *dragMoveEvent);
    void resizeEvent(QResizeEvent *resizeEvent);

signals:
    void Upload(QList<QUrl> &Droplist);
    // void Refresh();

};


#endif
