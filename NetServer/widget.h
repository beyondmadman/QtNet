#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "tcpservice.h"
#include "../Common/messageheader.h"
#include "../Common/inetevent.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget, public INetServerEvent
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    //启动服务
    void on_btnStart_clicked();
    //停止服务
    void on_btnStop_clicked(); 
    //发送消息
    void on_btnMessage_clicked();
    //发送图片
    void on_btnImage_clicked();
    //清空消息
    void on_btnClear_clicked();

private:
    //客户端加入事件
    void OnNetJoin(QString clientIP,int clientPort);
    //客户端离开事件
    void OnNetLeave(QString clientIP,int clientPort);
    //客户端消息事件
    void OnReadData(QString clientIP,int clientPort,qint32 cmdType,QByteArray data);

private:
    Ui::Widget *ui;

    TCPServer *tcpServer;
    qint32 imageIndex;
};

#endif // WIDGET_H
