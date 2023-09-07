#ifndef SPEEDLIMITPANEL_H
#define SPEEDLIMITPANEL_H

#include <QDialog>

namespace Ui {
class speedLimitPanel;
}

class speedLimitPanel : public QDialog
{
    Q_OBJECT

public:
    explicit speedLimitPanel(QWidget *parent = nullptr);
    ~speedLimitPanel();

    int BufferLength;
    char delayms;


private:
    Ui::speedLimitPanel *ui;

};

#endif // SPEEDLIMITPANEL_H
