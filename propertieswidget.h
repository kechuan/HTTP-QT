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

    Ui::PropertiesWidget *ui;

private:
    
    Ui::MainWindow *m_ui;

public slots:
   void clearStatusList();
   void AddTaskQueue(const char* Status,QTreeWidgetItem* listItem);
//    void AddTaskQueue2(int Status,QTreeWidgetItem* listItem);

   //StatusChanged
   void StatusChanged(int Status,QTreeWidgetItem* listItem);

   //Downloading&Uploading
   void Progresstest(std::map<std::string,int> &TaskMap,std::string& Name,int& Progress);
   void ProgressUpdate(int& Progress,QTreeWidgetItem* listItem);

   void testSlot(QString& itemName,float& Progress);


   bool TaskList_Menu(QTreeWidgetItem *listItem, int column);


   void ShowFromExplorer();

};



#endif // PROPERTIESWIDGET_H
