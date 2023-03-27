#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include "about.h"
#include "ip_controlpanel.h" //sub .h include done.
#include "propertieswidget.h"


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
    void keyPressEvent(QKeyEvent *event);
//    void mousePressEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui; //从此Mainwindow下的指代的指针则为ui  MainWindow *ui = new Ui::MainWindow();
    About *AboutWindow;
    IP_controlPanel *IP_controlPanelWindow;
    PropertiesWidget *DockWidget;


private slots:
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
    void NewDir();
    void Copy();
    void Cut();
    // void Upload(); -> get?
    // void Refresh(); -> cli surfingPath
    

    //Dir Area
    // void Open() -> itemAccess
    
    //File Area
    // void Download(); -> itemAccess


    void LostSelection(int column);

    void showStatus(bool&);

    void clearStatusList();

signals:
    void connetPressed();

};

#endif // MAINWINDOW_H
