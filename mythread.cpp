#include "mythread.h"
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <windows.h>
#include <QMessageBox>
#include <math.h>
#include "testitem.h"
#include "currentitem.h"
#include "command.h"
#include "qstringinthex.h"

myThread::myThread(QTcpSocket * meterSocket, QTcpSocket *zynqSocket, QString logFile, QString csvFile, QObject *parent) :
    meter(meterSocket),
    zynq(zynqSocket),
    log(logFile),
    csv(csvFile),
    QThread(parent)
{
    meterTimeOut = 1000;  // ����
    zynqTimeOut = 3000;  // ����
}
// ��¼Log
void myThread::writeLog(QString message)
{
    QDateTime local(QDateTime::currentDateTime());
    QString timeStamp = local.toString("yyyy-MM-dd hh:mm:ss.zzz  ");
    QFile f(log);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        qDebug() << tr("log �ļ���ʧ��!");
        return;
    }
    QTextStream txtOutput(&f);
    emit statusBarShow(message.simplified());
    if(message == "\n")
        txtOutput << endl;
    else
        txtOutput << timeStamp << message << endl;
    f.close();
}
// д������
void myThread::writeCsv(QString data)
{
    QFile f(csv);
    if(!f.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
        qDebug() << tr("�����ļ���ʧ��!");
        return;
    }
    QTextStream textStream;
    textStream.setDevice(&f);
    textStream.setCodec("utf-8");
    textStream << data +"\n";
    f.close();
}
// ���������ñ�����Ϣ
bool myThread::sendMeter(QString message)
{
    writeLog(tr("�����ñ���: ") + message);
    message += "\n";
    int sendRc = meter->write((const char *)message.toUtf8());
    if(sendRc == -1){
        writeLog(tr("���ñ�������ʧ�ܣ�"));
        return false;
    } else
        return true;
}
// ������ZYNQ������Ϣ
bool myThread::sendZynq(QString message)
{
    writeLog(tr("��ZYNQ����: ") + message);
    message += "\n";
    int sendRc = zynq->write((const char *)message.toUtf8());
    if(sendRc == -1){
        writeLog(tr("ZYNQ��������ʧ�ܣ�"));
        return false;
    } else
        return true;
}
// ���俪ʼ�����
void myThread::paragraph(QString para)
{
    if(para == "start")
        writeLog(tr("////////////////////////////////////////"));
    else if (para == "end")
        writeLog(tr("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"));
}
// �����ñ��ȡ��Ϣ
bool myThread::recvMeter()
{
    qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch(); // ��ȡ��ǰʱ��, ����
    while(1){
        Sleep(10);
        if(meter->bytesAvailable() > 0)
            break;
        qint64 end = QDateTime::currentDateTime().toMSecsSinceEpoch(); // ��ȡ��ǰʱ��, ����
        if((end-start) >= meterTimeOut){
            writeLog(tr("��ȡ���ñ�ʱ"));
            return false;
        }
    }
    meterMessage = meter->readAll();
    meterMessage = meterMessage.simplified();
    writeLog(tr("�����ñ����: ") + meterMessage);
    return true;
}
// ��ZYNQ��ȡ��Ϣ
bool myThread::recvZynq()
{
    qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch(); // ��ȡ��ǰʱ��, ����
    while(1){
        Sleep(10);
        if(zynq->bytesAvailable() > 0)
            break;
        qint64 end = QDateTime::currentDateTime().toMSecsSinceEpoch(); // ��ȡ��ǰʱ��, ����
        if((end-start) >= zynqTimeOut){
            writeLog(tr("��ȡZYNQ��ʱ"));
            return false;
        }
    }
    zynqMessage = zynq->readAll();
    zynqMessage = zynqMessage.simplified();
    writeLog(tr("��ZYNQ����: ") + zynqMessage);
    return true;
}

