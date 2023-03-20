/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *About;
    QAction *IP_controlPanel;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget_contentShow;
    QWidget *Tab_FileShare;
    QVBoxLayout *verticalLayout_5;
    QTreeWidget *Filelist;
    QWidget *Tab_TaskQueue;
    QVBoxLayout *verticalLayout_6;
    QVBoxLayout *TaskQueue;
    QWidget *Tab_log;
    QVBoxLayout *verticalLayout_2;
    QTextBrowser *textBrowser_log;
    QMenuBar *menubar;
    QMenu *menuSetting;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(750, 523);
        About = new QAction(MainWindow);
        About->setObjectName(QString::fromUtf8("About"));
        IP_controlPanel = new QAction(MainWindow);
        IP_controlPanel->setObjectName(QString::fromUtf8("IP_controlPanel"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget_contentShow = new QTabWidget(centralwidget);
        tabWidget_contentShow->setObjectName(QString::fromUtf8("tabWidget_contentShow"));
        tabWidget_contentShow->setMaximumSize(QSize(2000, 2000));
        Tab_FileShare = new QWidget();
        Tab_FileShare->setObjectName(QString::fromUtf8("Tab_FileShare"));
        verticalLayout_5 = new QVBoxLayout(Tab_FileShare);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        Filelist = new QTreeWidget(Tab_FileShare);
        new QTreeWidgetItem(Filelist);
        Filelist->setObjectName(QString::fromUtf8("Filelist"));
        Filelist->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        Filelist->setSelectionMode(QAbstractItemView::SingleSelection);
        Filelist->setIndentation(25);
        Filelist->setWordWrap(false);
        Filelist->header()->setDefaultSectionSize(200);

        verticalLayout_5->addWidget(Filelist);

        tabWidget_contentShow->addTab(Tab_FileShare, QString());
        Tab_TaskQueue = new QWidget();
        Tab_TaskQueue->setObjectName(QString::fromUtf8("Tab_TaskQueue"));
        verticalLayout_6 = new QVBoxLayout(Tab_TaskQueue);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        TaskQueue = new QVBoxLayout();
        TaskQueue->setObjectName(QString::fromUtf8("TaskQueue"));

        verticalLayout_6->addLayout(TaskQueue);

        tabWidget_contentShow->addTab(Tab_TaskQueue, QString());
        Tab_log = new QWidget();
        Tab_log->setObjectName(QString::fromUtf8("Tab_log"));
        verticalLayout_2 = new QVBoxLayout(Tab_log);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        textBrowser_log = new QTextBrowser(Tab_log);
        textBrowser_log->setObjectName(QString::fromUtf8("textBrowser_log"));
        textBrowser_log->setMinimumSize(QSize(700, 221));
        textBrowser_log->setMaximumSize(QSize(1200, 500));

        verticalLayout_2->addWidget(textBrowser_log);

        tabWidget_contentShow->addTab(Tab_log, QString());

        verticalLayout->addWidget(tabWidget_contentShow);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 750, 26));
        menuSetting = new QMenu(menubar);
        menuSetting->setObjectName(QString::fromUtf8("menuSetting"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSetting->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuSetting->addAction(IP_controlPanel);
        menuHelp->addAction(About);

        retranslateUi(MainWindow);

        tabWidget_contentShow->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        About->setText(QCoreApplication::translate("MainWindow", "About", nullptr));
        IP_controlPanel->setText(QCoreApplication::translate("MainWindow", "IP\346\216\247\345\210\266\345\217\260", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = Filelist->headerItem();
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("MainWindow", "Size", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("MainWindow", "Filename", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "Icon", nullptr));

        const bool __sortingEnabled = Filelist->isSortingEnabled();
        Filelist->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = Filelist->topLevelItem(0);
        ___qtreewidgetitem1->setText(2, QCoreApplication::translate("MainWindow", "setting->IP control Panel", nullptr));
        ___qtreewidgetitem1->setText(1, QCoreApplication::translate("MainWindow", "please config ", nullptr));
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("MainWindow", "To start", nullptr));
        Filelist->setSortingEnabled(__sortingEnabled);

        tabWidget_contentShow->setTabText(tabWidget_contentShow->indexOf(Tab_FileShare), QCoreApplication::translate("MainWindow", "FileShare", nullptr));
        tabWidget_contentShow->setTabText(tabWidget_contentShow->indexOf(Tab_TaskQueue), QCoreApplication::translate("MainWindow", "TaskQueue", nullptr));
        textBrowser_log->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Microsoft YaHei UI'; font-size:9pt; font-weight:400; font-style:normal;\" bgcolor=\"#000000\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffffff;\">console Information...</span></p></body></html>", nullptr));
        tabWidget_contentShow->setTabText(tabWidget_contentShow->indexOf(Tab_log), QCoreApplication::translate("MainWindow", "Log", nullptr));
        menuSetting->setTitle(QCoreApplication::translate("MainWindow", "Setting", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
