#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "tcpsocket.h"
#include "../Common/messageheader.h"
#include "../Common/inetevent.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget, public INetClientEvent
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    //发送消息
    //void OnSendData();

    //接收消息
    void OnReadData(qint32 cmdType,QByteArray data);
    //断开信息
    void OnDisconnect();
    //错误信息
    void OnReadError(QString SocketError);

private:
    Ui::Widget *ui;
    TCPSocket *tcpClient;
    qint32 imageIndex;

private slots:
    //启动服务
    void on_btnStart_clicked();
    //停止服务
    void on_btnStop_clicked();
    //清空消息
    void on_btnClear_clicked();
    //发送消息
    void on_btnMessage_clicked();
    //发送图片
    void on_btnImage_clicked();
};

#endif // WIDGET_H
