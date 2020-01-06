#include "batchadd.h"
#include "ui_batchadd.h"
#include <QMessageBox>
#include <QDoubleValidator>
#include "qstringinthex.h"

BatchAdd::BatchAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BatchAdd)
{
    ui->setupUi(this);
    ui->lineEditNum->setValidator(new QIntValidator(1, 10000, this));
    ui->lineEditStartData->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    ui->lineEditDataStep->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
}

BatchAdd::~BatchAdd()
{
    delete ui;
}
// ����
void BatchAdd::on_pushBtnGenerate_clicked()
{
    QString number, dataStart, dataSetp, addrStart;
    bool isRise;
    number = ui->lineEditNum->text();
    dataStart = ui->lineEditStartData->text();
    dataSetp = ui->lineEditDataStep->text();
    addrStart = ui->lineEditStartAddr->text();

    if(number.size() == 0 || dataStart.size() == 0 || dataSetp.size() == 0 || addrStart.size() ==0){
        QMessageBox::information(this, QString(tr("����ʧ��")), QString(tr("����δ��д��������������д")),QMessageBox::Yes);
        return;
    }else if(!QStringIsInt(addrStart)){
        QMessageBox::information(this, QString(tr("����ʧ��")), QString(tr("��ʼ��ַ������Ч��ʮ���ƻ�ʮ������������������д")),QMessageBox::Yes);
        return;
    }else if(number == 0){
        QMessageBox::information(this, QString(tr("����ʧ��")), QString(tr("���ݸ�������Ϊ0����������д")),QMessageBox::Yes);
        return;
    }
    int intNumber;
    double doubleDataStart, doubleDataSetp;
    intNumber = number.toInt();
    doubleDataStart = dataStart.toDouble();
    doubleDataSetp = dataSetp.toDouble();
    if(ui->radioBtnRise->isChecked())
        isRise = true;
    else
        isRise = false;
    emit returnParams(intNumber, doubleDataStart, doubleDataSetp, addrStart, isRise);

    this->close();
}
// ����
void BatchAdd::on_pushBtnReset_clicked()
{
    ui->lineEditNum->clear();
    ui->lineEditStartData->clear();
    ui->lineEditDataStep->clear();
    ui->lineEditStartAddr->clear();
    ui->radioBtnRise->setChecked(true);
}
