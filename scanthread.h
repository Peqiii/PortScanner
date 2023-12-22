#ifndef SCANTHREAD_H
#define SCANTHREAD_H

#include <QThread>
#include <QTcpSocket> //套接字
#include <QQueue>
#include <QDebug> //调试
#include <QMutex> //互斥变量
class ScanThread : public QThread
{
    Q_OBJECT
public:
    ScanThread(QString, QQueue<int> *, QMutex *, bool *);

protected:
    virtual void run(); //run 函数

private:
    QString m_strIP;
    QQueue<int> *queuePort;
    QMutex *m_mutex;
    bool *isStop;
signals:
    void send_scan_signal(int, bool); //信号声明，不需要实现
};

#endif // SCANTHREAD_H
