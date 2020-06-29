#include "tcpsocket.h"
#include <QHostAddress>
#include <QDebug>

//构造函数
TCPSocket::TCPSocket(QObject *parent)
    : QTcpSocket(parent)
{
}

//析构函数
TCPSocket::~TCPSocket()
{
    this->disconnectFromHost();
    this->deleteLater();
}

//服务端断开客户端
void TCPSocket::serverDisConnectSlot(const int id)
{
    //指定ID断开
    if (id == socketID)
    {
        this->disconnectFromHost();
    }
    //-1全部断开
    else if (id == -1)
    {
        this->disconnectFromHost();
        this->deleteLater();
    }
}

void TCPSocket::writeData(const QByteArray &data, const int id)
{
    if(id == socketID)
    {
        write(data);
    }
}

void TCPSocket::readData()
{

}
