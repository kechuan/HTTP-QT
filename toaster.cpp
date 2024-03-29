#include "toaster.h"
#include "propertieswidget.h"
#include "ui_toaster.h"

#include "connect.h"
#include "FileList.h"
#include "./dependences/enumIndex.h"

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

extern Connect *Client1;
extern FileList *SurfingFile;
extern PropertiesWidget *DockWidget;

Toaster::Toaster(QWidget *parent,Ui::MainWindow *m_ui) :
    QWidget(parent),
    ui(new Ui::Toaster),
    m_ui(m_ui)
{
    ui->setupUi(this);

    QWidget *Toaster = this;
    Toaster->hide();

    QObject::connect(Client1,&Connect::ToasterAction,this,&Toaster::DownloadToaster);

}

Toaster::Toaster(QWidget *parent,Ui::PropertiesWidget *prop_ui) :
    QWidget(parent),
    ui(new Ui::Toaster),
    prop_ui(prop_ui)
{
    ui->setupUi(this);

    QWidget *DragToaster = this;
    DragToaster->hide();

    QObject::connect(SurfingFile,&FileList::DragingFile,this,&Toaster::DragingToaster);

}

void Toaster::DownloadToaster(const QString& FileName,const int& status){
    QWidget *Toaster = this;
    QPushButton *pushButton_navBar = ui->pushButton_navBar;

    QPropertyAnimation *ToasterShow = new QPropertyAnimation(Toaster,"pos");
    QSequentialAnimationGroup *ToasterAnimation = new QSequentialAnimationGroup(this);

    ToasterAnimation->addAnimation(ToasterShow);
    ToasterAnimation->addPause(2000);
    ToasterShow->setDuration(500);

    if(Toaster->isHidden()){

        qDebug("toolWidget width:%f, toolWidget height:%d",m_ui->toolWidget->width()*0.06,m_ui->toolWidget->height()-38);

        ToasterShow->setStartValue(m_ui->toolWidget->mapToParent(QPoint(m_ui->toolWidget->width()*0.23,-(m_ui->toolWidget->height()))));
        ToasterShow->setEndValue(m_ui->toolWidget->mapToParent(QPoint(m_ui->toolWidget->width()*0.23,-(m_ui->toolWidget->height())*1.2)));

        //字数显示上限

        QString ToasterFileName;
        if(FileName.size()>50){
            ToasterFileName = FileName.sliced(0,50);
            ToasterFileName.append("...");
            
            Toaster->resize(QSize(666,Toaster->height()));

            ToasterShow->setStartValue(m_ui->toolWidget->mapToParent(QPoint(m_ui->toolWidget->width()*0.15,-(m_ui->toolWidget->height()))));
            ToasterShow->setEndValue(m_ui->toolWidget->mapToParent(QPoint(m_ui->toolWidget->width()*0.15,-(m_ui->toolWidget->height())*1.2)));
        }

        else{
            ToasterFileName = FileName;
            Toaster->adjustSize();
        }

        switch(status){
            case TaskCompleted:{
                pushButton_navBar->setStyleSheet(
                    R"(
                        border-radius:15px;
                        font:18px;
                        color:black;
                        background-color:#118CED;
                    )"
                );

                pushButton_navBar->setIcon(QIcon(":/svgPack/StatusPack/icon-Completed.svg").pixmap(QSize(32,32)));
                pushButton_navBar->setText(ToasterFileName+" Finished.");
                break;
            };

            case TaskPaused:{
                pushButton_navBar->setStyleSheet(
                    R"(
                        border-radius:15px;
                        font:18px;
                        color:black;
                        background-color:#F7B507;
                    )"
                );

                pushButton_navBar->setIcon(QIcon(":/svgPack/StatusPack/icon-Pause.svg").pixmap(QSize(32,32)));
                pushButton_navBar->setText(ToasterFileName+" Paused.");
                break;
            };

            case TaskCanceled:{
                pushButton_navBar->setStyleSheet(
                    R"(
                        border-radius:15px;
                        font:18px;
                        color:black;
                        background-color:#F1270D;
                    )"
                );

                pushButton_navBar->setIcon(QIcon(":/svgPack/StatusPack/icon-Remove.svg").pixmap(QSize(32,32)));
                pushButton_navBar->setText(ToasterFileName+" Canceled.");
                break;
            };
        }

        ToasterAnimation->start();

        QGraphicsOpacityEffect *ToasterOpacity = new QGraphicsOpacityEffect(Toaster); //Add opacity prop to uiWidget
        QPropertyAnimation *FadingAnimation = new QPropertyAnimation(ToasterOpacity,"opacity",this);    //QPropertyAnimation get opacity prop in SliderOpacity
        QPropertyAnimation *ShowingAnimation = new QPropertyAnimation(ToasterOpacity,"opacity",this);    //QPropertyAnimation get opacity prop in SliderOpacity

        ToasterOpacity->setOpacity(1.0);                                                             //give value
        Toaster->setGraphicsEffect(ToasterOpacity);

        ShowingAnimation->setStartValue(0.0);
        ShowingAnimation->setEndValue(1.0);
        ShowingAnimation->setDuration(1000);


        ShowingAnimation->start();

        Toaster->show();

        QObject::connect(ToasterAnimation,&QPropertyAnimation::finished,this,[=]{
            FadingAnimation->setStartValue(1.0);
            FadingAnimation->setEndValue(0.0);
            FadingAnimation->setDuration(1000);

            FadingAnimation->start();

            QObject::connect(FadingAnimation,&QPropertyAnimation::finished,this,[=]{
                qDebug("Downloading Toaster trigged truely");
                Toaster->hide();
            });
        });
    }

}

