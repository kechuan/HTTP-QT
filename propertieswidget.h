#ifndef PROPERTIESWIDGET_H
#define PROPERTIESWIDGET_H

#include <QWidget>

#include "ui_mainwindow.h"

namespace Ui {
class PropertiesWidget;
}

class PropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertiesWidget(QWidget *parent = nullptr, Ui::MainWindow *m_ui = nullptr);
    ~PropertiesWidget();

    void keyPressEvent(QKeyEvent *event);

    Ui::PropertiesWidget *ui;

private:
    
    Ui::MainWindow *m_ui;

public slots:
   void clearStatusList();

   //ActionTrigged
   void ActionPressed();

   //StatusChanged
   void StatusChanged(int Status,QTreeWidgetItem* listItem);

   //Downloading&Uploading
   
   void ProgressUpdate(const QString& itemName,const QString& itemSize,QString& itemLink,float& Progress);

   bool TaskList_Menu(QTreeWidgetItem *listItem, int column);

   void OpenFile(QTreeWidgetItem* listItem,int colmun);
   void OpenFileFromFolder(QTreeWidgetItem* listItem,int colmun);


};



#endif // PROPERTIESWIDGET_H
