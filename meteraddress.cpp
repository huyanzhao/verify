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
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(onConnected()));
    connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(disconnect()));
    qDebug() << tr("���յ������ñ�IP��") << host << tr(", �˿ڣ�") << QString::number(port);
    ui->lineEditIP->setText(host);
    ui->lineEditPort->setText(QString::number(port));
}

meterAddress::~meterAddress()
{
    delete ui;
}

void meterAddress::on_pushBtnTest_clicked()
{
    qDebug() << tr("����");
    ui->labelStatusInfo->setText(tr("���Խ�����......"));
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/start.bmp"));
    // ȡ�����е�����
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->lineEditIP->text(), ui->lineEditPort->text().toInt());
}

void meterAddress::onConnected()
{
    qDebug() << tr("�����ɹ�����");
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/pass.bmp"));
    ui->labelStatusInfo->setText(tr("���ñ����ӳɹ�"));
}

void meterAddress::disconnect()
{
    qDebug() << tr("�Ͽ�����������");
    ui->labelStatusInfo->setText(tr("���ñ�����ʧ�ܣ�����"));
    ui->labelStatusIcon->setPixmap(QPixmap(":/new/prefix/img/fail.bmp"));
}

void meterAddress::on_pushBtnCancel_clicked()
{
    this->close();
}

void meterAddress::on_pushBtnOk_clicked()
{
    qDebug() << tr("ȷ��");
    host = ui->lineEditIP->text();
    port = ui->lineEditPort->text().toInt();
    // �����ź�
    emit dlgReturn(host, port);

}
