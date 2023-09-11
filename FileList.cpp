#include "FileList.h"
#include "ip_controlpanel.h"

#include "./dependences/enumIndex.h"

#include <QDebug>
#include <QDrag>
#include <QDropEvent>
#include <QDragLeaveEvent>
#include <QMimeData>
#include <QWidget>
#include <QHeaderView>


extern QList<QTreeWidgetItem*> selectedFileList;
extern QString parentPath;

extern IP_controlPanel *IP_controlPanelWindow;
extern int AccessLevel;

QPoint dragStartPosition;

FileList::FileList(QWidget *ParentWidget):QTreeWidget(ParentWidget){

    QObject::connect(IP_controlPanelWindow,&IP_controlPanel::connetPressed,this,[=]{
        this->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        this->header()->setSectionResizeMode(0,QHeaderView::Fixed); //这条细则可以免受上述的自适应长度影响
        this->setColumnWidth(0,60);

    });


    QList<QString> InitalHeader{"Icon","Filename","Size"};
    this->setHeaderItem(new QTreeWidgetItem(InitalHeader));


    this->addTopLevelItem(new QTreeWidgetItem({"To start","please config","setting->IP control Panel"}));
    this->addTopLevelItem(new QTreeWidgetItem({"Change Path","please config","setting->storagePath Setting"}));
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


void FileList::renderIcon(char &IconIndex){
    auto RenderIcon = [=](const char *svgAdress){
        this->topLevelItem(this->topLevelItemCount()-1)->setData(iconList, Qt::DecorationRole,QIcon(svgAdress).pixmap(QSize(26,26)));
    };

    switch(IconIndex){
        case Dir: RenderIcon(R"(:/svgPack/TypePack/icon-Dir.svg)"); break;
        case Text: RenderIcon(R"(:/svgPack/TypePack/icon-Text.svg)"); break;
        case Image: RenderIcon(R"(:/svgPack/TypePack/icon-Image.svg)"); break;
        case Video: RenderIcon(R"(:/svgPack/TypePack/icon-Video.svg)"); break;
        case Music: RenderIcon(R"(:/svgPack/TypePack/icon-Music.svg)"); break;
        case Compress: RenderIcon(R"(:/svgPack/TypePack/icon-Compress.svg)"); break;
        case Code: RenderIcon(R"(:/svgPack/TypePack/icon-Code.svg)"); break;
        case UnknownFile: RenderIcon(R"(:/svgPack/TypePack/icon-Unknown.svg)"); break;
    }
}

void FileList::mouseMoveEvent(QMouseEvent *mouseMoveEvent){

    selectedFileList = this->selectedItems();

    emit DragingFile();

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    mimeData->setData("application/x-qwidget",QByteArray::number((quintptr)this)); //将this指针转化为quintptr 挂上x-qwidget标签

    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap(":/icoPack/title.ico"));

    //热点基本上定义了拖动操作正在进行时拖动的像素图应位于的位置(固定)
    //    drag->setHotSpot(mouseMoveEvent->pos());

    int dragResult = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
    //这里exec插入枚举的意思是 执行drag时 允许的操作 即为 默认允许 所drag的widget的复制

    //而 | 之后 则是按下ctrl键进入多选的模式

    switch(dragResult){
        case Qt::IgnoreAction: emit DropStop(); break;
    }

}

void FileList::dragMoveEvent(QDragMoveEvent *dragMoveEvent){
    dragMoveEvent->acceptProposedAction();
}

void FileList::dragEnterEvent(QDragEnterEvent *dragEnterEvent){

    if(parentPath.isEmpty()) dragEnterEvent->ignore(); //未获取目录信息前直接抛出)

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
