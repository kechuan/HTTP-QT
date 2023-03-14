#include <QDebug>
#include "IP_contorlPanel.h"
#include "ui_IP_contorlPanel.h"


IP_contorlPanel::IP_contorlPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IP_contorlPanel)
{
    ui->setupUi(this);
    qDebug() << "IP_contorlPanel created.";
}

IP_contorlPanel::~IP_contorlPanel()
{
    delete ui;
    qDebug() << "IP_contorlPanel deleted."; //基本不会触发 因为进行的只是hide/show
}
