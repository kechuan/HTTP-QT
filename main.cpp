#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

//#include <QtCore/QTextCodec>

//# pragma execution_character_set("GBK")

int main(int argc, char *argv[])
{
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//    system("chcp 65001");
//    std::locale::global(std::locale("")); //将运行语言切换成本地
    QApplication a(argc, argv);

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
