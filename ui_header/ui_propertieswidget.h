/********************************************************************************
** Form generated from reading UI file 'propertieswidget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTIESWIDGET_H
#define UI_PROPERTIESWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PropertiesWidget
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedProperties;
    QWidget *pageTaskQueue;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_Continue;
    QPushButton *pushButton_Pause;
    QPushButton *pushButton_Remove;
    QFrame *line;
    QSpacerItem *horizontalSpacer;
    QTreeWidget *treeWidget;
    QWidget *pageObj;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayoutTextBrowser;
    QTextBrowser *textBrowserObj;

    void setupUi(QWidget *PropertiesWidget)
    {
        if (PropertiesWidget->objectName().isEmpty())
            PropertiesWidget->setObjectName(QString::fromUtf8("PropertiesWidget"));
        PropertiesWidget->resize(889, 503);
        verticalLayout = new QVBoxLayout(PropertiesWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedProperties = new QStackedWidget(PropertiesWidget);
        stackedProperties->setObjectName(QString::fromUtf8("stackedProperties"));
        pageTaskQueue = new QWidget();
        pageTaskQueue->setObjectName(QString::fromUtf8("pageTaskQueue"));
        verticalLayout_2 = new QVBoxLayout(pageTaskQueue);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_Continue = new QPushButton(pageTaskQueue);
        pushButton_Continue->setObjectName(QString::fromUtf8("pushButton_Continue"));

        horizontalLayout->addWidget(pushButton_Continue);

        pushButton_Pause = new QPushButton(pageTaskQueue);
        pushButton_Pause->setObjectName(QString::fromUtf8("pushButton_Pause"));

        horizontalLayout->addWidget(pushButton_Pause);

        pushButton_Remove = new QPushButton(pageTaskQueue);
        pushButton_Remove->setObjectName(QString::fromUtf8("pushButton_Remove"));

        horizontalLayout->addWidget(pushButton_Remove);

        line = new QFrame(pageTaskQueue);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout);

        treeWidget = new QTreeWidget(pageTaskQueue);
        new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(treeWidget);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        verticalLayout_2->addWidget(treeWidget);

        stackedProperties->addWidget(pageTaskQueue);
        pageObj = new QWidget();
        pageObj->setObjectName(QString::fromUtf8("pageObj"));
        verticalLayout_5 = new QVBoxLayout(pageObj);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayoutTextBrowser = new QVBoxLayout();
        verticalLayoutTextBrowser->setObjectName(QString::fromUtf8("verticalLayoutTextBrowser"));
        textBrowserObj = new QTextBrowser(pageObj);
        textBrowserObj->setObjectName(QString::fromUtf8("textBrowserObj"));

        verticalLayoutTextBrowser->addWidget(textBrowserObj);


        verticalLayout_5->addLayout(verticalLayoutTextBrowser);

        stackedProperties->addWidget(pageObj);

        verticalLayout->addWidget(stackedProperties);


        retranslateUi(PropertiesWidget);

        QMetaObject::connectSlotsByName(PropertiesWidget);
    } // setupUi

    void retranslateUi(QWidget *PropertiesWidget)
    {
        PropertiesWidget->setWindowTitle(QCoreApplication::translate("PropertiesWidget", "Form", nullptr));
        pushButton_Continue->setText(QCoreApplication::translate("PropertiesWidget", "Continue", nullptr));
        pushButton_Pause->setText(QCoreApplication::translate("PropertiesWidget", "Pause", nullptr));
        pushButton_Remove->setText(QCoreApplication::translate("PropertiesWidget", "Remove", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(5, QCoreApplication::translate("PropertiesWidget", "DateTime", nullptr));
        ___qtreewidgetitem->setText(4, QCoreApplication::translate("PropertiesWidget", "Speed", nullptr));
        ___qtreewidgetitem->setText(3, QCoreApplication::translate("PropertiesWidget", "Size", nullptr));
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("PropertiesWidget", "Progress", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("PropertiesWidget", "Filename", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("PropertiesWidget", "Status(hidden)", nullptr));

        const bool __sortingEnabled = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(5, QCoreApplication::translate("PropertiesWidget", "Date.now()", nullptr));
        ___qtreewidgetitem1->setText(4, QCoreApplication::translate("PropertiesWidget", "???", nullptr));
        ___qtreewidgetitem1->setText(3, QCoreApplication::translate("PropertiesWidget", "SizePath", nullptr));
        ___qtreewidgetitem1->setText(2, QCoreApplication::translate("PropertiesWidget", "bing:how to implement progressBar in QTreewidgetItem", nullptr));
        ___qtreewidgetitem1->setText(1, QCoreApplication::translate("PropertiesWidget", "\346\226\260\345\273\272\346\226\207\346\234\254\346\226\207\346\241\243.txt", nullptr));
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("PropertiesWidget", "Downloading...", nullptr));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem2->setText(5, QCoreApplication::translate("PropertiesWidget", "Date.now()", nullptr));
        ___qtreewidgetitem2->setText(4, QCoreApplication::translate("PropertiesWidget", "???", nullptr));
        ___qtreewidgetitem2->setText(3, QCoreApplication::translate("PropertiesWidget", "fs.statSync whatever", nullptr));
        ___qtreewidgetitem2->setText(2, QCoreApplication::translate("PropertiesWidget", "cpp function", nullptr));
        ___qtreewidgetitem2->setText(1, QCoreApplication::translate("PropertiesWidget", "a FileDir", nullptr));
        ___qtreewidgetitem2->setText(0, QCoreApplication::translate("PropertiesWidget", "Uploading...", nullptr));
        treeWidget->setSortingEnabled(__sortingEnabled);

        textBrowserObj->setHtml(QCoreApplication::translate("PropertiesWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">\345\215\240\344\275\215\347\254\246</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PropertiesWidget: public Ui_PropertiesWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTIESWIDGET_H
