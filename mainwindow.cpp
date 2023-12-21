#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonExist_clicked()
{
     connect(ui->pushButtonExist, SIGNAL(clicked()), this, SLOT(close()));
}

void MainWindow::on_pushButtonScan_clicked()
{
    QString strIP = ui->lineEdit_IP->text();

       if(strIP.isEmpty()){
           QMessageBox::information(this,tr("Error"),QStringLiteral("请输入IP"),QMessageBox::Ok);
           return;
       }


       ui->treeWidget_output->clear();

       QTreeWidgetItem *items1 = new QTreeWidgetItem(ui->treeWidget_output, QStringList(strIP));
       for(int intPort=1;intPort<1024;intPort++)
       {
       QString strPort = QString::number(intPort);
       QTcpSocket socket(0);
       socket.abort();
       socket.connectToHost(strIP,intPort);
       if(socket.waitForConnected(10))
       {
           new QTreeWidgetItem(items1, QStringList(strPort + " is Listening"));

       }

       }
}



void MainWindow::on_label_picture_linkActivated(const QString &link)
{

}
