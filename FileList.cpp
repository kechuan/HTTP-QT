#include "FileList.h"


#include <QDebug>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QMimeData>
#include <QWidget>
#include <QHeaderView>

FileList::FileList(QWidget *ParentWidget):QTreeWidget(ParentWidget){

    this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    this->header()->setSectionResizeMode(0,QHeaderView::Fixed);
    this->setColumnWidth(0,80);




    QList<QString> InitalHeader{"Icon","Filename","Size"};
    this->setHeaderItem(new QTreeWidgetItem(InitalHeader));


    this->addTopLevelItem(new QTreeWidgetItem({"To start","please config","setting->IP control Panel"}));
    this->addTopLevelItem(new QTreeWidgetItem({"Change Path","please config","setting->DownloadPath Setting"}));
    this->addTopLevelItem(new QTreeWidgetItem({"Change Threads","please Drag","MaxThread"}));

    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    this->setAcceptDrops(true);
    this->setDragEnabled(true); //控件拖拽功能开启
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
        QList<QUrl> Droplist = dropEvent->mimeData()->urls();

        //Example File url: file:///D:/cpp/app/HTTP-UI/HTTP-QT/connect.h
        qDebug("dropEvent FileUrl:%s",dropEvent->mimeData()->text().toStdString().c_str());

        emit Upload(Droplist);

    }

    else{
        dropEvent->ignore();
    }

}

void FileList::resizeEvent(QResizeEvent *resizeEvent){

    //DOM更新
    this->setStyleSheet("QHeaderView::section{background:#A3C99FFF;}");

    qDebug("frame_FileList Height:%d",this->geometry().size().height());
    qDebug("frame_FileList Width:%d",this->geometry().size().width());


}
