#include "speedLimitPanel.h"
#include "ui_speedLimitPanel.h"

enum presetRank{
    Low,
    Medium,
    High
};

speedLimitPanel::speedLimitPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::speedLimitPanel)
{
    ui->setupUi(this);

    QSlider *horizontalSlider_presetSlider = ui->horizontalSlider_presetSlider;
    QPushButton *pushButton_collapse = ui->pushButton_collapse;

    QLineEdit *lineEdit_BufferLength = ui->lineEdit_BufferLength;
    QComboBox *comboBox_delayms = ui->comboBox_delayms;

    lineEdit_BufferLength->setMinimumWidth(comboBox_delayms->width());

    QDialogButtonBox *buttonBox = ui->buttonBox;

    horizontalSlider_presetSlider->setValue(0);
    horizontalSlider_presetSlider->setMaximum(2);

    QObject::connect(horizontalSlider_presetSlider,&QSlider::sliderMoved,this,[=](int position){
        switch(position){
            case Low:{
                lineEdit_BufferLength->setText("3800");
                comboBox_delayms->setCurrentText("1");
                break;

            }

            case Medium:{
                lineEdit_BufferLength->setText("3600");
                comboBox_delayms->setCurrentText("1");
                break;
            }

            case High:{
                lineEdit_BufferLength->setText("3072");
                comboBox_delayms->setCurrentText("2");
                break;
            }
        }
    });

    QObject::connect(buttonBox,&QDialogButtonBox::accepted,this,[=]{
        if(lineEdit_BufferLength->text()==""){
            qDebug("BufferLength:4096");
            BufferLength = 4096;
        }

        else{
            qDebug("BufferLength:%s",lineEdit_BufferLength->text().toStdString().c_str());
            BufferLength = lineEdit_BufferLength->text().toInt();
        }


        qDebug("delayms:%s",comboBox_delayms->currentText().toStdString().c_str());
        delayms = comboBox_delayms->currentText().toInt();

    });



}

speedLimitPanel::~speedLimitPanel()
{
    delete ui;
}
