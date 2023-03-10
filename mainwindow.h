#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "about.h"
// #include "IPcontorlPanel.h"

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
//    IP_contorlPanel *IP_contorlPanelWindow;

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
    bool TreeWidgetItem_Menu(QTreeWidgetItem *listItem, int column);

    void menu_blank();

    // void menuPressSlot();

//menuList

    //General Area
    // void Test();
    void Rename();
    void Delete();
    // void Refresh(); -> cli surfingPath
    void newDir();

    //Dir Area
    // void Open() -> itemAccess
    
    //File Area
    // void Download(); -> itemAccess

};

#endif // MAINWINDOW_H
