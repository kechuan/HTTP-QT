#include "FileList.h"

#include <QDebug>
#include <QDrag>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QMimeData>
#include <QWidget>
#include <QHeaderView>

extern QList<QTreeWidgetItem*> selectedFileList;

QPoint dragStartPosition;

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
    this->setDropIndicatorShown(true); //拖拽到这里时 鼠标指针变为可拖拽指示

    qDebug("FileList created");
}

FileList::~FileList(){
    qDebug("FileList deleted");
}

void FileList::mouseMoveEvent(QMouseEvent *mouseMoveEvent){

    selectedFileList = this->selectedItems();

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setData("application/x-qwidget",QByteArray::number((quintptr)this)); //将this指针转化为quintptr 挂上x-qwidget标签

    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap("./images/Icon.ico"));

    //热点基本上定义了拖动操作正在进行时拖动的像素图应位于的位置(固定)
    //    drag->setHotSpot(mouseMoveEvent->pos());

    drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

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


    //强制处理CSS样式
    //this->style()->unpolish(this);
    //this->style()->polish(this);
    //this->update();

    qDebug("frame_FileList Height:%d",this->geometry().size().height());
    qDebug("frame_FileList Width:%d",this->geometry().size().width());


}
