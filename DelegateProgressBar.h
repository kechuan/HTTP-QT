#ifndef DELEGATEPROGRESSBAR_H
#define DELEGATEPROGRESSBAR_H

#include "qpainter.h"
#include "qprogressbar.h"
//#include "qspinbox.h"
#include "qstyleditemdelegate.h"


//委托类
//继承自 QStyledItemDelegate 样式化委托

class ProgressBarDelegate : public QStyledItemDelegate {
public:
    ProgressBarDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {} //将重载之后的函数直接构造运行 parent指向QStyledItemDelegate

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        QProgressBar *progress = new QProgressBar();
        progress->setMinimum(0);
        progress->setMaximum(100);

        progress->setValue(index.data(Qt::UserRole).toInt());
        progress->setGeometry(option.rect); //位置信息

        progress->setStyleSheet
        (
            "QProgressBar {border: 2px solid grey;border-radius: 5px;background-color: #FFFFFF;}"
            "QProgressBar::chunk {background-color: #CD96CD;width: 20px;}"
        );
        progress->setAlignment(Qt::AlignCenter);

        progress->setFormat(QString::number(index.data(Qt::UserRole).toInt()) + "%");

        painter->save(); //save the inital painter data.

        painter->translate(option.rect.topLeft()); //画布以 QStyleOptionViewItem 左上角开始 这里指代TreeWidgetsItems
        progress->render(painter); // 开始渲染
        painter->restore(); //还原: translate归0
    }

    //    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
    //        QSpinBox *editor = new QSpinBox(parent);
    //        editor->setMinimum(0);
    //        editor->setMaximum(100);
    //        return editor;
    //    }

    // void setEditorData(QWidget *editor, const QModelIndex &index) const override {
    //     int value = index.model()->data(index, Qt::UserRole).toInt();
    //     static_cast<QSpinBox *>(editor)->setValue(value);
    // }

    // void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {
    //     int value = static_cast<QSpinBox *>(editor)->value(); //类型转换 将QSpinBox尽可能转化为int
    //     model->setData(index, value, Qt::UserRole);
    // }

    //更新geometry位置
    // void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
    //     editor->setGeometry(option.rect);
    // }
};



#endif
