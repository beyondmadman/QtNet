#ifndef INETEVENT_H
#define INETEVENT_H

//网络事件接口
class INetServerEvent
{
public:
    //客户端加入事件
    virtual void OnNetJoin(QString clientIP,int clientPort) = 0;
    //客户端消息事件
    virtual void OnReadData(QString clientIP,int clientPort,qint32 cmdType,QByteArray data) = 0;
    //客户端离开事件
    virtual void OnNetLeave(QString clientIP,int clientPort) = 0;
};

//网络事件接口
class INetClientEvent
{
public:
    //消息事件
    virtual void OnReadData(qint32 cmdType,QByteArray data) = 0;
    //断开信息
    virtual void OnDisconnect() = 0;
    //错误信息
    virtual void OnReadError(QString SocketError) = 0;
};

#endif // INETEVENT_H
