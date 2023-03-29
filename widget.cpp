#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QStringList serialportname;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        serialportname<<info.portName();
    }
    ui->comboBox->addItems(serialportname);

    serial = new QSerialPort(this);
    QSerialPort::BaudRate baudrate;
    QSerialPort::DataBits databits;
    QSerialPort::StopBits stopbits;
    QSerialPort::Parity   parity;
    QSerialPort::FlowControl flowcontrol;

    baudrate = QSerialPort::Baud9600;
    databits = QSerialPort::Data8;
    stopbits = QSerialPort::OneStop;
    parity   = QSerialPort::NoParity;
    flowcontrol = QSerialPort::NoFlowControl;

}

Widget::~Widget()
{
    delete ui;
}

