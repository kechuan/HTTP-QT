#include "speedLimitPanel.h"
#include "ui_speedLimitPanel.h"

speedLimitPanel::speedLimitPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::speedLimitPanel)
{
    ui->setupUi(this);

    QComboBox *comboBox_speedLimit = ui->comboBox_speedLimit;
    QDialogButtonBox *buttonBox = ui->buttonBox;

    QObject::connect(buttonBox,&QDialogButtonBox::accepted,this,[=]{
        qDebug("speedLimit:%s",comboBox_speedLimit->currentText().toStdString().c_str());
        speedLimit = comboBox_speedLimit->currentText().toInt();
    });

}

speedLimitPanel::~speedLimitPanel()
{
    delete ui;
}