void myThread::run()
{
}
verifyVoltageThread::verifyVoltageThread(testItem * ch, QTcpSocket * meterSocket,
                                         QTcpSocket *zynqSocket, QString logFile,
                                         QString csvFile, QObject *parent):
    myThread(meterSocket, zynqSocket, logFile, csvFile, parent)
{
    qDebug() << "logFile: " << logFile;
    cmdList = ch->getCmdList();
    dataList = ch->getDataList();
    setCmdVerify = ch->getSetCmdVerify();
    setMulti = ch->getSetMulti();
    dmmCmdVerify = ch->getDmmCmdVerify();
    dmmMulti = ch->getDmmMulti();
    meterCmdVerify = ch->getMeterCmdVerify();
    meterMulti = ch->getMeterMulti();
    setCmdTest = ch->getSetCmdTest();
    dmmCmdTest = ch->getDmmCmdTest();
    meterCmdTest = ch->getMeterCmdTest();

    cmdDelay = 100;  // ����
}
void verifyVoltageThread::run()
{
    qDebug() << tr("��ʼУ׼��ѹ");

    // �������ñ�ΪԶ�̵�ѹģʽ
    paragraph("start");
    bool judge = true;
    if(!sendMeter("SYSTEM:REMOTE"))
        judge = false;
    else
        recvMeter();
    writeLog("\n");
    if(!sendMeter(":CONF:VOLT:DC"))
        judge = false;
    else
        recvMeter();
    paragraph("end");
    if(!judge){
        writeLog(tr("�������ñ�ʧ��\n"));
    } else{
        writeLog(tr("�������ñ�ɹ�\n"));
    }

    // ��ʼ��ch
    judge = true;
    int cmdIndex = 0;
    paragraph("start");
    QString message;
    for(int i = 0; i != cmdList->size(); ++i){
        command * cmd = cmdList->at(i);
        message = QString("[%1]").arg(++cmdIndex) + cmd->getFullName();
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        if(cmd->getStart().isEmpty() || cmd->getEnd().isEmpty() || cmd->getJudge().isEmpty()){
            writeLog(tr("������ȱ�ٽ�ȡ�������жϽ������"));
        }else
            judge = cmd->equalJudge();
        if(i != cmdList->size()-1)
            writeLog("\n");
    }
    paragraph("end");
    if(!judge){
        writeLog(tr("��ʼ��CHʧ��\n"));
    }else{
        writeLog(tr("��ʼ��CH�ɹ�\n"));
    }

    // ��ʼУ׼
    QList<QPair<QString, QString> *> * datas = new QList<QPair<QString, QString> *>;
    for(int i = 0; i != dataList->size(); ++i){  // ��������
        if(dataList->at(i)->first){
            datas->append(dataList->at(i)->second);
        }
    }
    if(dmmCmdVerify->getStart().isEmpty() || dmmCmdVerify->getEnd().isEmpty() || dmmCmdVerify->getRatio().toFloat() == 0){
        writeLog(tr("��ѹ��ȡ����ȱ�ٽ�ȡ����������"));
        return;
    }
    if(meterCmdVerify->getRatio().toFloat() == 0){
        writeLog(tr("���ñ��ȡ����ȱ�ٲ����"));
        return;
    }
    QStringList title;
    title << "dac" << "addr" << "adc" << "addr" << "ref" << "addr" << "adc-dac" << "adc-ref";
    writeCsv(title.join(","));
    emit setProgressMaxSize(datas->size());
    for(int i = 0; i != datas->size(); ++i){
        paragraph("start");
        judge = true;
        // ���õ�ѹ
        int dacSet = datas->at(i)->first.toInt();
        message = QString("[%1]%2(%3)").arg(++cmdIndex).arg(setCmdVerify->getName()).arg(dacSet);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        int dac = int(dacSet * setMulti);
        Sleep(cmdDelay);
        // ��ȡ��ѹ
        message = QString("[%1]%2()").arg(++cmdIndex).arg(dmmCmdVerify->getName());
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        dmmCmdVerify->setResult(zynqMessage);
        judge = dmmCmdVerify->judgeRatio(float(dacSet), float(dacSet));
        float adc = dmmCmdVerify->getFloatResult().toFloat() * dmmMulti;
        // ��ȡ���ñ�
        message = meterCmdVerify->getName();
        if(!sendMeter(message))
            judge = false;
        else
            recvMeter();
        meterCmdVerify->setFloatResult(meterMessage.toFloat());
        judge = meterCmdVerify->judgeRatio(dmmCmdVerify->getFloatResult().toFloat(), float(dacSet));
        float ref = meterMessage.toFloat() * meterMulti;
        // д��eeprom
        QString address = datas->at(i)->second;
        int addr;
        if(!QStringIsInt(address)){
            writeLog(QString(tr("���ݵ�ַ%1��Ч")).arg(address));
            return;
        }else
            addr = QString2int(address);
        QString dacStr = QString("%1").arg(dac);
        while(dacStr.length() < 6)
            dacStr = "0" + dacStr;
        QString adcStr = QString("%1").arg(int(adc));
        while(adcStr.length() < 6)
            adcStr = "0" + adcStr;
        QString refStr = QString("%1").arg(int(ref));
        while(refStr.length() < 6)
            refStr = "0" + refStr;
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(int2hexString(addr)).arg(dacStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(int2hexString(addr+6)).arg(adcStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(int2hexString(addr+12)).arg(refStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        // �������ݵ�csv
        QStringList csvline;
        csvline << QString("%1").arg(dacSet)
                << QString("%1").arg(addr)
                << QString("%1").arg(adc)
                << QString("%1").arg(addr+6)
                << QString("%1").arg(ref)
                << QString("%1").arg(addr+12)
                << QString("%1").arg(adc-dac)
                << QString("%1").arg(adc-ref);
        writeCsv(csvline.join(","));
        // ����ʾ
        QString passOrFail;
        if(judge)
            passOrFail = "pass";
        else
            passOrFail = "fail";
        QStringList tableline = csvline;
        tableline << passOrFail;
        tableline.insert(tableline.begin(), QString("set valtage %1").arg(dacSet));
        emit showTable(tableline);
        emit setProgressCurSize(i+1);
        paragraph("end");
        writeLog("\n");
    }
    statusBarShow(tr("��ѹУ׼���"));
}
