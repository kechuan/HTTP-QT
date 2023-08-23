#ifndef TOASTER_H
#define TOASTER_H

#include <QWidget>

#include "ui_mainwindow.h"
#include "ui_propertieswidget.h"

namespace Ui {
class Toaster;
}

class Toaster : public QWidget
{
    Q_OBJECT

public:

    explicit Toaster(QWidget *parent = nullptr,Ui::MainWindow *m_ui = nullptr);
    explicit Toaster(QWidget *parent = nullptr,Ui::PropertiesWidget *prop_ui = nullptr);
    ~Toaster();

    void DownloadToaster(const QString& FileName);
    void DragingToaster();

private:
    Ui::Toaster *ui;
    Ui::MainWindow *m_ui;
    Ui::PropertiesWidget *prop_ui;
};

#endif // TOASTER_H
