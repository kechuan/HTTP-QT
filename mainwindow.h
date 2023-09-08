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

    void closeEvent(QCloseEvent *closeEvent);
    void resizeEvent(QResizeEvent *resizeEvent);


    // void dragEnterEvent(QDragEnterEvent *dragEnterEvent);
    // void dropEvent(QDropEvent* dropEvent);
    // void dragLeaveEvent(QDragLeaveEvent *dragLeaveEvent);
    // void mousePressEvent(QMouseEvent *mousePressEvent);
    void keyPressEvent(QKeyEvent *event);


    void Upload(); //需求FileDialog补充信息
    void Upload(QList<QUrl>& DropList); //拖拽获取信息

private:
    Ui::MainWindow *ui; //从此Mainwindow下的指代的指针则为ui  MainWindow *ui = new Ui::MainWindow();
    About *AboutWindow;



private slots:
    void Tab_pressed();
    void itemAccess(QTreeWidgetItem *listItem);
    bool FileList_Menu(QTreeWidgetItem *listItem);


/*Right Click menuList */
    void Rename(QTreeWidgetItem *listItem);
    void Delete();
    void NewDir();
    void Copy();
    void Cut();

    void Refresh();
/*Function Area */
    void LostSelection();
    void showStatus(bool&);



signals:
    void DockProgressCreate(QTreeWidgetItem *Item);

};

#endif // MAINWINDOW_H
