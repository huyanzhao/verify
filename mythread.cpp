#include "mythread.h"
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include <windows.h>
#include <QMessageBox>
#include "testitem.h"
#include "currentitem.h"
#include "command.h"

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
    if(!f.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
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
    emit setProgressMaxSize(cmdList->size());
}
void verifyVoltageThread::run()
{
    qDebug() << tr("��ʼУ׼��ѹ");

    // �������ñ�ΪԶ�̵�ѹģʽ
    paragraph("start");
    bool judge = true;
    if(!sendMeter("SYSTEM:REMOTE"))
        judge = false;
    else{
        if(recvMeter())
            qDebug() << "meter message: " << meterMessage;
        else
            qDebug() << tr("��ȡ���ñ�ʧ��");
    }
    writeLog("\n");
    if(!sendMeter(":CONF:VOLT:DC"))
        judge = false;
    else{
        if(recvMeter())
            qDebug() << "meter message: " << meterMessage;
        else
            qDebug() << tr("��ȡ���ñ�ʧ��");
    }
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
        else{
            if(recvZynq())
                qDebug() << "zynqMessage: " << zynqMessage;
            else
                qDebug() << tr("��ȡZYNQʧ��");
        }
        QString result;
        if(!cmd->getStart().isEmpty() && !cmd->getEnd().isEmpty()){
            writeLog(QString(tr("start= %1, end= %2")).arg(cmd->getStart()).arg(cmd->getEnd()));
            int start = zynqMessage.indexOf(cmd->getStart());
            int end = zynqMessage.indexOf(cmd->getEnd());
            writeLog(QString(tr("startIndex= %1, endIndex= %2")).arg(start).arg(end));
            if(start == -1 || end == -1){
                writeLog(tr("��ȡʧ��"));
                judge = false;
            }else{
                start = start + cmd->getStart().size();
                result = zynqMessage.mid(start, end-start);
                writeLog(QString(tr("result= %1")).arg(result));
                writeLog(QString(tr("Judge= %1")).arg(cmd->getJudge()));
                if(!result.compare(cmd->getJudge())){
                    writeLog(tr("�����ƥ��"));
                    judge = false;
                }
            }
        }else{
            writeLog(tr("������û��start��end��ȡ����"));
        }
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
    for(int i = 0; i != datas->size(); ++i){
        paragraph("start");
        QString cmd = setCmdVerify->getName();
        message = QString("[%1]").arg(++cmdIndex) + cmd + QString("(%1)").arg(datas->at(i)->first);
        if(!sendZynq(message))
            judge = false;
        else{
            if(recvZynq())
                qDebug() << "zynqMessage: " << zynqMessage;
            else
                qDebug() << tr("��ȡZYNQʧ��");
        }



        paragraph("end");
    }
}
