#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "about.h"

QT_BEGIN_NAMESPACE
namespace Ui { 
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    About *AboutWindow;
    

private slots:
    void action_pressed();
    void Tab_pressed();
    void itemAccess(QTreeWidgetItem *listItem, int column);
    

    void showTreeRightMenu(QPoint);
    void menu_blank();

private:
    Ui::MainWindow *ui; //从此Mainwindow下的指代的指针则为ui  MainWindow *ui = new Ui::MainWindow();
    enum itemType{root = QTreeWidgetItem::UserType,child};
    
};

#endif // MAINWINDOW_H
