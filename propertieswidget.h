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
    void resizeEvent(QResizeEvent *resizeEvent);

    void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
    void dropEvent(QDropEvent* dropEvent);
    void dragMoveEvent(QDragMoveEvent *dragMoveEvent);

    void deletePrompt(QList<QTreeWidgetItem*> selectedTreeWidgetItems);

    Ui::PropertiesWidget *ui;

private:
    Ui::MainWindow *m_ui;


signals:
    void Download(QTreeWidgetItem *DropItem);
    void TaskPaused(QTreeWidgetItem *TaskItem);
    void TaskContinue(QTreeWidgetItem *TaskItem);
    void TaskCancel(QTreeWidgetItem *TaskItem);
    void TaskSpeedLimit(QTreeWidgetItem *TaskItem,int &BufferLength,char &millonSeconds);
    void TaskLimitSpeedCancel(QTreeWidgetItem *TaskItem);

public slots:
   void clearStatusList();

   void ActionPressed();

   void StatusChanged(int Status,QTreeWidgetItem* listItem);

   void ProgressCreate(QTreeWidgetItem *Item);
   void ProgressUpdate(const QString& itemName,const float& Progress,const QString& itemSpeed);
   
   bool TaskList_Menu(QTreeWidgetItem *listItem);

   void OpenFile(QTreeWidgetItem* listItem);
   void OpenFileFromFolder(QTreeWidgetItem* listItem);

signals:
   void DropFile();

};

#endif
