#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionScan_paremeters,  SIGNAL(triggered()), this, SLOT(on_actionScan_triggered()));
    isStop = false;
    //给QTreeWidget 初始化表头
    QStringList head;
    head << "Scan result:";
    ui->treeWidget_output->setHeaderLabels(head);
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
        isStop = false;//再次点击扫描时，flag设置为false
        threadCount = 0;//再次点击时，线程数量计数器清零
        ui->pushButtonStop->setEnabled(true);//再次点击时，停止按钮设置为可用
        ui->pushButtonScan->setEnabled(false);//扫描按钮设置为不可用，扫描结束后或者点击stop后再设置为可用
        ui->pushButtonListening->setEnabled(false);

        QString strIP = ui->lineEdit_IP->text();
        int startPort;
        int endPort;
        int threads;
        dialog.getScanParams(&startPort,&endPort,&threads);
        threadNum = threads;

        if(strIP.isEmpty()){
            QMessageBox::information(this,"Error","Please in put IP",QMessageBox::Ok);
            return;
        }
        if(startPort == 0){
            QMessageBox::information(this,"Error","Please input start port",QMessageBox::Ok);
            return;
        }
        if(endPort == 0){
            QMessageBox::information(this,"Error","Please input end port",QMessageBox::Ok);
            return;
        }
        if(endPort < startPort){
            QMessageBox::information(this,"Error", "Start port couldn't bigger than end port",QMessageBox::Ok);
            return;
        }
        for(int i=startPort; i<=endPort;i++){
            queuePort.enqueue(i);//端口号加入队列
        }

        ui->treeWidget_output->clear();
        itemRoot = new QTreeWidgetItem(ui->treeWidget_output, QStringList(strIP));
        //开启十个扫描线程
        for (int i = 0; i< threads; i++) {
            ScanThread *myThread = new ScanThread(strIP, &queuePort, &m_mutex, &isStop);
            //连接myThread 的信号send_scan_signal(int, bool)和槽函数recv_result(int, bool)
            connect(myThread, SIGNAL(send_scan_signal(int, bool)),this,SLOT(recv_result(int, bool)));
            //当收到线程类的finished信号后，delete线程对象
            connect(myThread, &ScanThread::finished, myThread, &QObject::deleteLater);
            //当收到线程类的finished信号后，在接收的槽函数进行统计。一个信号可以对应多个槽函数
            connect(myThread, &ScanThread::finished, this, &MainWindow::recv_finished_threadnum);
            myThread->start();
        }
}

void MainWindow::on_pushButtonListening_clicked()
{
    isStop = false;//再次点击扫描时，flag设置为false
    threadCount = 0;//再次点击时，线程数量计数器清零
    ui->pushButtonStop->setEnabled(true);//再次点击时，停止按钮设置为可用
    ui->pushButtonScan->setEnabled(false);//扫描按钮设置为不可用，扫描结束后或者点击stop后再设置为可用
    ui->pushButtonListening->setEnabled(false);

    QString strIP = ui->lineEdit_IP->text();
    int startPort;
    int endPort;
    int threads;
    dialog.getScanParams(&startPort,&endPort,&threads);
    threadNum = threads;

    if(strIP.isEmpty()){
        QMessageBox::information(this,"Error","Please in put IP",QMessageBox::Ok);
        return;
    }
    if(startPort == 0){
        QMessageBox::information(this,"Error","Please input start port",QMessageBox::Ok);
        return;
    }
    if(endPort == 0){
        QMessageBox::information(this,"Error","Please input end port",QMessageBox::Ok);
        return;
    }
    if(endPort < startPort){
        QMessageBox::information(this,"Error", "Start port couldn't bigger than end port",QMessageBox::Ok);
        return;
    }
    for(int i=startPort; i<=endPort;i++){
        queuePort.enqueue(i);//端口号加入队列
    }

    ui->treeWidget_output->clear();
    itemRoot = new QTreeWidgetItem(ui->treeWidget_output, QStringList(strIP));
    //开启十个扫描线程
    for (int i = 0; i< threads; i++) {
        ScanThread *myThread = new ScanThread(strIP, &queuePort, &m_mutex, &isStop);
        //连接myThread 的信号send_scan_signal(int, bool)和槽函数recv_result(int, bool)
        connect(myThread, SIGNAL(send_scan_signal(int, bool)),this,SLOT(recv_resultListen(int, bool)));
        //当收到线程类的finished信号后，delete线程对象
        connect(myThread, &ScanThread::finished, myThread, &QObject::deleteLater);
        //当收到线程类的finished信号后，在接收的槽函数进行统计。一个信号可以对应多个槽函数
        connect(myThread, &ScanThread::finished, this, &MainWindow::recv_finished_threadnum);
        myThread->start();
    }
}

void MainWindow::on_pushButtonStop_clicked()
{

    isStop = true;  //给线程类传递了isStop的地址，双方共享一个变量的内存，一改俱改
    m_mutex.lock(); //获得锁，其它线程等待
    queuePort.clear();//点击stop之后，清空任务队列，以免下次扫描时还有剩余任务
    m_mutex.unlock(); //始放锁

    ui->pushButtonScan->setEnabled(true);
    ui->pushButtonListening->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);

}


//在每次调用 recv_result 函数之前，检查 itemLeaf 指针是否已经指向了一个有效的对象。如果是，首先删除该对象以释放内存。
void MainWindow::recv_result(int port, bool isOpen)
{
    QString strPort = QString::number(port);
    if(isOpen)
        itemLeaf = new QTreeWidgetItem(itemRoot, QStringList(strPort + "is Listening"));
    else
        itemLeaf = new QTreeWidgetItem(itemRoot, QStringList(strPort + " closed"));
}

void MainWindow::recv_resultListen(int port, bool isOpen)
{
    QString strPort = QString::number(port);
    if (isOpen)
        itemLeaf = new QTreeWidgetItem(itemRoot, QStringList(strPort + "is Listening"));
}

void MainWindow::recv_finished_threadnum()
{
    threadCount ++;
    if(threadCount == threadNum){
        ui->pushButtonScan->setEnabled(true);
        ui->pushButtonListening->setEnabled(true);
        ui->pushButtonStop->setEnabled(false);
        QMessageBox::information(this,"info","Scanner is ending");
    }
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




void MainWindow::on_pushButton_test_clicked()
{
    QProcess *process = new QProcess(this);
    QString strIP = ui->lineEdit_IP->text();
    connect(process, SIGNAL(readyRead()),this, SLOT(on_read()));
    process->start("ping",QStringList() <<strIP);
}

void MainWindow::on_read()
{
  QProcess *pProces = (QProcess *)sender();
  QElapsedTimer time;
  time.start();
  while(time.elapsed()<5000)
  QCoreApplication::processEvents();
  QString result = pProces->readAll();
  QMessageBox::warning(NULL, "", result);
}
