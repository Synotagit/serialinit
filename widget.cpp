#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QString>
#include <QSerialPort>
#include <QTimer>
#include <QStringList>
#include <QDebug>
#include <QTimer>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

        //获取系统的有效串口
        foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        qDebug()<<info.portName()<<info.description();
        ui->portbox->addItem(info.portName());

        QSerialPort serial(info);

        //测试串口是否空闲
        if(serial.open(QIODevice::ReadWrite)){
        ui->portbox->addItem(info.portName());
        serial.close();
        }
        else{
        ui->portbox->addItem(info.portName()+"(被占用)");
        }
    }

        //未打开串口不使能发送按钮
        if(ui->openserialbtn->text()=="打开串口")
        ui->sendbtn->setEnabled(false);

        //1秒更新一次串口
        timer = new QTimer;
        connect(timer,&QTimer::timeout,this,&Widget::UpdatePort);
        timer->start(1000);
}

Widget::~Widget()
{
    delete ui;
}

//打开串口按钮槽
void Widget::on_openserialbtn_clicked()
{

     //如果串口为关闭状态时按下
    if(ui->openserialbtn->text()=="打开串口")
    {
        //串口为当前串口box显示的串口
        serial = new QSerialPort(ui->portbox->currentText());

        //使能发送按钮
        ui->sendbtn->setEnabled(true);

        //不使能portbox
        ui->portbox->setEnabled(false);

        //如果串口正在使用
        if(!serial->open(QIODevice::ReadWrite)){
            QMessageBox::warning(this,"tips","串口打开失败");
            serial->deleteLater();
            return;
        }

        //初始化
        serial->setBaudRate(QSerialPort::Baud9600);
        serial->setDataBits(QSerialPort::Data8);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setParity(QSerialPort::NoParity);
        serial->setFlowControl(QSerialPort::NoFlowControl);

         //不使能portbox
         ui->portbox->setEnabled(false);

         //槽，使接收框接收数据
         connect(serial,&QSerialPort::readyRead,this,[&](){
         QByteArray arr = serial ->readAll();
         if(!arr.isEmpty())
         ui->receiveedit->insertPlainText(arr);

        });

        ui->openserialbtn->setText("关闭串口");
    }

    //串口处于打开状态
    else{
        //释放串口
        serial->clear();
        serial->close();
        serial->deleteLater();

        //使能portbox
        ui->portbox->setEnabled(true);
        //不使能发送按钮
        ui->sendbtn->setEnabled(false);
        ui->openserialbtn->setText("打开串口");
    }
}

//更新串口函数定义
void Widget::UpdatePort()
{
    //可使用的串口赋值给newPortStringList
    QStringList newPortStringList;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        newPortStringList += info.portName();
    }

    //把新串口加入portbox
    if(newPortStringList.size() != oldPortStringList.size())
    {
        oldPortStringList = newPortStringList;
        ui->portbox->clear();
        ui->portbox->addItems(oldPortStringList);
    }

}

//发送按钮槽
void Widget::on_sendbtn_clicked()
{
        QByteArray arr = ui->sendedit->toPlainText().toLocal8Bit();
        //发送
        serial->write(arr);
    }
