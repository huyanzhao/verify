#include "meteraddress.h"
#include "ui_meteraddress.h"
#include <QtNetwork>
#include <QIntValidator>

meterAddress::meterAddress(QString hostIP, int hostPort, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::meterAddress),
    host(hostIP),
    port(hostPort)
{
    ui->setupUi(this);
    ui->lineEditPort->setValidator(new QIntValidator(1, 65535, this));
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnect()));
    ui->lineEditIP->setText(host);
    ui->lineEditPort->setText(QString::number(port));
}
// 析构
meterAddress::~meterAddress()
{
    delete ui;
}
// 测试按钮
void meterAddress::on_pushBtnTest_clicked()
{
    ui->labelStatusInfo->setText(tr("尝试接连中......"));
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/start.bmp"));
    // 取消已有的连接
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->lineEditIP->text(), ui->lineEditPort->text().toInt());
}
// 连接成功
void meterAddress::onConnected()
{
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/pass.bmp"));
    ui->labelStatusInfo->setText(tr("万用表连接成功"));
}
// 断开连接
void meterAddress::disconnect()
{
    ui->labelStatusInfo->setText(tr("万用表连接失败！！！"));
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/fail.bmp"));
}
// 取消按钮
void meterAddress::on_pushBtnCancel_clicked()
{
    this->close();
}
// 确定按钮
void meterAddress::on_pushBtnOk_clicked()
{
    host = ui->lineEditIP->text();
    port = ui->lineEditPort->text().toInt();
    // 发射信号
    emit meterConfigDone(host, port);
    this->close();
}
