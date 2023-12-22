#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void  getScanParams(int *minPort,int *maxPort,int *threads);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Dialog *ui;
    int minPort=1;
    int maxPort=1024;
    int threads=10;

};

#endif // DIALOG_H
