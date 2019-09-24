#include "mythread.h"
#include <QTcpSocket>
#include <QDebug>
#include <QFile>
#include <QDateTime>
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
    connect(meter, SIGNAL(readyRead()), this, SLOT(readMeterMessage()));
    meterBlockSize = 0;  // ��ʼ�����ݴ�С��ϢΪ0
    connect(zynq, SIGNAL(readyRead()), this, SLOT(readZynqMessage()));
    zynqBlockSize = 0;  // ��ʼ�����ݴ�С��ϢΪ0
}
// �������ñ���Ϣ
void myThread::readMeterMessage()
{
    QDataStream in(meter);
    // �����������汾������Ҫ�ͷ���������ͬ
    in.setVersion(QDataStream::Qt_4_6);
    // ����Ǹտ�ʼ��������
    if (meterBlockSize == 0) {
        //�жϽ��յ������Ƿ�������ֽڣ�Ҳ�����ļ��Ĵ�С��Ϣ��ռ�Ŀռ�
        //������򱣴浽blockSize�����У�����ֱ�ӷ��أ�������������
        if(meter->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> meterBlockSize;
    }
    // ���û�еõ�ȫ�������ݣ��򷵻أ�������������
    if(meter->bytesAvailable() < meterBlockSize) return;
    // �����յ������ݴ�ŵ�������
    in >> meterMessage;
    // ��ʾ���յ�������
    qDebug() << tr("���ñ�ظ�: ") << meterMessage;
}
// ����ZYNQ��Ϣ
void myThread::readZynqMessage()
{
    QDataStream in(zynq);
    // �����������汾������Ҫ�ͷ���������ͬ
    in.setVersion(QDataStream::Qt_4_6);
    // ����Ǹտ�ʼ��������
    if (zynqBlockSize == 0) {
        //�жϽ��յ������Ƿ�������ֽڣ�Ҳ�����ļ��Ĵ�С��Ϣ��ռ�Ŀռ�
        //������򱣴浽blockSize�����У�����ֱ�ӷ��أ�������������
        if(zynq->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> zynqBlockSize;
    }
    // ���û�еõ�ȫ�������ݣ��򷵻أ�������������
    if(zynq->bytesAvailable() < zynqBlockSize) return;
    // �����յ������ݴ�ŵ�������
    in >> zynqMessage;
    // ��ʾ���յ�������
    qDebug() << tr("ZYNQ�ظ�: ") << zynqMessage;
}
// ��¼Log
void myThread::writeLog(QString logFile, QString message)
{
    QDateTime local(QDateTime::currentDateTime());
    QString timeStamp = local.toString("yyyy-MM-dd hh:mm:ss.zzz  ");
    QFile f(logFile);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << tr("log �ļ���ʧ��!");
//        QMessageBox::warning(this, tr("���ļ�ʧ��"), QString("%1 ��ʧ��!").arg(logFile), QMessageBox::Ok);
        return;
    }
    QTextStream txtOutput(&f);
    if(message == "\n")
        txtOutput << endl;
    else
        txtOutput << timeStamp << message << endl;
    f.close();
}
// д������
void myThread::writeCsv(QString csvFile, QString data)
{
    QFile f(csvFile);
    if(!f.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
        qDebug() << tr("�����ļ���ʧ��!");
//        QMessageBox::warning(this, tr("���ļ�ʧ��"), QString(tr("%1 ��ʧ��!")).arg(csvFile), QMessageBox::Ok);
        return;
    }
    QTextStream textStream;
    textStream.setDevice(&f);
    textStream.setCodec("utf-8");
    textStream << data +"\n";
    f.close();
}
// ���͸����ñ�
bool myThread::sendMeter(QString * message)
{
    int sendRc = meter->write((const char *)message);
    if(sendRc == -1){
        qDebug () << tr("���ñ�������ʧ�ܣ�");
        return false;
    } else
        return true;
}
// ���͸�ZYNQ
bool myThread::sendZynq(QString * message)
{
    int sendRc = zynq->write((const char *)message);
    if(sendRc == -1){
        qDebug () << tr("ZYNQ��������ʧ�ܣ�");
        return false;
    } else
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
    qDebug() << tr("У׼��ѹ�̴߳����ɹ�!");
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
    bool judge = true;
    QString message = "SYSTEM:REMOTE";
    if(!sendMeter(&message))
        judge = false;
    message = ":CONF:VOLT:DC";
    if(!sendMeter(&message))
        judge = false;
    if(!judge){
        writeLog(log, QString("�������ñ�ʧ��"));
        emit statusBarShow(tr("�������ñ�ʧ��"));
    } else{
        writeLog(log, QString("�������ñ�ɹ�"));
        emit statusBarShow(tr("�������ñ�ɹ�"));
    }
    int cmdIndex = 0;
    // ��ʼ��ch
    judge = true;
    for(int i = 0; i != cmdList->size(); ++i){
        command * cmd = cmdList->at(i);
        writeLog(log, QString("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"));
        message = cmd->getFullName();
        message = QString("[%1]").arg(++cmdIndex) + message;
        writeLog(log, QString("send: ") + message);
        emit statusBarShow(QString(tr("��ZYNQ����: ")) + message);
        if(!sendZynq(&message))
            judge = false;
        else{
            qDebug() << "zynaMessage: " << zynqMessage;
            writeLog(log, QString("recv: ") + zynqMessage);
            emit statusBarShow(QString(tr("��ZYNQ����: ")) + zynqMessage);
        }
        QString result;
        if(!cmd->getStart().isEmpty() && !cmd->getEnd().isEmpty()){
            int start = zynqMessage.indexOf(cmd->getStart());
            int end = zynqMessage.indexOf(cmd->getEnd());
            if(start == -1 || end == -1)
                judge = false;
            else{
                start = start + cmd->getStart().size();
                result = zynqMessage.mid(start, end-start);
                if(!result.compare(cmd->getJudge()))
                    judge = false;
            }
        }
        writeLog(log, QString("��������������������������������������������������������������������������������\n"));
    }
    if(!judge){
        writeLog(log, QString("��ʼ��CHʧ��"));
        emit statusBarShow(tr("��ʼ��CHʧ��"));
    }else{
        writeLog(log, QString("��ʼ��CH�ɹ�"));
        emit statusBarShow(tr("��ʼ��CH�ɹ�"));
    }
}
