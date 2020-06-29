#ifndef TCPITEM_H
#define TCPITEM_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class TCPSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit TCPSocket(QObject *parent = 0);
    ~TCPSocket();

public:
    //写入数据
    void writeData(const QByteArray &data, const int id);
    //读取数据
    void readData();

private:
    //套接字ID
    qintptr socketID;


signals:
     void socketDisConnect(const int id,const QString &,const quint16, QThread *);

public slots:
     void serverDisConnectSlot(const int id);

};

#endif // TCPITEM_H
