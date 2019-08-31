#include "meteraddress.h"
#include "ui_meteraddress.h"
#include <QDebug>
#include <QtNetwork>

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
    qDebug() << tr("recive IP of multimeter:") << host << tr(", port:") << QString::number(port);
    ui->lineEditIP->setText(host);
    ui->lineEditPort->setText(QString::number(port));
}

meterAddress::~meterAddress()
{
    delete ui;
}

void meterAddress::on_pushBtnTest_clicked()
{
    qDebug() << tr("Test");
    ui->labelStatusInfo->setText(tr("尝试接连中......"));
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/start.bmp"));
    // 取消已有的连接
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->lineEditIP->text(), ui->lineEditPort->text().toInt());
}

void meterAddress::onConnected()
{
    qDebug() << tr("Success in succession！！");
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/pass.bmp"));
    ui->labelStatusInfo->setText(tr("万用表连接成功"));
}

void meterAddress::disconnect()
{
    qDebug() << tr("Disconnect！！！");
    ui->labelStatusInfo->setText(tr("万用表连接失败！！！"));
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/fail.bmp"));
}

void meterAddress::on_pushBtnCancel_clicked()
{
    this->close();
}

void meterAddress::on_pushBtnOk_clicked()
{
    qDebug() << tr("Determine");
    host = ui->lineEditIP->text();
    port = ui->lineEditPort->text().toInt();
    // 发射信号
    emit meterConfigDone(host, port);

}
