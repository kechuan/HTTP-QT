#ifndef FILELIST_H
#define FILELIST_H

#include <QTreeWidget>
#include <QObject>
#include <QWidget>

#include "ui_mainwindow.h"

class FileList:QTreeWidget{
    Q_OBJECT
public:
    explicit FileList(QWidget *parent);

    ~FileList();

    void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
    void dropEvent(QDropEvent* dropEvent);
    void dragMoveEvent(QDragMoveEvent *dragMoveEvent);
    void resizeEvent(QResizeEvent *resizeEvent);


private:
    Ui::MainWindow *m_ui; //从此Mainwindow下的指代的指针则为ui  MainWindow *ui = new Ui::MainWindow();
    QTreeWidget *FileListWidget;

};


#endif
