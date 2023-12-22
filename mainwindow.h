#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTcpSocket>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QUrl>
#include "dialog.h"
#include "scanthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonExist_clicked();

    void on_pushButtonScan_clicked();

    void on_pushButtonStop_clicked();

    void on_actionScan_triggered();

    void recv_result(int, bool); //自定义槽函数，用于接收线程发送的消息

    void recv_resultListen(int, bool);

    void recv_finished_threadnum();//自定义槽函数，用于接收线程结束的消息


    void on_pushButtonListening_clicked();


private:
    Ui::MainWindow *ui;
    Dialog dialog;

    QTreeWidgetItem *itemRoot; //树形控件的条目，用于表示目标IP
    QTreeWidgetItem *itemLeaf; //树形控件的条目，用于表示目标端口

    bool isStop;
    QMutex m_mutex; //互斥变量，全局变量
    QQueue<int> queuePort;
    int threadCount = 0;
    int threadNum ;     //线程总数




protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
