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
    if(!f.open(QFile::WriteOnly | QFile::Text | QFile::Append)){
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
        writeLog(tr("////////////////////////////////////////////////////////////////////////////////"));
    else if (para == "end")
        writeLog(tr("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\"));
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
    cmdList = ch->getCmdList();
    dataList = ch->getDataList();
    setCmdVerify = ch->getSetCmdVerify();
    setMulti = ch->getSetMulti();
    dmmCmdVerify = ch->getDmmCmdVerify();
    dmmMulti = ch->getDmmMulti();
    meterCmdVerify = ch->getMeterCmdVerify();
    meterMulti = ch->getMeterMulti();

    cmdDelay = 100;  // 毫秒
}
void verifyVoltageThread::run()
{
    qDebug() << tr("开始校准电压");

    // 设置万用表为远程电压模式
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
        else
            recvZynq();
        if(cmd->getStart().isEmpty() || cmd->getEnd().isEmpty() || cmd->getJudge().isEmpty()){
            writeLog(tr("该命令缺少截取参数或判断结果参数"));
        }else
            judge = cmd->equalJudge();
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
    if(dmmCmdVerify->getStart().isEmpty() || dmmCmdVerify->getEnd().isEmpty() || dmmCmdVerify->getRatio().toFloat() == 0){
        writeLog(tr("电压读取命令缺少截取参数或差比率"));
        return;
    }
    if(meterCmdVerify->getRatio().toFloat() == 0){
        writeLog(tr("万用表读取命令缺少差比率"));
        return;
    }
    QStringList title;
    title << "set" << "addr" << "dmm" << "addr" << "meter" << "addr" << "dmm-set" << "dmm-meter";
    writeCsv(title.join(","));
    emit setProgressMaxSize(datas->size());
    for(int i = 0; i != datas->size(); ++i){
        paragraph("start");
        judge = true;
        // 设置电压
        int set = datas->at(i)->first.toInt();
        message = QString("[%1]%2(%3)").arg(++cmdIndex).arg(setCmdVerify->getName()).arg(set);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        int dac = int(set * setMulti);
        Sleep(cmdDelay);
        // 读取电压
        message = QString("[%1]%2()").arg(++cmdIndex).arg(dmmCmdVerify->getName());
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        dmmCmdVerify->setResult(zynqMessage);
        judge = dmmCmdVerify->judgeRatio(float(set), float(set));
        float dmm = dmmCmdVerify->getFloatResult().toFloat() * dmmMulti;
        // 读取万用表
        message = meterCmdVerify->getName();
        if(!sendMeter(message))
            judge = false;
        else
            recvMeter();
        meterCmdVerify->setFloatResult(meterMessage.toFloat());
        judge = meterCmdVerify->judgeRatio(dmmCmdVerify->getFloatResult().toFloat(), float(set));
        float meter = meterMessage.toFloat() * meterMulti;
        // 写入eeprom
        QString address = datas->at(i)->second;
        int addr;
        if(!QStringIsInt(address)){
            writeLog(QString(tr("数据地址%1无效")).arg(address));
            return;
        }else
            addr = QString2int(address);
        QString dacStr = QString("%1").arg(dac);
        while(dacStr.length() < 6)
            dacStr = "0" + dacStr;
        QString dmmStr = QString("%1").arg(int(dmm));
        while(dmmStr.length() < 6)
            dmmStr = "0" + dmmStr;
        QString meterStr = QString("%1").arg(int(meter));
        while(meterStr.length() < 6)
            meterStr = "0" + meterStr;
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(int2hexString(addr)).arg(dacStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(int2hexString(addr+6)).arg(dmmStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        message = QString("[%1]eeprom write string(DMM, at16, %2, %3)").arg(++cmdIndex).arg(int2hexString(addr+12)).arg(meterStr);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        // 保存数据到csv
        QStringList csvline;
        csvline << QString("%1").arg(set)
                << QString("%1").arg(addr)
                << QString("%1").arg(dmm)
                << QString("%1").arg(addr+6)
                << QString("%1").arg(meter)
                << QString("%1").arg(addr+12)
                << QString("%1").arg(dmm-dac)
                << QString("%1").arg(dmm-meter);
        writeCsv(csvline.join(","));
        // 表格表示
        QString passOrFail;
        if(judge)
            passOrFail = "pass";
        else
            passOrFail = "fail";
        QStringList tableline = csvline;
        tableline << passOrFail;
        tableline.insert(tableline.begin(), QString("set valtage %1").arg(set));
        emit showTable(tableline);
        emit setProgressCurSize(i+1);
        paragraph("end");
        writeLog("\n");
    }
    statusBarShow(tr("电压校准完成"));
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

    cmdDelay = 100;  // 毫秒
}
void testVoltageThread::run()
{
    qDebug() << tr("开始校准电压");

    // 设置万用表为远程电压模式
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
        else
            recvZynq();
        if(cmd->getStart().isEmpty() || cmd->getEnd().isEmpty() || cmd->getJudge().isEmpty()){
            writeLog(tr("该命令缺少截取参数或判断结果参数"));
        }else
            judge = cmd->equalJudge();
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
    if(dmmCmdTest->getStart().isEmpty() || dmmCmdTest->getEnd().isEmpty() || dmmCmdTest->getRatio().toFloat() == 0){
        writeLog(tr("电压读取命令缺少截取参数或差比率"));
        return;
    }
    if(meterCmdTest->getRatio().toFloat() == 0){
        writeLog(tr("万用表读取命令缺少差比率"));
        return;
    }
    QStringList title;
    title << "set" << "dmm" << "meter" << "dmm-set" << "(dmm-set)/set(%)" << "dmm-meter" << "(dmm-meter)/set(%)";
    writeCsv(title.join(","));
    emit setProgressMaxSize(datas->size());
    for(int i = 0; i != datas->size(); ++i){
        paragraph("start");
        judge = true;
        // 设置电压
        int set = datas->at(i)->first.toInt();
        message = QString("[%1]%2(%3)").arg(++cmdIndex).arg(setCmdTest->getName()).arg(set);
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        Sleep(cmdDelay);
        // 读取电压
        message = QString("[%1]%2()").arg(++cmdIndex).arg(dmmCmdTest->getName());
        if(!sendZynq(message))
            judge = false;
        else
            recvZynq();
        dmmCmdTest->setResult(zynqMessage);
        judge = dmmCmdTest->judgeRatio(float(set), float(set));
        float dmm = dmmCmdTest->getFloatResult().toFloat();
        // 读取万用表
        message = meterCmdTest->getName();
        if(!sendMeter(message))
            judge = false;
        else
            recvMeter();
        meterCmdTest->setFloatResult(meterMessage.toFloat());
        judge = meterCmdTest->judgeRatio(dmmCmdTest->getFloatResult().toFloat(), float(set));
        float meter = meterMessage.toFloat();

        float sub1 = dmm-set;
        float ratio1 = sub1 / set * 100;
        float sub2 = dmm-meter;
        float ratio2 = sub2 / set * 100;

        // 保存数据到csv
        QStringList csvline;
        csvline << QString::number(set, 'f', 4)
                << QString::number(dmm, 'f', 2)
                << QString::number(meter, 'f', 2)
                << QString::number(sub1, 'f', 2)
                << QString::number(ratio1, 'f', 2)
                << QString::number(sub2, 'f', 2)
                << QString::number(ratio2, 'f', 2);
        writeCsv(csvline.join(","));
        // 表格表示
        QString passOrFail;
        if(judge)
            passOrFail = "pass";
        else
            passOrFail = "fail";
        QStringList tableline = csvline;
        tableline << passOrFail;
        tableline.insert(tableline.begin(), QString("set valtage %1").arg(set));
        emit showTable(tableline);
        emit setProgressCurSize(i+1);
        paragraph("end");
        writeLog("\n");
    }
    statusBarShow(tr("电压校准完成"));
}
