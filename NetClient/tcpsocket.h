#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QHostAddress>
#include "../Common/messageheader.h"
#include "../Common/inetevent.h"

class TCPSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TCPSocket(QObject *parent = 0);
    //析构函数
    ~TCPSocket();

    //启动服务
    bool startSocket(QString ip,qint32 port);
    //停止服务
    bool stopSocket();
    //设置接口
    void setEventObj(INetClientEvent* event)
    {
        iNetEvent = event;
    }

    //接收数据
    void readData();
    //发送数据
    void writeData(QByteArray pack,QByteArray data);

private:
    //数据缓冲
    QByteArray readBuffer;
    //事件接口
    INetClientEvent* iNetEvent;

signals:

public slots:
    //断开信息
    void scoketDisconnect();
    //错误信息
    void readError(QAbstractSocket::SocketError);
};

#endif // TCPSOCKET_H
