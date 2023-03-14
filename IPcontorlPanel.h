#ifndef IPCONTORLPANEL_H
#define IPCONTORLPANEL_H

#include <QWidget>

namespace Ui {
class IP_contorlPanel;
}

class IP_contorlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit IP_contorlPanel(QWidget *parent = nullptr);
    ~IP_contorlPanel();

private:
    Ui::IP_contorlPanel *ui;
};

#endif
