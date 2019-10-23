#include "mythread.h"
#include <QTcpSocket>
#include <QFile>
#include <QDateTime>
#include <windows.h>
//#include <sys/stat.h>
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


    rx.setPattern("(\\.){0,1}0+$");
}
// ��¼Log
void myThread::writeLog(QString message)
{
    QDateTime local(QDateTime::currentDateTime());
    QString timeStamp = local.toString("yyyy-MM-dd hh:mm:ss.zzz  ");
    QFile f(log);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;
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
    if(!f.open(QFile::WriteOnly | QFile::Text | QFile::Append))
        return;
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
        writeLog(tr("////////////////////////////////////////////////////////////////////////////////"));
    else if (para == "end")
        writeLog(tr("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"));
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
    cmdList = ch->getCmdList();
    dataList = ch->getDataList();
    setCmdVerify = ch->getSetCmdVerify();
    setMulti = ch->getSetMulti();
    dmmCmdVerify = ch->getDmmCmdVerify();
    dmmMulti = ch->getDmmMulti();
    meterCmdVerify = ch->getMeterCmdVerify();
    meterMulti = ch->getMeterMulti();

    cmdDelay = 100;  // ����

    dataLength = ch->getDataLength();
}
void verifyVoltageThread::run()
{
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
    QList<DataItem *> * datas = new QList<DataItem *>;
    for(int i = 0; i != dataList->size(); ++i){  // ��������
        if(dataList->at(i)->check){
            datas->append(dataList->at(i));
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
    title << "set" << "addr" << "dmm" << "addr" << "meter" << "addr" << "dmm-set" << "dmm-meter";
    writeCsv(title.join(","));
    emit setProgressMaxSize(datas->size());
    for(int i = 0; i != datas->size(); ++i){
        paragraph("start");
        judge = true;
        // ���õ�ѹ
        double set = datas->at(i)->data;
        message = QString("[%1]%2(%3)").arg(++cmdIndex).arg(setCmdVerify->getName()).arg(set);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        int dac = int(set * setMulti);
        Sleep(cmdDelay);
        // ��ȡ��ѹ
        message = QString("[%1]%2()").arg(++cmdIndex).arg(dmmCmdVerify->getName());
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        dmmCmdVerify->setResult(zynqMessage);
        judge = dmmCmdVerify->judgeRatio(float(set), float(set));
        float dmm = dmmCmdVerify->getFloatResult().toFloat() * dmmMulti;
        // ��ȡ���ñ�
        message = meterCmdVerify->getName();
        if(!sendMeter(message))
            judge = false;
        else
            recvMeter();
        meterCmdVerify->setFloatResult(meterMessage.toFloat());
        judge = meterCmdVerify->judgeRatio(dmmCmdVerify->getFloatResult().toFloat(), float(set));
        float meter = meterMessage.toFloat() * meterMulti;
        // д��eeprom
        QString dacStr = QString("%1").arg(dac);
        while(dacStr.length() < dataLength)
            dacStr = "0" + dacStr;
        QString dmmStr = QString("%1").arg(int(dmm));
        while(dmmStr.length() < dataLength)
            dmmStr = "0" + dmmStr;
        QString meterStr = QString("%1").arg(int(meter));
        while(meterStr.length() < dataLength)
            meterStr = "0" + meterStr;
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(datas->at(i)->dacAddr).arg(dacStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(datas->at(i)->adcAddr).arg(dmmStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(datas->at(i)->refAddr).arg(meterStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        // �������ݵ�csv
        QStringList csvline;
        csvline << QString("%1").arg(set,0,'f',-1).replace(rx,"")
                << QString("%1").arg(datas->at(i)->dacAddr)
                << QString("%1").arg(dmm,0,'f',-1).replace(rx,"")
                << QString("%1").arg(datas->at(i)->adcAddr)
                << QString("%1").arg(meter,0,'f',-1).replace(rx,"")
                << QString("%1").arg(datas->at(i)->refAddr)
                << QString("%1").arg(set-dmm/dmmMulti,0,'f',-1).replace(rx,"")
                << QString("%1").arg(meter/meterMulti-dmm/dmmMulti,0,'f',-1).replace(rx,"");
        writeCsv(csvline.join(","));
        // ����ʾ
        QString passOrFail;
        if(judge)
            passOrFail = "pass";
        else
            passOrFail = "fail";
        QStringList tableline = csvline;
        tableline << passOrFail;
        tableline.insert(tableline.begin(), QString("set valtage %1").arg(set,0,'f',-1).replace(rx,""));
        emit showTable(tableline);
        emit setProgressCurSize(i+1);
        paragraph("end");
        writeLog("\n");
    }
    statusBarShow(tr("��ѹУ׼���"));
}

testVoltageThread::testVoltageThread(testItem * ch, QTcpSocket * meterSocket,
                                         QTcpSocket *zynqSocket, QString logFile,
                                         QString csvFile, QObject *parent):
    myThread(meterSocket, zynqSocket, logFile, csvFile, parent)
{
    cmdList = ch->getCmdList();
    dataList = ch->getDataList();
    setCmdTest = ch->getSetCmdTest();
    dmmCmdTest = ch->getDmmCmdTest();
    meterCmdTest = ch->getMeterCmdTest();

    cmdDelay = 100;  // ����
}
void testVoltageThread::run()
{
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

    // ��ʼ����
    QList<DataItem *> * datas = new QList<DataItem *>;
    for(int i = 0; i != dataList->size(); ++i){  // ��������
        if(dataList->at(i)->check){
            datas->append(dataList->at(i));
        }
    }
    if(dmmCmdTest->getStart().isEmpty() || dmmCmdTest->getEnd().isEmpty() || dmmCmdTest->getRatio().toFloat() == 0){
        writeLog(tr("��ѹ��ȡ����ȱ�ٽ�ȡ����������"));
        return;
    }
    if(meterCmdTest->getRatio().toFloat() == 0){
        writeLog(tr("���ñ��ȡ����ȱ�ٲ����"));
        return;
    }
    QStringList title;
    title << "set" << "dmm" << "meter" << "dmm-set" << "(dmm-set)/set(%)" << "dmm-meter" << "(dmm-meter)/set(%)";
    writeCsv(title.join(","));
    emit setProgressMaxSize(datas->size());
    for(int i = 0; i != datas->size(); ++i){
        paragraph("start");
        judge = true;
        // ���õ�ѹ
        double set = datas->at(i)->data;
        message = QString("[%1]%2(%3)").arg(++cmdIndex).arg(setCmdTest->getName()).arg(set);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        Sleep(cmdDelay);
        // ��ȡ��ѹ
        message = QString("[%1]%2()").arg(++cmdIndex).arg(dmmCmdTest->getName());
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        dmmCmdTest->setResult(zynqMessage);
        judge = dmmCmdTest->judgeRatio(float(set), float(set));
        float dmm = dmmCmdTest->getFloatResult().toFloat();
        // ��ȡ���ñ�
        message = meterCmdTest->getName();
        if(!sendMeter(message))
            judge = false;
        else
            recvMeter();
        meterCmdTest->setFloatResult(meterMessage.toFloat());
        judge = meterCmdTest->judgeRatio(dmmCmdTest->getFloatResult().toFloat(), float(set));
        float meter = meterMessage.toFloat();

        float sub1 = set-dmm;
        float ratio1 = sub1 / set * 100;
        float sub2 = meter-dmm;
        float ratio2 = sub2 / set * 100;

        // �������ݵ�csv
        QStringList csvline;
        csvline << QString("%1").arg(set,0,'f',-1).replace(rx,"")
                << QString("%1").arg(dmm,0,'f',-1).replace(rx,"")
                << QString("%1").arg(meter,0,'f',-1).replace(rx,"")
                << QString("%1").arg(sub1,0,'f',-1).replace(rx,"")
                << QString("%1").arg(ratio1,0,'f',-1).replace(rx,"")+"\%"
                << QString("%1").arg(sub2,0,'f',-1).replace(rx,"")
                << QString("%1").arg(ratio2,0,'f',-1).replace(rx,"")+"\%";
        writeCsv(csvline.join(","));
        // ����ʾ
        QString passOrFail;
        if(judge)
            passOrFail = "pass";
        else
            passOrFail = "fail";
        QStringList tableline = csvline;
        tableline << passOrFail;
        tableline.insert(tableline.begin(), QString("set valtage %1").arg(set,0,'f',-1).replace(rx,""));
        emit showTable(tableline);
        emit setProgressCurSize(i+1);
        paragraph("end");
        writeLog("\n");
    }
    statusBarShow(tr("��ѹ�������"));
}
verifyCurrentThread::verifyCurrentThread(currentItem * psuParam, QList<int> * partListParam, QString StrParam,
                                         QTcpSocket * meterSocket, QTcpSocket *zynqSocket,
                                         QString logFile, QString csvFile, QObject *parent):
    myThread(meterSocket, zynqSocket, logFile, csvFile, parent)
{
    psu = psuParam;
    partList = partListParam;
    Str = StrParam;
    cmdDelay = 100;  // ����
}
void verifyCurrentThread::run()
{
    // �������ñ�ΪԶ�̵���ģʽ
    paragraph("start");
    bool judge = true;
    if(!sendMeter("SYSTEM:REMOTE"))
        judge = false;
    else
        recvMeter();
    writeLog("\n");
    if(!sendMeter(":CONF:CURR:DC"))
        judge = false;
    else
        recvMeter();
    paragraph("end");
    if(!judge){
        writeLog(tr("�������ñ�ʧ��\n"));
    } else{
        writeLog(tr("�������ñ�ɹ�\n"));
    }

    // ��ʼ��psu
    judge = true;
    int cmdIndex = 0;
    paragraph("start");
    QString message;
    QList<command *> * cmdList = psu->getPreCmdList();
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
        writeLog(tr("��ʼ��PSUʧ��\n"));
    }else{
        writeLog(tr("��ʼ��PSU�ɹ�\n"));
    }
    int sum = 0;
    for(int i=0; i != partList->size(); ++i){
        if(partList->at(i) == 1 && psu->getPart1())
            sum += psu->getPart1()->getDataList()->size();
        if(partList->at(i) == 2 && psu->getPart2())
            sum += psu->getPart2()->getDataList()->size();
        if(partList->at(i) == 3 && psu->getPart3())
            sum += psu->getPart3()->getDataList()->size();
        if(partList->at(i) == 4 && psu->getPart4())
            sum += psu->getPart4()->getDataList()->size();
        if(partList->at(i) == 5 && psu->getPart5())
            sum += psu->getPart5()->getDataList()->size();
    }
    int indexTable = 0;
    emit setProgressMaxSize(sum);
    for(int i=0; i != partList->size(); ++i){
        testItem * part;
        switch(partList->at(i)){
        case 1:
            part = psu->getPart1();
            break;
        case 2:
            part = psu->getPart2();
            break;
        case 3:
            part = psu->getPart3();
            break;
        case 4:
            part = psu->getPart4();
            break;
        case 5:
            part = psu->getPart5();
            break;
        default:
            statusBarShow(tr("part���������˳�У׼"));
            return;
        }
        if(part == NULL){
            statusBarShow(tr("û��part%1�����ݣ�������һ��").arg(partList->at(i)));
            continue;
        }
        // �л���λ
        judge = true;
        paragraph("start");
        QList<command *> * cmdList = part->getCmdList();
        for(int j = 0; j != cmdList->size(); ++j){
            command * cmd = cmdList->at(j);
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
            writeLog(tr("��λ�л�ʧ��\n"));
        }else{
            writeLog(tr("��λ�л��ɹ�\n"));
        }
        // ��ʼУ׼
        QList<DataItem *> * datas = new QList<DataItem *>;
        QList<DataItem *> * dataList = part->getDataList();
        for(int k = 0; k != dataList->size(); ++k){  // ��������
            if(dataList->at(k)->check){
                datas->append(dataList->at(k));
            }
        }
        command * dmmCmdVerify = part->getDmmCmdVerify();
        if(dmmCmdVerify->getStart().isEmpty() || dmmCmdVerify->getEnd().isEmpty() || dmmCmdVerify->getRatio().toFloat() == 0){
            writeLog(tr("��ѹ��ȡ����ȱ�ٽ�ȡ����������"));
            return;
        }
        command * meterCmdVerify = part->getMeterCmdVerify();
        if(meterCmdVerify->getRatio().toFloat() == 0){
            writeLog(tr("���ñ��ȡ����ȱ�ٲ����"));
            return;
        }
        writeCsv(QString("Part%1").arg(partList->at(i)));
        QStringList title;
        title << "set" << "addr" << "dmm" << "addr" << "meter" << "addr" << "dmm-set" << "dmm-meter";
        writeCsv(title.join(","));
        command * setCmdVerify = part->getSetCmdVerify();
        int setMulti = part->getSetMulti();
        int dmmMulti = part->getDmmMulti();
        int meterMulti = part->getMeterMulti();
        dataLength = part->getDataLength();
        for(int l = 0; l != datas->size(); ++l){
            paragraph("start");
            judge = true;
            // ���õ���
            double set = datas->at(l)->data;
            message = QString("[%1]%2(%3)").arg(++cmdIndex).arg(setCmdVerify->getName()).arg(set);
            if(!sendZynq(message))
                judge = false;
            else
                recvZynq();
            int dac = int(set * setMulti);
            Sleep(cmdDelay);
            // ��ȡ��ѹ
            message = QString("[%1]%2()").arg(++cmdIndex).arg(dmmCmdVerify->getName());
            if(!sendZynq(message))
                judge = false;
            else
                recvZynq();
            dmmCmdVerify->setResult(zynqMessage);
            judge = dmmCmdVerify->judgeRatio(float(set), float(set));
            float dmm = dmmCmdVerify->getFloatResult().toFloat() * dmmMulti;
            // ��ȡ���ñ�
            message = meterCmdVerify->getName();
            if(!sendMeter(message))
                judge = false;
            else
                recvMeter();
            meterCmdVerify->setFloatResult(meterMessage.toFloat());
            judge = meterCmdVerify->judgeRatio(dmmCmdVerify->getFloatResult().toFloat(), float(set));
            float meter = meterMessage.toFloat() * meterMulti;
            // д��eeprom
            QString dacStr = QString("%1").arg(dac);
            while(dacStr.length() < dataLength)
                dacStr = "0" + dacStr;
            QString dmmStr = QString("%1").arg(int(dmm));
            while(dmmStr.length() < dataLength)
                dmmStr = "0" + dmmStr;
            QString meterStr = QString("%1").arg(int(meter));
            while(meterStr.length() < dataLength)
                meterStr = "0" + meterStr;
            message = QString("[%1]eeprom write string(%2, at16, %3, %4)").arg(++cmdIndex).arg(Str).arg(datas->at(l)->dacAddr).arg(dacStr);
            if(!sendZynq(message))
                judge = false;
            else
                recvZynq();
            message = QString("[%1]eeprom write string(%2, at16, %3, %4)").arg(++cmdIndex).arg(Str).arg(datas->at(l)->adcAddr).arg(dmmStr);
            if(!sendZynq(message))
                judge = false;
            else
                recvZynq();
            message = QString("[%1]eeprom write string(%2, at16, %3, %4)").arg(++cmdIndex).arg(Str).arg(datas->at(l)->refAddr).arg(meterStr);
            if(!sendZynq(message))
                judge = false;
            else
                recvZynq();
            // �������ݵ�csv
            QStringList csvline;
            csvline << QString("%1").arg(set,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(datas->at(l)->dacAddr)
                    << QString("%1").arg(dmm,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(datas->at(l)->adcAddr)
                    << QString("%1").arg(meter,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(datas->at(l)->refAddr)
                    << QString("%1").arg(set-dmm/dmmMulti,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(meter/meterMulti-dmm/dmmMulti,0,'f',-1).replace(rx,"");
            writeCsv(csvline.join(","));
            // ����ʾ
            QString passOrFail;
            if(judge)
                passOrFail = "pass";
            else
                passOrFail = "fail";
            QStringList tableline = csvline;
            tableline << passOrFail;
            tableline.insert(tableline.begin(), QString("set part%1 current %2").arg(partList->at(i)).arg(set,0,'f',-1).replace(rx,""));
            emit showTable(tableline);
            emit setProgressCurSize(++indexTable);
            paragraph("end");
            writeLog("\n");
        }
        writeCsv("\n");
    }
    statusBarShow(tr("����У׼���"));
}
testCurrentThread::testCurrentThread(currentItem *psuParam, QList<int> * partListParam, QString StrParam,
                                     QTcpSocket * meterSocket, QTcpSocket * zynqSocket,
                                     QString logFile, QString csvFile, QObject *parent):
    myThread(meterSocket, zynqSocket, logFile, csvFile, parent)
{
    psu = psuParam;
    partList = partListParam;
    Str = StrParam;
    cmdDelay = 100;  // ����
}
void testCurrentThread::run()
{
    // �������ñ�ΪԶ�̵���ģʽ
    paragraph("start");
    bool judge = true;
    if(!sendMeter("SYSTEM:REMOTE"))
        judge = false;
    else
        recvMeter();
    writeLog("\n");
    if(!sendMeter(":CONF:CURR:DC"))
        judge = false;
    else
        recvMeter();
    paragraph("end");
    if(!judge){
        writeLog(tr("�������ñ�ʧ��\n"));
    } else{
        writeLog(tr("�������ñ�ɹ�\n"));
    }

    // ��ʼ��psu
    judge = true;
    int cmdIndex = 0;
    paragraph("start");
    QString message;
    QList<command *> * cmdList = psu->getPreCmdList();
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
        writeLog(tr("��ʼ��PSUʧ��\n"));
    }else{
        writeLog(tr("��ʼ��PSU�ɹ�\n"));
    }
    int sum = 0;
    for(int i=0; i != partList->size(); ++i){  // ����õ��������������������ý�����
        if(partList->at(i) == 1 && psu->getPart1())
            sum += psu->getPart1()->getDataList()->size();
        if(partList->at(i) == 2 && psu->getPart2())
            sum += psu->getPart2()->getDataList()->size();
        if(partList->at(i) == 3 && psu->getPart3())
            sum += psu->getPart3()->getDataList()->size();
        if(partList->at(i) == 4 && psu->getPart4())
            sum += psu->getPart4()->getDataList()->size();
        if(partList->at(i) == 5 && psu->getPart5())
            sum += psu->getPart5()->getDataList()->size();
    }
    int indexTable = 0;
    emit setProgressMaxSize(sum);
    for(int i=0; i != partList->size(); ++i){
        testItem * part;
        switch(partList->at(i)){
        case 1:
            part = psu->getPart1();
            break;
        case 2:
            part = psu->getPart2();
            break;
        case 3:
            part = psu->getPart3();
            break;
        case 4:
            part = psu->getPart4();
            break;
        case 5:
            part = psu->getPart5();
            break;
        default:
            statusBarShow(tr("part���������˳�У׼"));
            return;
        }
        if(part == NULL){
            statusBarShow(tr("û��part%1�����ݣ�������һ��").arg(partList->at(i)));
            continue;
        }
        // �л���λ
        judge = true;
        paragraph("start");
        QList<command *> * cmdList = part->getCmdList();
        for(int j = 0; j != cmdList->size(); ++j){
            command * cmd = cmdList->at(j);
            message = QString("[%1]").arg(++cmdIndex) + cmd->getFullName();
            if(!sendZynq(message))
                judge = false;
            else
                recvZynq();
            if(cmd->getStart().isEmpty() || cmd->getEnd().isEmpty() || cmd->getJudge().isEmpty()){
                writeLog(tr("������ȱ�ٽ�ȡ�������жϽ������"));
            }else
                judge = cmd->equalJudge();
            if(j != cmdList->size()-1)
                writeLog("\n");
        }
        paragraph("end");
        if(!judge){
            writeLog(tr("��λ�л�ʧ��\n"));
        }else{
            writeLog(tr("��λ�л��ɹ�\n"));
        }
        // ��ʼ����
        QList<DataItem *> * datas = new QList<DataItem *>;
        QList<DataItem *> * dataList = part->getDataList();
        for(int k = 0; k != dataList->size(); ++k){  // ��������
            if(dataList->at(k)->check){
                datas->append(dataList->at(k));
            }
        }
        command * dmmCmdTest = part->getDmmCmdTest();
        if(dmmCmdTest->getStart().isEmpty() || dmmCmdTest->getEnd().isEmpty() || dmmCmdTest->getRatio().toFloat() == 0){
            writeLog(tr("������ȡ����ȱ�ٽ�ȡ����������"));
            return;
        }
        command * meterCmdTest = part->getMeterCmdTest();
        if(meterCmdTest->getRatio().toFloat() == 0){
            writeLog(tr("���ñ��ȡ����ȱ�ٲ����"));
            return;
        }
        writeCsv(QString("Part%1").arg(partList->at(i)));
        QStringList title;
        title << "set" << "addr" << "dmm" << "addr" << "meter" << "addr" << "dmm-set" << "dmm-meter";
        writeCsv(title.join(","));
        command * setCmdTest = part->getSetCmdTest();
        for(int l = 0; l != datas->size(); ++l){
            paragraph("start");
            judge = true;
            // ���õ���
            double set = datas->at(l)->data;
            message = QString("[%1]%2(%3)").arg(++cmdIndex).arg(setCmdTest->getName()).arg(set);
            if(!sendZynq(message))
                judge = false;
            else
                recvZynq();
            Sleep(cmdDelay);
            // ��ȡ��ѹ
            message = QString("[%1]%2()").arg(++cmdIndex).arg(dmmCmdTest->getName());
            if(!sendZynq(message))
                judge = false;
            else
                recvZynq();
            dmmCmdTest->setResult(zynqMessage);
            judge = dmmCmdTest->judgeRatio(float(set), float(set));
            float dmm = dmmCmdTest->getFloatResult().toFloat();
            // ��ȡ���ñ�
            message = meterCmdTest->getName();
            if(!sendMeter(message))
                judge = false;
            else
                recvMeter();
            meterCmdTest->setFloatResult(meterMessage.toFloat());
            judge = meterCmdTest->judgeRatio(dmmCmdTest->getFloatResult().toFloat(), float(set));
            float meter = meterMessage.toFloat();

            float sub1 = set-dmm;
            float ratio1 = sub1 / set * 100;
            float sub2 = meter-dmm;
            float ratio2 = sub2 / set * 100;

            // �������ݵ�csv
            QStringList csvline;
            csvline << QString("%1").arg(set,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(dmm,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(meter,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(sub1,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(ratio1,0,'f',-1).replace(rx,"")+"\%"
                    << QString("%1").arg(sub2,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(ratio2,0,'f',-1).replace(rx,"")+"\%";
            writeCsv(csvline.join(","));
            // ����ʾ
            QString passOrFail;
            if(judge)
                passOrFail = "pass";
            else
                passOrFail = "fail";
            QStringList tableline = csvline;
            tableline << passOrFail;
            tableline.insert(tableline.begin(), QString("set part%1 current %2").arg(partList->at(i)).arg(set,0,'f',-1).replace(rx,""));
            emit showTable(tableline);
            emit setProgressCurSize(++indexTable);
            paragraph("end");
            writeLog("\n");
        }
        writeCsv("\n");
    }
    statusBarShow(tr("�����������"));
}
