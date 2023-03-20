#include "ip_controlpanel.h"
#include "ui_ip_controlpanel.h"


#include "connect.h"
#include "./dependences/HTMLFliter.h"

extern QString FullIP;
extern int Port;

extern QString SurfingPath;
extern QString ParentPath;

extern std::vector<std::string> LinkVector;
extern std::vector<std::string> PathVector;
extern std::vector<std::string> NameVector;
extern std::vector<std::string> SizeVector;

extern Connect Client1; //共同使用Client1的数据

IP_controlPanel::IP_controlPanel(QWidget *parent,Ui::MainWindow *extern_ui):
    QWidget(parent),
    ui(new Ui::IP_controlPanel),
    extern_ui(extern_ui)
{
    ui->setupUi(this);

    this->setWindowTitle("IP控制台");

    QObject::connect(ui->pushButton_Connect,SIGNAL(released()),this,SLOT(action_pressed()));
    QObject::connect(ui->pushButton_Abort,SIGNAL(released()),this,SLOT(action_pressed()));


}



IP_controlPanel::~IP_controlPanel()
{
    delete ui;
}


void IP_controlPanel::action_pressed(){
   qDebug() << "action IP cotrolPanel Trigger";

   QTextBrowser *log_view = extern_ui->textBrowser_log;
   QPushButton *button = (QPushButton*)sender();
   QString buttonName = button->text();


        if(buttonName == "Connect"){
            qDebug() << "Connect trigger";

            //通过子成员函数arg的QString链接字符串快捷用法 注意 arg本身仅支持右值传入
            FullIP = QString("%1.%2.%3.%4").arg(ui->lineEdit_IP_1->text(),ui->lineEdit_IP_2->text(),ui->lineEdit_IP_3->text(),ui->lineEdit_IP_4->text());
            Port = ui->lineEdit_Port->text().toInt();

            log_view->append(R"(<span style=" color:#ffffff;">)"+buttonName+FullIP+":"+ui->lineEdit_Port->text()+R"(</span>)");

            if(Client1.cliPing(FullIP,Port)){
                std::string Information = Client1.cliFileSurfing(FullIP,Port);
                HTMLExtract(Information,LinkVector,NameVector);


                extern_ui->Filelist->clear();

                //codeway add TreeItem

                for(int index = 0;index<=NameVector.size()-1;index++){
                    QList<QString> newItemInformation{"-",NameVector.at(index).c_str(),"—",LinkVector.at(index).c_str()};
                    QTreeWidgetItem *newItem = new QTreeWidgetItem(newItemInformation);
                    extern_ui->Filelist->addTopLevelItem(newItem);
                }

                ui->pushButton_Connect->setEnabled(false);
                QWidget::close(); //原来如此。。 


            }

            else{
                //链接超时处理
                qDebug()<<"ping failed.";
            }
        }


        else{
            qDebug()<<"connect Abort.";
            ui->pushButton_Connect->setEnabled(true);

            //这里应该放一下http库里的强制终止任何现在的行为 下载/上传 以及请求网页的操作 等等等等

            extern_ui->Filelist->clear();
            QList<QString> newItemInformation{"To start","please config","setting->IP control Panel",""};
            extern_ui->Filelist->addTopLevelItem(new QTreeWidgetItem(newItemInformation));


        }


    
}


