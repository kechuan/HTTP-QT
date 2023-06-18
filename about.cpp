#include <QDebug>
#include "about.h"
#include "ui_about.h"

#include <QDesktopServices>


About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    qDebug() << "about created.";

    QLabel *label_httplib = ui->label_httplib;

    label_httplib->setTextFormat(Qt::RichText);
    label_httplib->setText("<a href='https://github.com/yhirose/cpp-httplib'>cpp-httplib</a>");

    label_httplib->setOpenExternalLinks(true);

    QObject::connect(label_httplib,&QLabel::linkActivated,this,[=](const QString &link){
        QDesktopServices::openUrl(link);
    });
}

About::~About()
{
    delete ui;
    qDebug() << "about deleted."; //基本不会触发 因为进行的只是hide/show
}
