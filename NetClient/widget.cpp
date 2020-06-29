#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    imageIndex = 0;

    //设置窗口
    this->setWindowTitle("客户端");
    this->setWindowIcon(QIcon(":/Res/client.jpg"));

    //初始化客户端
    tcpClient = new TCPSocket();

    ui->btnStart->setEnabled(true);
    ui->btnMessage->setEnabled(false);
    ui->btnImage->setEnabled(false);
    ui->edtSend->setReadOnly(true);
}

Widget::~Widget()
{
    delete ui;
    if(tcpClient->stopSocket())
    {
        delete tcpClient;
    }
}

//消息事件
void Widget::OnReadData(qint32 cmdType,QByteArray data)
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

        QString str = QString("服务端：%1：").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
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

        QString str = QString("服务端：%1：").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        ui->edtRecv->setTextColor(QColor(0,0,255));
        ui->edtRecv->append(str);
        ui->edtRecv->append(htmlImg);
    }
        break;
    default:
        break;
    }
}

//断开信息
void Widget::OnDisconnect()
{
    if (this->isVisible())
    {
        ui->edtRecv->setTextColor(QColor(255,0,0));
        ui->edtRecv->append("服务器断开连接");

        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
        ui->btnMessage->setEnabled(false);
        ui->btnImage->setEnabled(false);
        ui->edtSend->setReadOnly(true);
        ui->edtRecv->setStyleSheet(" background-color: rgba(255, 255, 255);");
    }
}

//错误信息
void Widget::OnReadError(QString SocketError)
{
    //错误信息
    ui->edtRecv->append(SocketError);

    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
    ui->btnMessage->setEnabled(false);
    ui->btnImage->setEnabled(false);
    ui->edtSend->setReadOnly(true);
    ui->edtRecv->setStyleSheet(" background-color: rgba(255, 255, 255);");
    ui->edtRecv->setTextColor(QColor(255,0,0));
}

//启动服务
void Widget::on_btnStart_clicked()
{
    tcpClient->setEventObj(this);

    if (tcpClient->startSocket(ui->edtIP->text(),ui->edtPort->text().toInt()))
    {
        tcpClient->setEventObj(this);

        //设置提示
        ui->edtRecv->setStyleSheet(" background-color: rgba(100, 225, 100, 30);");
        ui->edtRecv->setTextColor(QColor(255,0,0));
        ui->edtRecv->setText("客户端启动成功");

        //设置按钮
        ui->btnStart->setEnabled(false);
        ui->btnStop->setEnabled(true);
        ui->btnMessage->setEnabled(true);
        ui->btnImage->setEnabled(true);
        ui->edtSend->setReadOnly(false);
    }
    else
    {
         ui->edtRecv->setTextColor(QColor(255,0,0));
         ui->edtRecv->setText("服务端未启动");
    }
}

void Widget::on_btnStop_clicked()
{
   if (tcpClient->stopSocket())
    {
        ui->btnStart->setEnabled(true);
        ui->btnStop->setEnabled(false);
        ui->btnMessage->setEnabled(false);
        ui->btnImage->setEnabled(false);
        ui->edtSend->setReadOnly(true);
        ui->edtRecv->setStyleSheet(" background-color: rgba(255, 255, 255);");
        ui->edtRecv->setTextColor(QColor(255,0,0));
        ui->edtRecv->setText("客户端停止");
    }
}

void Widget::on_btnClear_clicked()
{
    ui->edtRecv->clear();
}

void Widget::on_btnMessage_clicked()
{
    QString data = ui->edtSend->toPlainText();
    if(data != "")
    {

        tcpClient->writeData(buffer,data.toUtf8().data());

        QString str = QString("客户端：%1：").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        ui->edtRecv->setTextColor(QColor(0,0,255));
        ui->edtRecv->append(str);
        ui->edtRecv->append(data);
    }

    ui->edtSend->clear();
    ui->edtSend->setFocus();
}

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

    tcpClient->writeData(buffer,data);

    QString htmlImg = QString("<img src=\"%1\"><img>");
    htmlImg = htmlImg.arg(image);

    QString str = QString("客户端：%1：").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->edtRecv->setTextColor(QColor(0,0,255));
    ui->edtRecv->append(str);
    ui->edtRecv->append(htmlImg);
}
