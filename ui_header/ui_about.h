/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *AboutName;
    QVBoxLayout *verticalLayout_2;
    QLabel *AboutVersion;
    QVBoxLayout *verticalLayout_3;
    QLabel *AboutTime;

    void setupUi(QWidget *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(411, 168);
        verticalLayout = new QVBoxLayout(About);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        AboutName = new QLabel(About);
        AboutName->setObjectName(QString::fromUtf8("AboutName"));
        AboutName->setEnabled(true);
        AboutName->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(AboutName);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        AboutVersion = new QLabel(About);
        AboutVersion->setObjectName(QString::fromUtf8("AboutVersion"));
        AboutVersion->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(AboutVersion);


        verticalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        AboutTime = new QLabel(About);
        AboutTime->setObjectName(QString::fromUtf8("AboutTime"));
        AboutTime->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(AboutTime);


        verticalLayout->addLayout(verticalLayout_3);


        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QWidget *About)
    {
        About->setWindowTitle(QCoreApplication::translate("About", "Form", nullptr));
        AboutName->setText(QCoreApplication::translate("About", "Test Window\342\200\251", nullptr));
        AboutVersion->setText(QCoreApplication::translate("About", "version:-2", nullptr));
        AboutTime->setText(QCoreApplication::translate("About", "Build Time:23.3.15", nullptr));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
