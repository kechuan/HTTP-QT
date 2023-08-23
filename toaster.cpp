#include "toaster.h"
#include "ui_toaster.h"

#include "connect.h"
#include "FileList.h"

#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

extern Connect *Client1;
extern FileList *SurfingFile;

Toaster::Toaster(QWidget *parent,Ui::MainWindow *m_ui) :
    QWidget(parent),
    ui(new Ui::Toaster),
    m_ui(m_ui)
{
    ui->setupUi(this);

    /* Toaster Area */

    QWidget *Toaster = this;
    Toaster->hide();

    QObject::connect(Client1,&Connect::ToasterShow,this,&Toaster::DownloadToaster);

}

Toaster::Toaster(QWidget *parent,Ui::PropertiesWidget *prop_ui) :
    QWidget(parent),
    ui(new Ui::Toaster),
    prop_ui(prop_ui)
{
    ui->setupUi(this);

    /* Toaster Area */

    QWidget *Toaster = this;
    Toaster->hide();

//    QObject::connect(Client1,&Connect::ToasterShow,this,&Toaster::DownloadToaster);

    QObject::connect(SurfingFile,&FileList::DragingFile,this,[=]{
        qDebug("FileList Toaster Test");
        DragingToaster();
    });
}

void Toaster::DownloadToaster(const QString& FileName){
    QWidget *Toaster = this;

    QPushButton *pushButton_navBar = ui->pushButton_navBar;
    pushButton_navBar->setStyleSheet(
        "background-color:#118CED;"
        "border-radius:15px;"
        "color:white;"
        "font:16px;"
    );

    QPropertyAnimation *ToasterShow = new QPropertyAnimation(Toaster,"pos");

    QGraphicsOpacityEffect *ToasterOpacity = new QGraphicsOpacityEffect(Toaster); //Add opacity prop to uiWidget
    QPropertyAnimation *FadingAnimation = new QPropertyAnimation(ToasterOpacity,"opacity",this);    //QPropertyAnimation get opacity prop in SliderOpacity
    QPropertyAnimation *ShowingAnimation = new QPropertyAnimation(ToasterOpacity,"opacity",this);    //QPropertyAnimation get opacity prop in SliderOpacity

    ToasterOpacity->setOpacity(1.0);                                                             //give value
    Toaster->setGraphicsEffect(ToasterOpacity);

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

    if(Toaster->isHidden()){
        ToasterShow->setStartValue(Toaster->mapToParent(QPoint(static_cast<int>(m_ui->toolWidget->width()*0.06),static_cast<int>(m_ui->tabWidget_contentShow->height()-40))));
        ToasterShow->setEndValue(Toaster->mapToParent(QPoint(static_cast<int>(m_ui->toolWidget->width()*0.06),static_cast<int>(m_ui->tabWidget_contentShow->height()-40))));

        qDebug("toolWidget width:%d, toolWidget height:%d",m_ui->toolWidget->width(),m_ui->toolWidget->height());

        pushButton_navBar->setText(FileName+" finshed.");
        Toaster->show();
        ToasterAnimation->start();
        ShowingAnimation->start();
        Toaster->raise();

        QObject::connect(ToasterAnimation,&QPropertyAnimation::finished,this,[=]{
            FadingAnimation->start();
            QObject::connect(FadingAnimation,&QPropertyAnimation::finished,this,[=]{
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
        //ToasterShow->setStartValue(QPoint(static_cast<int>(prop_ui->treeWidgetTaskQueue->width()/10),static_cast<int>(prop_ui->treeWidgetTaskQueue->height()/5)));
        //ToasterShow->setEndValue(QPoint(static_cast<int>(prop_ui->treeWidgetTaskQueue->width()/10),static_cast<int>(prop_ui->treeWidgetTaskQueue->height()/5)));

        ToasterShow->setStartValue(QPoint(static_cast<int>(prop_ui->treeWidgetTaskQueue->width()/9),static_cast<int>(prop_ui->treeWidgetTaskQueue->height()/3)));
        ToasterShow->setEndValue(QPoint(static_cast<int>(prop_ui->treeWidgetTaskQueue->width()/9),static_cast<int>(prop_ui->treeWidgetTaskQueue->height()/3)));


        pushButton_navBar->setText("drag file(s) here...");
        qDebug("height width:%d,prop height:%d",prop_ui->treeWidgetTaskQueue->width(),prop_ui->treeWidgetTaskQueue->height());

        DragToaster->show();
        ToasterAnimation->start();
        ShowingAnimation->start();
        DragToaster->raise();

        QObject::connect(ToasterAnimation,&QPropertyAnimation::finished,this,[=]{
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
