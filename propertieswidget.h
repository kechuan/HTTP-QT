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

    

private:
    Ui::PropertiesWidget *ui;
    Ui::MainWindow *m_ui;

public slots:
   void clearStatusList();
   void AddTaskQueue();
};



#endif // PROPERTIESWIDGET_H
