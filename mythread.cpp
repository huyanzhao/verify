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
    meterTimeOut = 1000;  // 毫秒
    zynqTimeOut = 3000;  // 毫秒
}
// 记录Log
void myThread::writeLog(QString message)
{
    QDateTime local(QDateTime::currentDateTime());
    QString timeStamp = local.toString("yyyy-MM-dd hh:mm:ss.zzz  ");
    QFile f(log);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        qDebug() << tr("log 文件打开失败!");
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
// 写入数据
void myThread::writeCsv(QString data)
{
    QFile f(csv);
    if(!f.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
        qDebug() << tr("数据文件打开失败!");
        return;
    }
    QTextStream textStream;
    textStream.setDevice(&f);
    textStream.setCodec("utf-8");
    textStream << data +"\n";
    f.close();
}
// 给发送万用表发送信息
bool myThread::sendMeter(QString message)
{
    writeLog(tr("向万用表发送: ") + message);
    message += "\n";
    int sendRc = meter->write((const char *)message.toUtf8());
    if(sendRc == -1){
        writeLog(tr("万用表发送数据失败！"));
        return false;
    } else
        return true;
}
// 给发送ZYNQ发送信息
bool myThread::sendZynq(QString message)
{
    writeLog(tr("向ZYNQ发送: ") + message);
    message += "\n";
    int sendRc = zynq->write((const char *)message.toUtf8());
    if(sendRc == -1){
        writeLog(tr("ZYNQ发送数据失败！"));
        return false;
    } else
        return true;
}
// 段落开始与结束
void myThread::paragraph(QString para)
{
    if(para == "start")
        writeLog(tr("////////////////////////////////////////"));
    else if (para == "end")
        writeLog(tr("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"));
}
// 从万用表读取信息
bool myThread::recvMeter()
{
    qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch(); // 获取当前时间, 毫秒
    while(1){
        Sleep(10);
        if(meter->bytesAvailable() > 0)
            break;
        qint64 end = QDateTime::currentDateTime().toMSecsSinceEpoch(); // 获取当前时间, 毫秒
        if((end-start) >= meterTimeOut){
            writeLog(tr("读取万用表超时"));
            return false;
        }
    }
    meterMessage = meter->readAll();
    meterMessage = meterMessage.simplified();
    writeLog(tr("从万用表接收: ") + meterMessage);
    return true;
}
// 从ZYNQ读取信息
bool myThread::recvZynq()
{
    qint64 start = QDateTime::currentDateTime().toMSecsSinceEpoch(); // 获取当前时间, 毫秒
    while(1){
        Sleep(10);
        if(zynq->bytesAvailable() > 0)
            break;
        qint64 end = QDateTime::currentDateTime().toMSecsSinceEpoch(); // 获取当前时间, 毫秒
        if((end-start) >= zynqTimeOut){
            writeLog(tr("读取ZYNQ超时"));
            return false;
        }
    }
    zynqMessage = zynq->readAll();
    zynqMessage = zynqMessage.simplified();
    writeLog(tr("从ZYNQ接收: ") + zynqMessage);
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
    qDebug() << tr("开始校准电压");

    // 设置万用表为远程电压模式
    paragraph("start");
    bool judge = true;
    if(!sendMeter("SYSTEM:REMOTE"))
        judge = false;
    else{
        if(recvMeter())
            qDebug() << "meter message: " << meterMessage;
        else
            qDebug() << tr("读取万用表失败");
    }
    writeLog("\n");
    if(!sendMeter(":CONF:VOLT:DC"))
        judge = false;
    else{
        if(recvMeter())
            qDebug() << "meter message: " << meterMessage;
        else
            qDebug() << tr("读取万用表失败");
    }
    paragraph("end");
    if(!judge){
        writeLog(tr("设置万用表失败\n"));
    } else{
        writeLog(tr("设置万用表成功\n"));
    }

    // 初始化ch
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
                qDebug() << tr("读取ZYNQ失败");
        }
        QString result;
        if(!cmd->getStart().isEmpty() && !cmd->getEnd().isEmpty()){
            writeLog(QString(tr("start= %1, end= %2")).arg(cmd->getStart()).arg(cmd->getEnd()));
            int start = zynqMessage.indexOf(cmd->getStart());
            int end = zynqMessage.indexOf(cmd->getEnd());
            writeLog(QString(tr("startIndex= %1, endIndex= %2")).arg(start).arg(end));
            if(start == -1 || end == -1){
                writeLog(tr("截取失败"));
                judge = false;
            }else{
                start = start + cmd->getStart().size();
                result = zynqMessage.mid(start, end-start);
                writeLog(QString(tr("result= %1")).arg(result));
                writeLog(QString(tr("Judge= %1")).arg(cmd->getJudge()));
                if(!result.compare(cmd->getJudge())){
                    writeLog(tr("结果不匹配"));
                    judge = false;
                }
            }
        }else{
            writeLog(tr("该命令没有start或end截取参数"));
        }
        if(i != cmdList->size()-1)
            writeLog("\n");
    }
    paragraph("end");
    if(!judge){
        writeLog(tr("初始化CH失败\n"));
    }else{
        writeLog(tr("初始化CH成功\n"));
    }

    // 开始校准
    QList<QPair<QString, QString> *> * datas = new QList<QPair<QString, QString> *>;
    for(int i = 0; i != dataList->size(); ++i){  // 整理数据
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
                qDebug() << tr("读取ZYNQ失败");
        }



        paragraph("end");
    }
}
