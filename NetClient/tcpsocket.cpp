#include "tcpsocket.h"
#include <QDataStream>

TCPSocket::TCPSocket(QObject *parent) : QTcpSocket(parent)
{
    connect(this, &QTcpSocket::readyRead, this, readData);
    connect(this,&QTcpSocket::disconnected, this ,scoketDisconnect);
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), \
            this, SLOT(readError(QAbstractSocket::SocketError)));
}

TCPSocket::~TCPSocket()
{

}

bool TCPSocket::startSocket(QString ip,qint32 port)
{

    this->connectToHost(ip, port);

    return this->waitForConnected(1000);
}

bool TCPSocket::stopSocket()
{
    bool ret = false;
    this->disconnectFromHost();
    if (this->state() == QAbstractSocket::UnconnectedState
            || this->waitForDisconnected(1000))
    {
        this->close();
        ret = true;
    }
    return ret;
}

//接收数据
void TCPSocket::readData()
{

    QByteArray buffer;
    buffer = readAll();

    QDataStream receive(&readBuffer,QIODevice::ReadOnly);
    readBuffer.append(buffer);
    qint64 readSize = readBuffer.size();

    while(readSize)
    {
        //包头
        //1数据大小
        qint32 dataLength = 0;
        //2包类型
        qint32 cmdType = CMD_MESSAGE;

        qint64 packSize = sizeof(dataLength) + sizeof(cmdType);
        if(readSize < packSize)
        {
            //qDebug() << "报头数据不够";
            break;
        }

        //数据拆包
        receive >> dataLength;
        receive >> cmdType;

        qint64 recvSize = dataLength;

        if(readSize < recvSize)
        {
            //qDebug() << "传输数据不够";
            break;
        }

        QByteArray recvData = readBuffer.mid(packSize,recvSize);
        if (iNetEvent)
        {
            iNetEvent->OnReadData(cmdType,recvData);
        }

        //缓存数据
        buffer = readBuffer.right(readSize - recvSize);
        readSize = buffer.size();
        readBuffer = buffer;
    }
}

//发送数据
void TCPSocket::writeData(QByteArray pack,QByteArray data)
{
    QByteArray buffer;
    buffer = pack;
    QDataStream send(&buffer,QIODevice::WriteOnly);

    //数据加包
    send.device()->seek(buffer.size());
    send << data;
    send.device()->seek(0);
    send << buffer.size();

    //发送数据
    this->write(buffer);
}

//断开信息
void TCPSocket::scoketDisconnect()
{
    //断开连接
    if (NULL != this)
    {
        this->close();

        if (iNetEvent)
        {
            iNetEvent->OnDisconnect();
        }
    }
}

//错误信息
void TCPSocket::readError(QAbstractSocket::SocketError)
{
    //断开连接
    this->disconnectFromHost();

    if (iNetEvent)
    {
        iNetEvent->OnReadError(QString("连接出错：%1").arg(this->errorString()));
    }
}
