#ifndef UPDATEUITHREAD_H
#define UPDATEUITHREAD_H

#include <QObject>
#include <QTreeWidgetItem>

class InvokeThread:public QObject{
	Q_OBJECT
	InvokeThread(QTreeWidgetItem* listItem,QObject *parent);

	void run();

}