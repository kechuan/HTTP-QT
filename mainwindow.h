#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>

#include "about.h"
#include "ip_controlpanel.h" //sub .h include done.
#include "propertieswidget.h"

#include "FileList.h"

QT_BEGIN_NAMESPACE
namespace Ui { 
    class MainWindow;
}
QT_END_NAMESPACE

extern FileList *SurfingFile;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void closeEvent(QCloseEvent *closeEvent);
    void resizeEvent(QResizeEvent *resizeEvent);


    // void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
    // void dropEvent(QDropEvent* dropEvent);
//    void dragLeaveEvent(QDragLeaveEvent *dragLeaveEvent);

//    void mousePressEvent(QMouseEvent *mousePressEvent);
    void keyPressEvent(QKeyEvent *event);


    void Upload(); //需求FileDialog补充信息
    void Upload(QList<QUrl>& DropList); //拖拽获取信息

private:
    Ui::MainWindow *ui; //从此Mainwindow下的指代的指针则为ui  MainWindow *ui = new Ui::MainWindow();
    About *AboutWindow;
    IP_controlPanel *IP_controlPanelWindow;
    PropertiesWidget *DockWidget;

    QThread *thread; // 定义一个 QThread 的指针


private slots:
    void Tab_pressed();
    void itemAccess(QTreeWidgetItem *listItem, int column);
    bool FileList_Menu(QTreeWidgetItem *listItem, int column);

    // void updatePropertiesProgress(size_t& Progress,QTreeWidgetItem* listItem);


    // void menuPressSlot();

//menuList

    //General Area
    // void Test();
    void Rename(QTreeWidgetItem *listItem);
    void Delete();
    void NewDir();
    void Copy();
    void Cut();

    void Refresh();

    
    

    //Dir Area
    // void Open() -> itemAccess
    
    //File Area
    // void Download(); -> itemAccess


    void LostSelection(int column);

    void showStatus(bool&);

    void clearStatusList();

    // void SurfingDownloadPath();

signals:
    void connetPressed();
    void DownloadPressed(const char* Status,QTreeWidgetItem*);
    void DockProgressCreate(QTreeWidgetItem *Item);

};

#endif // MAINWINDOW_H
