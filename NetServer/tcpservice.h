#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QTcpServer>
#include <QTcpSocket>
#include "../Common/messageheader.h"
#include "../Common/inetevent.h"


class TCPServer : public QTcpServer
{
    Q_OBJECT
public:
    //构造函数
    explicit TCPServer(QObject *parent = 0);
    //析构函数
    ~TCPServer();

public:
    //启动服务
    bool startServer(int port);
    //停止服务
    void stopServer();
    //设置接口
    void setEventObj(INetServerEvent* event)
    {
        iNetEvent = event;
    }

    //接收数据
    void readData();
    //群发数据
    void writeData(QByteArray pack,QByteArray data);
    //指定发送
    void writeData(QString clientIP,int clientPort,QByteArray pack,QByteArray data);

private:
    QList<QTcpSocket*> tcpClient;
    QTcpSocket *currentClient;

    //数据缓冲
    QByteArray readBuffer;
    //事件接口
    INetServerEvent* iNetEvent;

signals:

public slots:
    //客户端连接
    void socketConnectionSlot();
    //客户端断开
    void socketDisConnectSlot();

};

#endif // TCPSERVICE_H
