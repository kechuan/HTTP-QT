#ifndef IP_CONTROLPANEL_H
#define IP_CONTROLPANEL_H

#include <QWidget>

#include "ui_mainwindow.h"

namespace Ui {
class IP_controlPanel;
}

class IP_controlPanel : public QWidget
{
    Q_OBJECT

public:
    IP_controlPanel(QWidget *parent = nullptr, Ui::MainWindow *m_ui = nullptr); //指定默认值 缺省表述nullptr

    //至于默认值 第一个arg代表着显示窗口 一般指代parent 即父类窗口 当然也可以nullptr以创建新ui
    //那么第二个呢? 则是指代需要获取ui信息的window
    ~IP_controlPanel();

    void keyPressEvent(QKeyEvent *event); //keyboard Event

private:
    Ui::IP_controlPanel *ui;
    Ui::MainWindow *m_ui;

private slots:
    void action_pressed();

signals:
    void connetPressed();

};


#endif // IP_CONTROLPANEL_H
