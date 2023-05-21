#include "mainwindow.h"
#include "qthread.h"

#include <QApplication>
#include <QTranslator>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<"main ID:"<<QThread::currentThreadId();


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
