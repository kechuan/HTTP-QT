#include "ip_controlpanel.h"
#include <QKeyEvent>

#include "ui_ip_controlpanel.h"

#include "./dependences/HTMLFliter.h"

#include "connect.h"
#include "FileList.h"

extern QString FullIP;
extern int Port;

extern QString SurfingPath;

extern std::vector<std::string> LinkVector;
extern std::vector<std::string> PathVector;
extern std::vector<std::string> NameVector;
extern std::vector<std::string> SizeVector;

extern FileList *SurfingFile;
extern Connect *Client1;
extern int AccessLevel;

bool connectedFlag = false;

enum AccessType{
    Guest,
    User,
    Admin
};



IP_controlPanel::IP_controlPanel(QWidget *parent,Ui::MainWindow *m_ui):
    QWidget(parent),
    ui(new Ui::IP_controlPanel),
    m_ui(m_ui)
{
    ui->setupUi(this);

    this->setWindowTitle("IP控制台");
    this->setWindowFlag(Qt::WindowMaximizeButtonHint,false); //禁用winForm上的最大化按钮

    QObject::connect(ui->pushButton_Connect,SIGNAL(released()),this,SLOT(action_pressed()));
    QObject::connect(ui->pushButton_Abort,SIGNAL(released()),this,SLOT(action_pressed()));

    QPushButton *pushButton_collapse = ui->pushButton_collapse;

    pushButton_collapse->setStyleSheet(R"(
        QPushButton{
            border:none;
            background:none
        }

        QPushButton:pressed{
            border: none;
            background: none;
        }
    )");

    //根据qrc文件下的描述确立路径 :/svgPack/ArrowPack/...
    QIcon rightArrow = QIcon(":/svgPack/ArrowPack/icon-rightArrow.svg");
    pushButton_collapse->setIcon(rightArrow);


    QObject::connect(pushButton_collapse,&QPushButton::clicked,this,[=]{
        if (ui->label_username->isHidden()) {

            ui->label_username->setVisible(true);
            ui->label_password->setVisible(true);

            ui->lineEdit_username->setVisible(true);
            ui->lineEdit_password->setVisible(true);

            QIcon downArrow = QIcon(":/svgPack/ArrowPack/icon-downArrow.svg");
            pushButton_collapse->setIcon(downArrow);

        }

        else {
            ui->label_username->setVisible(false);
            ui->label_password->setVisible(false);

            ui->lineEdit_username->setVisible(false);
            ui->lineEdit_password->setVisible(false);

            QIcon rightArrow = QIcon(":/svgPack/ArrowPack/icon-rightArrow.svg");
            pushButton_collapse->setIcon(rightArrow);
        }
    });

    ui->label_username->setVisible(false);
    ui->label_password->setVisible(false);

    ui->lineEdit_username->setVisible(false);
    ui->lineEdit_password->setVisible(false);
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);

    QObject::connect(ui->lineEdit_IP_1,&QLineEdit::textChanged,this,[this]{
        if(ui->lineEdit_IP_1->text().toInt()>255){
            ui->lineEdit_IP_1->setText("255");
        }
    });

    QObject::connect(ui->lineEdit_IP_2,&QLineEdit::textChanged,this,[this]{
        if(ui->lineEdit_IP_2->text().toInt()>255){
            ui->lineEdit_IP_2->setText("255");
        }
    });

    QObject::connect(ui->lineEdit_IP_3,&QLineEdit::textChanged,this,[this]{
        if(ui->lineEdit_IP_3->text().toInt()>255){
            ui->lineEdit_IP_3->setText("255");
        }
    });

    QObject::connect(ui->lineEdit_IP_4,&QLineEdit::textChanged,this,[this]{
        if(ui->lineEdit_IP_4->text().toInt()>255){
            ui->lineEdit_IP_4->setText("255");
        }
    });

    QRegularExpression LimitInputNumber(R"(\d{1,4}$)");

    QValidator *validator = new QRegularExpressionValidator(LimitInputNumber);

    ui->lineEdit_IP_1->setValidator(validator);
    ui->lineEdit_IP_2->setValidator(validator);
    ui->lineEdit_IP_3->setValidator(validator);
    ui->lineEdit_IP_4->setValidator(validator);

    qDebug() << "IP_controlPanel created.";

}


