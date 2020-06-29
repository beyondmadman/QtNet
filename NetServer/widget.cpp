#include "widget.h"
#include "ui_widget.h"
#include <QNetworkInterface>
#include <QDateTime>
#include <QFileDialog>
#include <QFile>


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    imageIndex = 0;

    //设置窗口
    this->setWindowTitle("服务端");
    this->setWindowIcon(QIcon(":/Res/server.jpg"));

    //创建服务
    tcpServer = new TCPServer();

    //获取IP
    ui->edtIP->setText(QNetworkInterface().allAddresses().at(1).toString());

    //设置按钮
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->btnMessage->setEnabled(false);
    ui->btnImage->setEnabled(false);
    ui->edtSend->setReadOnly(true);
}

Widget::~Widget()
{
    delete ui;
    tcpServer->stopServer();
    delete tcpServer;
}

//客户端加入事件
void Widget::OnNetJoin(QString clientIP,int clientPort)
{
    //添加客户端信息
    ui->cbxConnection->addItem(QString("%1:%2").arg(clientIP.split("::ffff:")[1])\
                                          .arg(clientPort));

    ui->edtRecv->setTextColor(QColor(255,0,0));
    ui->edtRecv->append(QString("[%1:%2]客户端进入").arg(clientIP.split("::ffff:")[1]).arg(clientPort));
}
//客户端消息事件
void Widget::OnReadData(QString clientIP,int clientPort,qint32 cmdType,QByteArray data)
{
   QDataStream receive(&data,QIODevice::ReadOnly);
   switch (cmdType)
   {
   case CMD_MESSAGE:
   {
       //包体
       //1数据
       QByteArray msg;
       receive >> msg;

       QString str = QString("[%1:%2]客户端：%3：").arg(clientIP.split("::ffff:")[1]).arg(clientPort).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
       ui->edtRecv->setTextColor(QColor(0,0,255));
       ui->edtRecv->append(str);
       ui->edtRecv->append(msg);
   }
       break;
   case CMD_IMAGE:
   {
       //包体
       //1数据
       QByteArray img;
       receive >> img;

       QString htmlImg = QString("<img src=\"%1\"><img>");
       QString index = QString::number(imageIndex++);

       htmlImg = htmlImg.arg(index + ".png");

       QFile file(index + ".png");
       file.open(QIODevice::WriteOnly);
       file.write(img);
       file.close();

       QString str = QString("[%1:%2]客户端：%3：").arg(clientIP.split("::ffff:")[1]).arg(clientPort).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
       ui->edtRecv->setTextColor(QColor(0,0,255));
       ui->edtRecv->append(str);
       ui->edtRecv->append(htmlImg);
   }
       break;
   default:
       break;
   }
}

//客户端离开事件
void Widget::OnNetLeave(QString clientIP,int clientPort)
{
    // 删除客户端信息
    ui->cbxConnection->removeItem(ui->cbxConnection->findText(tr("%1:%2").arg(clientIP.split("::ffff:")[1]).arg(clientPort)));

    ui->edtRecv->setTextColor(QColor(255,0,0));
    ui->edtRecv->append(QString("[%1:%2]客户端离开").arg(clientIP.split("::ffff:")[1]).arg(clientPort));
}

//启动服务
void Widget::on_btnStart_clicked()
{
   //启动服务
    bool ok = tcpServer->startServer(ui->edtPort->text().toInt());

    if (ok)
    {
        tcpServer->setEventObj(this);

        //设置按钮
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        ui->btnMessage->setEnabled(true);
        ui->btnImage->setEnabled(true);

        ui->edtSend->setReadOnly(false);
        ui->edtRecv->setStyleSheet(" background-color: rgba(100, 225, 100, 30);");
        ui->edtRecv->setTextColor(QColor(255,0,0));
        ui->edtRecv->append("服务器启动");
    }
}

//停止服务
void Widget::on_btnStop_clicked()
{
    tcpServer->stopServer();
    ui->btnStop->setEnabled(false);
    ui->btnMessage->setEnabled(false);
    ui->btnImage->setEnabled(false);
    ui->btnStart->setEnabled(true);

    ui->edtSend->setReadOnly(true);
    ui->edtRecv->setStyleSheet(" background-color: rgba(255, 255, 255);");
    ui->edtRecv->setTextColor(QColor(255,0,0));
    ui->edtRecv->append("服务器停止");
}

//发送消息
void Widget::on_btnMessage_clicked()
{
    QString data = ui->edtSend->toPlainText();

    // 文本为空
    if(data == "")  return;

    QByteArray buffer;
    QDataStream send(&buffer,QIODevice::WriteOnly);

    //包头
    //1数据大小
    qint32 dataLength =0;
    //2包类型
    qint32 cmdType = CMD_MESSAGE;
    //数据加包
    send << dataLength << cmdType;

    //全部连接
    if(ui->cbxConnection->currentIndex() == 0)
    {
        tcpServer->writeData(buffer,data.toUtf8().data());
    }
    //指定连接
    else
    {
        QString clientIP = ui->cbxConnection->currentText().split(":")[0];
        int clientPort = ui->cbxConnection->currentText().split(":")[1].toInt();

        tcpServer->writeData(clientIP,clientPort,buffer,data.toUtf8().data());
    }

    //设置颜色
    ui->edtRecv->setTextColor(QColor(0,0,255));
    QString str = QString("服务端：%1：").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->edtRecv->append(str);
    ui->edtRecv->append(data);

    ui->edtSend->clear();
    ui->edtSend->setFocus();
}

//发送图片
void Widget::on_btnImage_clicked()
{
    QString image = QFileDialog::getOpenFileName(this,"titie",".","Image files (*.png *.jpg *.bmp)");

    if (image.isEmpty())
    {
        return;
    }

    QFile file(image);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();

    QByteArray buffer;
    QDataStream send(&buffer,QIODevice::WriteOnly);

    //包头
    //1数据大小
    qint32 dataLength =0;
    //2包类型
    qint32 cmdType = CMD_IMAGE;
    //数据加包
    send << dataLength << cmdType;

    //全部连接
    if(ui->cbxConnection->currentIndex() == 0)
    {
         tcpServer->writeData(buffer,data);
    }
    //指定连接
    else
    {
        QString clientIP = ui->cbxConnection->currentText().split(":")[0];
        int clientPort = ui->cbxConnection->currentText().split(":")[1].toInt();

        tcpServer->writeData(clientIP,clientPort,buffer,data);
    }



    QString htmlImg = QString("<img src=\"%1\"><img>");
    htmlImg = htmlImg.arg(image);

    ui->edtRecv->setTextColor(QColor(0,0,255));
    QString str = QString("服务端：%1：").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->edtRecv->append(str);
    ui->edtRecv->append(htmlImg);
}

//清空消息
void Widget::on_btnClear_clicked()
{
     ui->edtRecv->clear();
}
