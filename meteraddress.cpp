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
// ����
meterAddress::~meterAddress()
{
    delete ui;
}
// ���԰�ť
void meterAddress::on_pushBtnTest_clicked()
{
    ui->labelStatusInfo->setText(tr("���Խ�����......"));
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/start.bmp"));
    // ȡ�����е�����
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->lineEditIP->text(), ui->lineEditPort->text().toInt());
}
// ���ӳɹ�
void meterAddress::onConnected()
{
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/pass.bmp"));
    ui->labelStatusInfo->setText(tr("���ñ����ӳɹ�"));
}
// �Ͽ�����
void meterAddress::disconnect()
{
    ui->labelStatusInfo->setText(tr("���ñ�����ʧ�ܣ�����"));
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/fail.bmp"));
}
// ȡ����ť
void meterAddress::on_pushBtnCancel_clicked()
{
    this->close();
}
// ȷ����ť
void meterAddress::on_pushBtnOk_clicked()
{
    host = ui->lineEditIP->text();
    port = ui->lineEditPort->text().toInt();
    // �����ź�
    emit meterConfigDone(host, port);
    this->close();
}