IP_controlPanel::~IP_controlPanel()
{
    delete ui;
}


void IP_controlPanel::action_pressed(){
   qDebug() << "action IP cotrolPanel Trigger";

   QTextBrowser *log_view = m_ui->textBrowser_log;
   QPushButton *button = (QPushButton*)sender();
   QString buttonName = button->text();

        if(buttonName == "Connect"){
            qDebug() << "Connect trigger";

            //通过子成员函数arg的QString链接字符串快捷用法 注意 arg本身仅支持右值传入
            FullIP = QString("%1.%2.%3.%4").arg(ui->lineEdit_IP_1->text(),ui->lineEdit_IP_2->text(),ui->lineEdit_IP_3->text(),ui->lineEdit_IP_4->text());
            Port = ui->lineEdit_Port->text().toInt();

            log_view->append(R"(<span style=" color:#ffffff;"> Trying)"+buttonName+" "+FullIP+":"+ui->lineEdit_Port->text()+R"(</span>)");

            if(Client1->cliPing()){
                log_view->append(R"(<span style=" color:#ffffff;">Ping Succ</span>)");


                //Login Mode
                if(!ui->lineEdit_username->text().isEmpty()){
                    qDebug("username:%s",ui->lineEdit_username->text().toStdString().c_str());
                    qDebug("password:%s",ui->lineEdit_password->text().toStdString().c_str());

                    QString username = ui->lineEdit_username->text();
                    QString password = ui->lineEdit_password->text();

                    Client1->cliLogin(username,password);

                        //鉴权步骤(选择)
                        //1.账号/密码错误是否以游客身份访问
                        if(AccessLevel==Guest){
                            int choice = QMessageBox::question(this,tr("Authentification Fault"),tr("Account or Password wrong.\nWanna insert Information again(Y) or Accessing Server with Guest Mode(N)."),QMessageBox::Yes|QMessageBox::No);

                        //2.重输入->失败返回1 建议在后端服务器加上账号保护验证.不过也只能保护账号 不能防恶意输入
                        //不然算法的引入 我怕体量一下涨一堆
                            switch(choice){
                                case QMessageBox::Yes: {
                                    ui->lineEdit_password->clear();
                                    log_view->append(R"(<span style=" color:#ffffff;">Login Failed.</span>)");
                                    return;
                                }
                                case QMessageBox::No: {break;}
                            }

                        }

                }



                connectedFlag = true;
                std::string Information = Client1->cliFileSurfing();
                HTMLExtract(Information,LinkVector,NameVector);

                SurfingFile->clear();
                emit connetPressed();

                for(int index = 0;index<=NameVector.size()-1;index++){
                    QList<QString> newItemInformation{"Disk",NameVector.at(index).c_str(),"—",LinkVector.at(index).c_str()};
                    QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
                    SurfingFile->addTopLevelItem(newItem);
                }

                ui->pushButton_Connect->setEnabled(false);
                QWidget::close(); //原来如此 widget是有直接close的功能的 应该等效于点X关闭。。

            }

            else{
                QMessageBox::information(this,tr("ping error"),tr("链接超时 请检查是否已打开服务器"));
            }
        }


        else{
            qDebug()<<"connect Abort.";
            ui->pushButton_Connect->setEnabled(true);

            connectedFlag = false;

            SurfingFile->clear();
            QList<QString> newItemInformation{"To start","please config","setting->IP control Panel",""};
            SurfingFile->addTopLevelItem(new QTreeWidgetItem(newItemInformation));


        }

}


//快捷键定义区
void IP_controlPanel::keyPressEvent(QKeyEvent *event){
    switch(event->key()){
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            emit ui->pushButton_Connect->released();
            qDebug("IP_controlPanel return pressed");
            break;
        }
    }

}


