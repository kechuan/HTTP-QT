#include <QDebug>
#include "about.h"
#include "ui_about.h"


About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    qDebug() << "about created.";
}

About::~About()
{
    delete ui;
    qDebug() << "about deleted."; //基本不会触发 因为进行的只是hide/show
}
