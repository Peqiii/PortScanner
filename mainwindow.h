#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidget>
#include <QTcpSocket>
#include <QCloseEvent>

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

    void on_actionScan_triggered();

    void on_label_picture_linkActivated(const QString &link);

private:
    Ui::MainWindow *ui;
    Dialog dialog;

protected:
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
