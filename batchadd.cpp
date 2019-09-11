#include "batchadd.h"
#include "ui_batchadd.h"
#include <QMessageBox>
#include <QDebug>
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
// 生成
void BatchAdd::on_pushBtnGenerate_clicked()
{
    QString number, dataStart, dataSetp, addrStart, addrStep;
    bool isRise;
    number = ui->lineEditNum->text();
    dataStart = ui->lineEditStartData->text();
    dataSetp = ui->lineEditDataStep->text();
    addrStart = ui->lineEditStartAddr->text();
    addrStep = ui->lineEditAddrStep->text();

    if(number.size() == 0 || dataStart.size() == 0 || dataSetp.size() == 0 || addrStart.size() ==0 || addrStep.size() == 0){
        QMessageBox::information(this, QString(tr("生成失败")), QString(tr("参数未填写完整，请重新填写")),QMessageBox::Yes);
        return;
    }else if(!QStringIsInt(addrStep)){
        QMessageBox::information(this, QString(tr("生成失败")), QString(tr("地址步长不是有效的十进制或十六进制数，请重新填写")),QMessageBox::Yes);
        return;
    }else if(!QStringIsInt(addrStart)){
        QMessageBox::information(this, QString(tr("生成失败")), QString(tr("起始地址不是有效的十进制或十六进制数，请重新填写")),QMessageBox::Yes);
        return;
    }else if(number == 0){
        QMessageBox::information(this, QString(tr("生成失败")), QString(tr("数据个数不能为0，请重新填写")),QMessageBox::Yes);
        return;
    }
    int intNumber, intAddrStep;
    double doubleDataStart, doubleDataSetp;
    intNumber = number.toInt();
    doubleDataStart = dataStart.toDouble();
    doubleDataSetp = dataSetp.toDouble();
    intAddrStep = QString2int(addrStep);
    if(ui->radioBtnRise->isChecked())
        isRise = true;
    else
        isRise = false;
    emit returnParams(intNumber, doubleDataStart, doubleDataSetp, addrStart, intAddrStep, isRise);

    this->close();
}
// 重置
void BatchAdd::on_pushBtnReset_clicked()
{
    ui->lineEditNum->clear();
    ui->lineEditStartData->clear();
    ui->lineEditDataStep->clear();
    ui->lineEditStartAddr->clear();
    ui->lineEditAddrStep->clear();
    ui->radioBtnRise->setChecked(true);
}
