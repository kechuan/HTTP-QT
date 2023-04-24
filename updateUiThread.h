#ifndef UPDATEUITHREAD_H
#define UPDATEUITHREAD_H

#include <QObject>

#include <QDebug>
#include <QThread>
#include <QRunnable>
#include <QThreadPool>

class InvokeThread:public QObject, public QRunnable{
	Q_OBJECT
public:
//	InvokeThread();

	void run() override { //override声明
        qDebug() << "Worker thread id - " << QThread::currentThreadId();
        // do some heavy work here
        for (int i = 0; i < 10; i++) {
            emit progress(i); // report progress
            qDebug("InvokeThread Line:22 run");
        }
        

        QMetaObject::invokeMethod(this, "updateUI", Qt::QueuedConnection); 

    }


signals:
    void progress(int value);

public slots:
    void updateUI() {
        qDebug() << "UI thread id - " << QThread::currentThreadId();
        qDebug() << "Work done!";
    }
};



//...main.cpp


/*

#include "updateUiThread.h"

InvokeThread testThread;
    testThread.run();

    QThreadPool::globalInstance()->start(&testThread); // start the worker in a thread pool

*/

#endif
