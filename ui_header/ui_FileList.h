/********************************************************************************
** Form generated from reading UI file 'FileList.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILELIST_H
#define UI_FILELIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileListWidget
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *FileList;

    void setupUi(QWidget *FileListWidget)
    {
        if (FileListWidget->objectName().isEmpty())
            FileListWidget->setObjectName(QString::fromUtf8("FileListWidget"));
        FileListWidget->resize(968, 350);
        verticalLayout = new QVBoxLayout(FileListWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        FileList = new QTreeWidget(FileListWidget);
        new QTreeWidgetItem(FileList);
        new QTreeWidgetItem(FileList);
        new QTreeWidgetItem(FileList);
        FileList->setObjectName(QString::fromUtf8("FileList"));
        FileList->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        FileList->setSelectionMode(QAbstractItemView::SingleSelection);
        FileList->setIndentation(25);
        FileList->setWordWrap(false);
        FileList->header()->setDefaultSectionSize(220);

        verticalLayout->addWidget(FileList);


        retranslateUi(FileListWidget);

        QMetaObject::connectSlotsByName(FileListWidget);
    } // setupUi

    void retranslateUi(QWidget *FileListWidget)
    {
        FileListWidget->setWindowTitle(QCoreApplication::translate("FileListWidget", "Form", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = FileList->headerItem();
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("FileListWidget", "Size", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("FileListWidget", "Filename", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("FileListWidget", "Icon", nullptr));

        const bool __sortingEnabled = FileList->isSortingEnabled();
        FileList->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = FileList->topLevelItem(0);
        ___qtreewidgetitem1->setText(2, QCoreApplication::translate("FileListWidget", "setting->IP control Panel", nullptr));
        ___qtreewidgetitem1->setText(1, QCoreApplication::translate("FileListWidget", "please config ", nullptr));
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("FileListWidget", "To start", nullptr));
        QTreeWidgetItem *___qtreewidgetitem2 = FileList->topLevelItem(1);
        ___qtreewidgetitem2->setText(2, QCoreApplication::translate("FileListWidget", "setting->DownloadPath Setting", nullptr));
        ___qtreewidgetitem2->setText(1, QCoreApplication::translate("FileListWidget", "please config ", nullptr));
        ___qtreewidgetitem2->setText(0, QCoreApplication::translate("FileListWidget", "Change Path", nullptr));
        QTreeWidgetItem *___qtreewidgetitem3 = FileList->topLevelItem(2);
        ___qtreewidgetitem3->setText(2, QCoreApplication::translate("FileListWidget", "MaxThread", nullptr));
        ___qtreewidgetitem3->setText(1, QCoreApplication::translate("FileListWidget", "please click ", nullptr));
        ___qtreewidgetitem3->setText(0, QCoreApplication::translate("FileListWidget", "Change Threads", nullptr));
        FileList->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class FileListWidget: public Ui_FileListWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILELIST_H
