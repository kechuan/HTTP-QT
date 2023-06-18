#include "FileList.h"

#include <QDebug>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QMimeData>
#include <QWidget>

//FileList::FileList(QTreeWidget *ParentWidget):QTreeWidget(ParentWidget){
FileList::FileList(QWidget *ParentWidget):QTreeWidget(ParentWidget){
    QList<QString> newItemInformation{"-","..","—","testForFileList"};

    this->setHeaderItem(new QTreeWidgetItem(newItemInformation));
    this->addTopLevelItem(new QTreeWidgetItem(newItemInformation));
    this->setAcceptDrops(true);



    this->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

//    this->setMinimumSize(650,350);
    this->setMaximumSize(16777215,16777215);
    this->setGeometry(32,22,650,350);

//    this->setDragEnabled(true);
    qDebug("FileList created");
}

FileList::~FileList(){
    qDebug("FileList deleted");
}

void FileList::dragMoveEvent(QDragMoveEvent *dragMoveEvent){
    dragMoveEvent->acceptProposedAction();
}

void FileList::dragEnterEvent(QDragEnterEvent *dragEnterEvent){

    if(dragEnterEvent->mimeData()->hasUrls()){
        dragEnterEvent->acceptProposedAction();
    }

    else{
        dragEnterEvent->ignore();
    }
}

void FileList::dropEvent(QDropEvent *dropEvent){
    if(dropEvent->mimeData()->hasUrls()){
        QList<QUrl> list = dropEvent->mimeData()->urls();//获取数据并保存到链表中
            for(int i = 0;i<list.length();i++){
                qDebug() << list[i].toLocalFile();
            }
        }
}

void FileList::resizeEvent(QResizeEvent *resizeEvent){
//    this->setMinimumSize(resizeEvent->size());
    QWidget::resizeEvent(resizeEvent);
}