void Toaster::DragingToaster(){
    QWidget *DragToaster = this;

    QPushButton *pushButton_navBar = ui->pushButton_navBar;
    pushButton_navBar->setStyleSheet(
        "background-color:black;"
        "border-radius:15px;"
        "color:white;"
        "font:24px;"
    );

    QPropertyAnimation *ToasterShow = new QPropertyAnimation(DragToaster,"pos");

    QGraphicsOpacityEffect *ToasterOpacity = new QGraphicsOpacityEffect(DragToaster); //Add opacity prop to uiWidget
    QPropertyAnimation *FadingAnimation = new QPropertyAnimation(ToasterOpacity,"opacity",this);    //QPropertyAnimation get opacity prop in SliderOpacity
    QPropertyAnimation *ShowingAnimation = new QPropertyAnimation(ToasterOpacity,"opacity",this);    //QPropertyAnimation get opacity prop in SliderOpacity

    ToasterOpacity->setOpacity(1.0);                                                             //give value
    DragToaster->setGraphicsEffect(ToasterOpacity);

    QSequentialAnimationGroup *ToasterAnimation = new QSequentialAnimationGroup(this);

    ToasterAnimation->addAnimation(ToasterShow);
    ToasterAnimation->addPause(2000);

    ToasterShow->setDuration(500);

    ShowingAnimation->setDuration(1000);
    ShowingAnimation->setStartValue(0.0);
    ShowingAnimation->setEndValue(1.0);
    ShowingAnimation->setEasingCurve(QEasingCurve::Linear);

    FadingAnimation->setDuration(1000);
    FadingAnimation->setStartValue(1.0);
    FadingAnimation->setEndValue(0.0);
    FadingAnimation->setEasingCurve(QEasingCurve::Linear);

    if(DragToaster->isHidden()){


        ToasterShow->setStartValue(QPoint(static_cast<int>(prop_ui->treeWidgetTaskQueue->width()/8),static_cast<int>(prop_ui->treeWidgetTaskQueue->height()/2)));
        ToasterShow->setEndValue(QPoint(static_cast<int>(prop_ui->treeWidgetTaskQueue->width()/8),static_cast<int>(prop_ui->treeWidgetTaskQueue->height()/2)));


        pushButton_navBar->setText("drag file(s) here...");
        qDebug("height width:%d,prop height:%d",prop_ui->treeWidgetTaskQueue->width(),prop_ui->treeWidgetTaskQueue->height());

        DragToaster->show();
        ToasterAnimation->start();
        ShowingAnimation->start();

        QObject::connect(SurfingFile,&FileList::DropStop,this,[=]{
            qDebug("Drop Canceled. Toaster hide");
            FadingAnimation->start();
            QObject::connect(FadingAnimation,&QPropertyAnimation::finished,this,[=]{
                DragToaster->hide();
            });
        });

        QObject::connect(DockWidget,&PropertiesWidget::DropFile,this,[=]{
            qDebug("SurfingFile Drop it. Toaster hide");
            FadingAnimation->start();
            QObject::connect(FadingAnimation,&QPropertyAnimation::finished,this,[=]{
                DragToaster->hide();
            });
        });

    }
}

Toaster::~Toaster()
{
    delete ui;
}
