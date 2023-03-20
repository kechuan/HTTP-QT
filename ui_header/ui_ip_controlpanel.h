/********************************************************************************
** Form generated from reading UI file 'ip_controlpanel.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IP_CONTROLPANEL_H
#define UI_IP_CONTROLPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IP_controlPanel
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_IP_controlPanel;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_IP_Panel;
    QHBoxLayout *horizontalLayout_IPsetting;
    QLabel *label_IP_Address;
    QLineEdit *lineEdit_IP_1;
    QLabel *label_Dot;
    QLineEdit *lineEdit_IP_2;
    QLabel *label_Dot_2;
    QLineEdit *lineEdit_IP_3;
    QLabel *label_Dot_3;
    QLineEdit *lineEdit_IP_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_IP_Port;
    QLineEdit *lineEdit_Port;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *groupBox_passportConfig;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_username;
    QLineEdit *lineEdit_username;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_password;
    QLineEdit *lineEdit_password;
    QHBoxLayout *horizontalLayout_connectionPanel;
    QPushButton *pushButton_Connect;
    QPushButton *pushButton_Abort;

    void setupUi(QWidget *IP_controlPanel)
    {
        if (IP_controlPanel->objectName().isEmpty())
            IP_controlPanel->setObjectName(QString::fromUtf8("IP_controlPanel"));
        IP_controlPanel->resize(400, 300);
        verticalLayout = new QVBoxLayout(IP_controlPanel);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_IP_controlPanel = new QGroupBox(IP_controlPanel);
        groupBox_IP_controlPanel->setObjectName(QString::fromUtf8("groupBox_IP_controlPanel"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_IP_controlPanel->sizePolicy().hasHeightForWidth());
        groupBox_IP_controlPanel->setSizePolicy(sizePolicy);
        groupBox_IP_controlPanel->setMinimumSize(QSize(341, 156));
        groupBox_IP_controlPanel->setLayoutDirection(Qt::LeftToRight);
        groupBox_IP_controlPanel->setFlat(false);
        groupBox_IP_controlPanel->setCheckable(false);
        verticalLayout_4 = new QVBoxLayout(groupBox_IP_controlPanel);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_IP_Panel = new QVBoxLayout();
        verticalLayout_IP_Panel->setObjectName(QString::fromUtf8("verticalLayout_IP_Panel"));
        horizontalLayout_IPsetting = new QHBoxLayout();
        horizontalLayout_IPsetting->setObjectName(QString::fromUtf8("horizontalLayout_IPsetting"));
        label_IP_Address = new QLabel(groupBox_IP_controlPanel);
        label_IP_Address->setObjectName(QString::fromUtf8("label_IP_Address"));

        horizontalLayout_IPsetting->addWidget(label_IP_Address);

        lineEdit_IP_1 = new QLineEdit(groupBox_IP_controlPanel);
        lineEdit_IP_1->setObjectName(QString::fromUtf8("lineEdit_IP_1"));
        lineEdit_IP_1->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_IPsetting->addWidget(lineEdit_IP_1);

        label_Dot = new QLabel(groupBox_IP_controlPanel);
        label_Dot->setObjectName(QString::fromUtf8("label_Dot"));

        horizontalLayout_IPsetting->addWidget(label_Dot);

        lineEdit_IP_2 = new QLineEdit(groupBox_IP_controlPanel);
        lineEdit_IP_2->setObjectName(QString::fromUtf8("lineEdit_IP_2"));
        lineEdit_IP_2->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_IPsetting->addWidget(lineEdit_IP_2);

        label_Dot_2 = new QLabel(groupBox_IP_controlPanel);
        label_Dot_2->setObjectName(QString::fromUtf8("label_Dot_2"));

        horizontalLayout_IPsetting->addWidget(label_Dot_2);

        lineEdit_IP_3 = new QLineEdit(groupBox_IP_controlPanel);
        lineEdit_IP_3->setObjectName(QString::fromUtf8("lineEdit_IP_3"));
        lineEdit_IP_3->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_IPsetting->addWidget(lineEdit_IP_3);

        label_Dot_3 = new QLabel(groupBox_IP_controlPanel);
        label_Dot_3->setObjectName(QString::fromUtf8("label_Dot_3"));

        horizontalLayout_IPsetting->addWidget(label_Dot_3);

        lineEdit_IP_4 = new QLineEdit(groupBox_IP_controlPanel);
        lineEdit_IP_4->setObjectName(QString::fromUtf8("lineEdit_IP_4"));
        lineEdit_IP_4->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_IPsetting->addWidget(lineEdit_IP_4);


        verticalLayout_IP_Panel->addLayout(horizontalLayout_IPsetting);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_IP_Port = new QLabel(groupBox_IP_controlPanel);
        label_IP_Port->setObjectName(QString::fromUtf8("label_IP_Port"));

        horizontalLayout_2->addWidget(label_IP_Port);

        lineEdit_Port = new QLineEdit(groupBox_IP_controlPanel);
        lineEdit_Port->setObjectName(QString::fromUtf8("lineEdit_Port"));
        lineEdit_Port->setMaximumSize(QSize(44, 16777215));
        lineEdit_Port->setMaxLength(32767);
        lineEdit_Port->setReadOnly(false);

        horizontalLayout_2->addWidget(lineEdit_Port);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout_IP_Panel->addLayout(horizontalLayout_2);

        groupBox_passportConfig = new QGroupBox(groupBox_IP_controlPanel);
        groupBox_passportConfig->setObjectName(QString::fromUtf8("groupBox_passportConfig"));
        verticalLayout_3 = new QVBoxLayout(groupBox_passportConfig);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_username = new QLabel(groupBox_passportConfig);
        label_username->setObjectName(QString::fromUtf8("label_username"));

        horizontalLayout->addWidget(label_username);

        lineEdit_username = new QLineEdit(groupBox_passportConfig);
        lineEdit_username->setObjectName(QString::fromUtf8("lineEdit_username"));

        horizontalLayout->addWidget(lineEdit_username);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_password = new QLabel(groupBox_passportConfig);
        label_password->setObjectName(QString::fromUtf8("label_password"));

        horizontalLayout_4->addWidget(label_password);

        lineEdit_password = new QLineEdit(groupBox_passportConfig);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));

        horizontalLayout_4->addWidget(lineEdit_password);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout_IP_Panel->addWidget(groupBox_passportConfig);

        horizontalLayout_connectionPanel = new QHBoxLayout();
        horizontalLayout_connectionPanel->setObjectName(QString::fromUtf8("horizontalLayout_connectionPanel"));
        pushButton_Connect = new QPushButton(groupBox_IP_controlPanel);
        pushButton_Connect->setObjectName(QString::fromUtf8("pushButton_Connect"));

        horizontalLayout_connectionPanel->addWidget(pushButton_Connect);

        pushButton_Abort = new QPushButton(groupBox_IP_controlPanel);
        pushButton_Abort->setObjectName(QString::fromUtf8("pushButton_Abort"));

        horizontalLayout_connectionPanel->addWidget(pushButton_Abort);


        verticalLayout_IP_Panel->addLayout(horizontalLayout_connectionPanel);


        verticalLayout_4->addLayout(verticalLayout_IP_Panel);


        verticalLayout->addWidget(groupBox_IP_controlPanel);


        retranslateUi(IP_controlPanel);

        QMetaObject::connectSlotsByName(IP_controlPanel);
    } // setupUi

    void retranslateUi(QWidget *IP_controlPanel)
    {
        IP_controlPanel->setWindowTitle(QCoreApplication::translate("IP_controlPanel", "Form", nullptr));
        groupBox_IP_controlPanel->setTitle(QCoreApplication::translate("IP_controlPanel", "\350\257\267\351\205\215\347\275\256\351\234\200\350\246\201\351\223\276\346\216\245\347\232\204\346\234\215\345\212\241\345\231\250IP:", nullptr));
        label_IP_Address->setText(QCoreApplication::translate("IP_controlPanel", "IP Address:", nullptr));
        lineEdit_IP_1->setText(QCoreApplication::translate("IP_controlPanel", "192", nullptr));
        label_Dot->setText(QCoreApplication::translate("IP_controlPanel", ".", nullptr));
        lineEdit_IP_2->setText(QCoreApplication::translate("IP_controlPanel", "168", nullptr));
        label_Dot_2->setText(QCoreApplication::translate("IP_controlPanel", ".", nullptr));
        lineEdit_IP_3->setText(QCoreApplication::translate("IP_controlPanel", "56", nullptr));
        label_Dot_3->setText(QCoreApplication::translate("IP_controlPanel", ".", nullptr));
        lineEdit_IP_4->setText(QCoreApplication::translate("IP_controlPanel", "1", nullptr));
        label_IP_Port->setText(QCoreApplication::translate("IP_controlPanel", "Port:", nullptr));
        lineEdit_Port->setText(QCoreApplication::translate("IP_controlPanel", "8888", nullptr));
        groupBox_passportConfig->setTitle(QCoreApplication::translate("IP_controlPanel", "\350\264\246\345\217\267/\345\257\206\347\240\201(\345\217\257\351\200\211)", nullptr));
        label_username->setText(QCoreApplication::translate("IP_controlPanel", "username:", nullptr));
        label_password->setText(QCoreApplication::translate("IP_controlPanel", "password:", nullptr));
        pushButton_Connect->setText(QCoreApplication::translate("IP_controlPanel", "Connect", nullptr));
        pushButton_Abort->setText(QCoreApplication::translate("IP_controlPanel", "Abort", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IP_controlPanel: public Ui_IP_controlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IP_CONTROLPANEL_H
