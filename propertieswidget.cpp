#include "propertieswidget.h"
#include "ui_propertieswidget.h"

extern QString FullIP;
extern int Port;

extern QString SurfingPath;
extern QString ParentPath;

extern std::vector<std::string> LinkVector;
extern std::vector<std::string> PathVector;
extern std::vector<std::string> NameVector;
extern std::vector<std::string> SizeVector;

PropertiesWidget::PropertiesWidget(QWidget *parent,Ui::MainWindow *m_ui) :
    QWidget(parent),
    ui(new Ui::PropertiesWidget),
    m_ui(m_ui)
{
    ui->setupUi(this);

    qDebug()<<"Status created";
}

PropertiesWidget::~PropertiesWidget()
{
    delete ui;
}


void PropertiesWidget::clearStatusList(){
    qDebug()<<"clearStatusList";
    ui->treeWidgetTaskQueue->clear();
}


void PropertiesWidget::AddTaskQueue(){
    QTreeWidget *treeWidgetTaskQueue = ui->treeWidgetTaskQueue;
    QList<QString> newItemInformation{"-","..","—",ParentPath};


    qDebug()<<"AddTaskQueue trigged";
    ui->treeWidgetTaskQueue->clear();
}




