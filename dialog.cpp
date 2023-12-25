#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::getScanParams(int *minPort,int *maxPort,int *threads)
{
    *minPort = this->minPort;
    *maxPort = this->maxPort;
    *threads = this->threads;
    return;
}

void Dialog::getmaxPort(int *maxPort)
{
    *maxPort = this->maxPort;
    return;
}

void Dialog::on_buttonBox_accepted()
{
    minPort=ui->spinBox_min->value();
    maxPort=ui->spinBox_max->value();
    threads=ui->spinBox_thread->value();
    return;
}
