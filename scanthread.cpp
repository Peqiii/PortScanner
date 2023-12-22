#include "scanthread.h"
ScanThread::ScanThread(QString m_strIP, QQueue<int> *queuePort, QMutex *m_mutex, bool *isStop)
{
    this->m_strIP = m_strIP;
    this->queuePort = queuePort;
    this->m_mutex = m_mutex;
    this->isStop = isStop;
}

void ScanThread::run()
{
    QTcpSocket socket(0);
    int port;
    while (!*isStop)
    {
        if(queuePort->empty()) //队列为空，退出线程
            return;

        m_mutex->lock(); //获得锁，其它线程等待
        port =  queuePort->dequeue();
        m_mutex->unlock(); //始放锁

        socket.abort();
        socket.connectToHost(m_strIP, port);
        if(socket.waitForConnected(10))
        {
            emit (send_scan_signal(port, true)); // 发送信号
            //qDebug("%d: %s",port,"opened"); // 调试信息
        }
        else
        {
            emit (send_scan_signal(port, false));
            //qDebug("%d: %s",port,"closed");
        }
    }
}
