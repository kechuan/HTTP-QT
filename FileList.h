#ifndef FILELIST_H
#define FILELIST_H

#include <QTreeWidget>
#include <QList>
#include <QUrl>

class FileList:public QTreeWidget{
    Q_OBJECT
public:
    explicit FileList(QWidget *parent);

    ~FileList();

    void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
    void dropEvent(QDropEvent* dropEvent);
    void dragMoveEvent(QDragMoveEvent *dragMoveEvent);
    void resizeEvent(QResizeEvent *resizeEvent);

//    void mousePressEvent(QMouseEvent *mousePressEvent);
    void mouseMoveEvent(QMouseEvent *mouseMoveEvent);

    void renderIcon(char &IconIndex);


signals:
    void Upload(QList<QUrl> &Droplist);
    void DragingFile();
    void DropStop();

};


#endif
