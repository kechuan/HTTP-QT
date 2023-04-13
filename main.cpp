#include "mainwindow.h"
#include "qthread.h"

#include <QApplication>
#include <QTranslator>

#include "connect.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // extern Connect Client1;

    // QThread thread;
    // Client1.moveToThread(&thread);
    // thread.start();

    qDebug() << "main thread:" << QThread::currentThreadId();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "HTTP-QT_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
     MainWindow w;
     w.show();

    return a.exec();
}
