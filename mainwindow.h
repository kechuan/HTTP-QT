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

private:
    Ui::MainWindow *ui; //从此Mainwindow下的指代的指针则为ui  MainWindow *ui = new Ui::MainWindow();
    enum itemType{
        root = QTreeWidgetItem::UserType, //1000  用于自定义类型的最小值。低于 UserType 的值由 Qt 预留。
        child
    };

    

private slots:
    void action_pressed();
    void Tab_pressed();
    void itemAccess(QTreeWidgetItem *listItem, int column);
    bool TreeWidgetItemPressed_Slot(QTreeWidgetItem *listItem, int column);

    // void showTreeRightMenu(QPoint);
    void menu_blank();

    // void menuPressSlot();

//menuList

    //General Area
    // void Test();
    // void Rename();
    // void Delete();
    void Refresh();
    // void newDir();

    //Dir Area
    void Open();
    
    //File Area
    // void Download();


};

#endif // MAINWINDOW_H
