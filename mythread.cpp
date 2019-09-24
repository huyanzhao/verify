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
    meterBlockSize = 0;  // 初始化数据大小信息为0
    connect(zynq, SIGNAL(readyRead()), this, SLOT(readZynqMessage()));
    zynqBlockSize = 0;  // 初始化数据大小信息为0
}
// 接收万用表信息
void myThread::readMeterMessage()
{
    QDataStream in(meter);
    // 设置数据流版本，这里要和服务器端相同
    in.setVersion(QDataStream::Qt_4_6);
    // 如果是刚开始接收数据
    if (meterBlockSize == 0) {
        //判断接收的数据是否大于两字节，也就是文件的大小信息所占的空间
        //如果是则保存到blockSize变量中，否则直接返回，继续接收数据
        if(meter->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> meterBlockSize;
    }
    // 如果没有得到全部的数据，则返回，继续接收数据
    if(meter->bytesAvailable() < meterBlockSize) return;
    // 将接收到的数据存放到变量中
    in >> meterMessage;
    // 显示接收到的数据
    qDebug() << tr("万用表回复: ") << meterMessage;
}
// 接收ZYNQ信息
void myThread::readZynqMessage()
{
    QDataStream in(zynq);
    // 设置数据流版本，这里要和服务器端相同
    in.setVersion(QDataStream::Qt_4_6);
    // 如果是刚开始接收数据
    if (zynqBlockSize == 0) {
        //判断接收的数据是否大于两字节，也就是文件的大小信息所占的空间
        //如果是则保存到blockSize变量中，否则直接返回，继续接收数据
        if(zynq->bytesAvailable() < (int)sizeof(quint16)) return;
        in >> zynqBlockSize;
    }
    // 如果没有得到全部的数据，则返回，继续接收数据
    if(zynq->bytesAvailable() < zynqBlockSize) return;
    // 将接收到的数据存放到变量中
    in >> zynqMessage;
    // 显示接收到的数据
    qDebug() << tr("ZYNQ回复: ") << zynqMessage;
}
// 记录Log
void myThread::writeLog(QString logFile, QString message)
{
    QDateTime local(QDateTime::currentDateTime());
    QString timeStamp = local.toString("yyyy-MM-dd hh:mm:ss.zzz  ");
    QFile f(logFile);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << tr("log 文件打开失败!");
//        QMessageBox::warning(this, tr("打开文件失败"), QString("%1 打开失败!").arg(logFile), QMessageBox::Ok);
        return;
    }
    QTextStream txtOutput(&f);
    if(message == "\n")
        txtOutput << endl;
    else
        txtOutput << timeStamp << message << endl;
    f.close();
}
// 写入数据
void myThread::writeCsv(QString csvFile, QString data)
{
    QFile f(csvFile);
    if(!f.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
        qDebug() << tr("数据文件打开失败!");
//        QMessageBox::warning(this, tr("打开文件失败"), QString(tr("%1 打开失败!")).arg(csvFile), QMessageBox::Ok);
        return;
    }
    QTextStream textStream;
    textStream.setDevice(&f);
    textStream.setCodec("utf-8");
    textStream << data +"\n";
    f.close();
}
// 发送给万用表
bool myThread::sendMeter(QString * message)
{
    int sendRc = meter->write((const char *)message);
    if(sendRc == -1){
        qDebug () << tr("万用表发送数据失败！");
        return false;
    } else
        return true;
}
// 发送给ZYNQ
bool myThread::sendZynq(QString * message)
{
    int sendRc = zynq->write((const char *)message);
    if(sendRc == -1){
        qDebug () << tr("ZYNQ发送数据失败！");
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
    qDebug() << tr("校准电压线程创建成功!");
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
    bool judge = true;
    QString message = "SYSTEM:REMOTE";
    if(!sendMeter(&message))
        judge = false;
    message = ":CONF:VOLT:DC";
    if(!sendMeter(&message))
        judge = false;
    if(!judge){
        writeLog(log, QString("设置万用表失败"));
        emit statusBarShow(tr("设置万用表失败"));
    } else{
        writeLog(log, QString("设置万用表成功"));
        emit statusBarShow(tr("设置万用表成功"));
    }
    int cmdIndex = 0;
    // 初始化ch
    judge = true;
    for(int i = 0; i != cmdList->size(); ++i){
        command * cmd = cmdList->at(i);
        writeLog(log, QString("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"));
        message = cmd->getFullName();
        message = QString("[%1]").arg(++cmdIndex) + message;
        writeLog(log, QString("send: ") + message);
        emit statusBarShow(QString(tr("向ZYNQ发送: ")) + message);
        if(!sendZynq(&message))
            judge = false;
        else{
            qDebug() << "zynaMessage: " << zynqMessage;
            writeLog(log, QString("recv: ") + zynqMessage);
            emit statusBarShow(QString(tr("从ZYNQ接收: ")) + zynqMessage);
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
        writeLog(log, QString("ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ\n"));
    }
    if(!judge){
        writeLog(log, QString("初始化CH失败"));
        emit statusBarShow(tr("初始化CH失败"));
    }else{
        writeLog(log, QString("初始化CH成功"));
        emit statusBarShow(tr("初始化CH成功"));
    }
}
