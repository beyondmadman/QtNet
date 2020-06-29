#include "tcpservice.h"
#include <QDataStream>

//构造函数
TCPServer::TCPServer(QObject *parent) : QTcpServer(parent)
{
   iNetEvent = NULL;

   connect(this, &TCPServer::newConnection,this,socketConnectionSlot);
}

//析构函数
TCPServer:: ~TCPServer()
{

}

//启动服务
bool TCPServer::startServer(int port)
{
    //创建监听
    bool ret = this->listen(QHostAddress::Any, port);

    return ret;
}

//停止服务
void TCPServer::stopServer()
{
    //移除客户端
    for(int i = 0; i<tcpClient.length(); i++)
    {
        tcpClient[i]->disconnectFromHost();
    }

    //关闭服务端
    this->close();
}

//客户端连接
 void TCPServer::socketConnectionSlot()
 {
     currentClient = this->nextPendingConnection();
     tcpClient.append(currentClient);

     //客户端加入事件
     if (iNetEvent)
     {
         iNetEvent->OnNetJoin(currentClient->peerAddress().toString(),currentClient->peerPort());
     }

     connect(currentClient, &QTcpSocket::readyRead, this, readData);
     connect(currentClient, &QTcpSocket::disconnected, this, socketDisConnectSlot);
 }

//客户端断开
void TCPServer::socketDisConnectSlot()
{
    //移除客户端
    for(int i=0; i<tcpClient.length(); i++)
    {
        if(tcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
            //客户端离开事件
            if (iNetEvent)
            {
                iNetEvent->OnNetLeave(tcpClient[i]->peerAddress().toString(),tcpClient[i]->peerPort());
            }

            tcpClient[i]->destroyed();
            tcpClient.removeAt(i);
        }
    }
}

void TCPServer::readData()
{
    // 遍历客户端
    for(int i=0; i<tcpClient.length(); i++)
    {
        QByteArray buffer;
        buffer = tcpClient[i]->readAll();

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
               // qDebug() << "报头数据不够";
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
                iNetEvent->OnReadData(tcpClient[i]->peerAddress().toString(),tcpClient[i]->peerPort(),cmdType,recvData);
            }

            //缓存数据
            buffer = readBuffer.right(readSize - recvSize);
            readSize = buffer.size();
            readBuffer = buffer;
        }
    }
}

//群发数据
void TCPServer::writeData(QByteArray pack,QByteArray data)
{
    QByteArray buffer;
    buffer = pack;
    QDataStream send(&buffer,QIODevice::WriteOnly);

    //数据加包
    send.device()->seek(buffer.size());
    send << data;
    send.device()->seek(0);
    send << buffer.size();

    for(int i=0; i<tcpClient.length(); i++)
    {
        //发送数据
        tcpClient[i]->write(buffer);
    }
}

//指定发送
void TCPServer::writeData(QString clientIP,int clientPort,QByteArray pack,QByteArray data)
{
    QByteArray buffer;
    buffer = pack;
    QDataStream send(&buffer,QIODevice::WriteOnly);

    //数据加包
    send.device()->seek(buffer.size());
    send << data;
    send.device()->seek(0);
    send << buffer.size();

    for(int i=0; i<tcpClient.length(); i++)
    {
        if(tcpClient[i]->peerAddress().toString().split("::ffff:")[1]==clientIP\
                        && tcpClient[i]->peerPort()==clientPort)
        {
            tcpClient[i]->write(buffer);
            return;
        }
    }
}

