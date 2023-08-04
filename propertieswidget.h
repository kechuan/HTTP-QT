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
//    void resizeEvent(QResizeEvent *resizeEvent);

    void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
    void dropEvent(QDropEvent* dropEvent);
    void dragMoveEvent(QDragMoveEvent *dragMoveEvent);

    void deletePrompt(QList<QTreeWidgetItem*> selectedTreeWidgetItems);

    Ui::PropertiesWidget *ui;

private:
    Ui::MainWindow *m_ui;

signals:
    void Download(QTreeWidgetItem *DropItem,int colmun);


public slots:
   void clearStatusList();

   //ActionTrigged
   void ActionPressed();

   //StatusChanged
   void StatusChanged(int Status,QTreeWidgetItem* listItem);

   //Downloading&Uploading

   void ProgressCreate(QTreeWidgetItem *Item);
   // void ProgressUpdate(const QString& itemName,const QString& itemSize,const QString& itemLink,const float& Progress);
   // void ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSize,const QString& itemSpeed,const QString& itemLink);
   void ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSpeed);
   

   bool TaskList_Menu(QTreeWidgetItem *listItem, int column);

   void OpenFile(QTreeWidgetItem* listItem,int colmun);
   void OpenFileFromFolder(QTreeWidgetItem* listItem,int colmun);


};



#endif // PROPERTIESWIDGET_H
