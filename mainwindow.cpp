#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionScan_paremeters,  SIGNAL(triggered()), this, SLOT(on_actionScan_triggered()));
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
    int minPort=0;
    int maxPort=0;
    dialog.getScanParams(&minPort,&maxPort);

       if(strIP.isEmpty()){
           QMessageBox::information(this,tr("Error"),QStringLiteral("Please input IP and choose port range!"),QMessageBox::Ok);
           return;
       }


       ui->treeWidget_output->clear();

       QTreeWidgetItem *items1 = new QTreeWidgetItem(ui->treeWidget_output, QStringList(strIP));
       while(minPort<=maxPort)
       {
           QString strPort = QString::number(minPort);
           QTcpSocket socket(0);
           socket.abort();
           socket.connectToHost(strIP,minPort);
           if(socket.waitForConnected(10))
           {
               new QTreeWidgetItem(items1, QStringList(strPort + " is Listening"));
           }
           minPort += 1;
           {
               qApp->processEvents();
           }
       }

   }




void MainWindow::on_label_picture_linkActivated(const QString &link)
{

}

void MainWindow::on_actionScan_triggered()
{
dialog.show();
}

void MainWindow::closeEvent ( QCloseEvent * e )
{
if( QMessageBox::question(this,
tr("Quit"),
tr("Are you sure to quit this application?"),
QMessageBox::Yes, QMessageBox::No )
== QMessageBox::Yes){
e->accept();
//接受退出信号
QApplication::exit();
}
else
e->ignore();
}
