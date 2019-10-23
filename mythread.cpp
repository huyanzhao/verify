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
    meterTimeOut = 1000;  // 毫秒
    zynqTimeOut = 3000;  // 毫秒


    rx.setPattern("(\\.){0,1}0+$");
}
// 记录Log
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
// 写入数据
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

    dataLength = ch->getDataLength();
}
void verifyVoltageThread::run()
{
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
    QList<DataItem *> * datas = new QList<DataItem *>;
    for(int i = 0; i != dataList->size(); ++i){  // 整理数据
        if(dataList->at(i)->check){
            datas->append(dataList->at(i));
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
        double set = datas->at(i)->data;
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
        // 保存数据到csv
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
        // 表格表示
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

    // 开始测试
    QList<DataItem *> * datas = new QList<DataItem *>;
    for(int i = 0; i != dataList->size(); ++i){  // 整理数据
        if(dataList->at(i)->check){
            datas->append(dataList->at(i));
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
        double set = datas->at(i)->data;
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

        float sub1 = set-dmm;
        float ratio1 = sub1 / set * 100;
        float sub2 = meter-dmm;
        float ratio2 = sub2 / set * 100;

        // 保存数据到csv
        QStringList csvline;
        csvline << QString("%1").arg(set,0,'f',-1).replace(rx,"")
                << QString("%1").arg(dmm,0,'f',-1).replace(rx,"")
                << QString("%1").arg(meter,0,'f',-1).replace(rx,"")
                << QString("%1").arg(sub1,0,'f',-1).replace(rx,"")
                << QString("%1").arg(ratio1,0,'f',-1).replace(rx,"")+"\%"
                << QString("%1").arg(sub2,0,'f',-1).replace(rx,"")
                << QString("%1").arg(ratio2,0,'f',-1).replace(rx,"")+"\%";
        writeCsv(csvline.join(","));
        // 表格表示
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
    statusBarShow(tr("电压测试完成"));
}
verifyCurrentThread::verifyCurrentThread(currentItem * psuParam, QList<int> * partListParam, QString StrParam,
                                         QTcpSocket * meterSocket, QTcpSocket *zynqSocket,
                                         QString logFile, QString csvFile, QObject *parent):
    myThread(meterSocket, zynqSocket, logFile, csvFile, parent)
{
    psu = psuParam;
    partList = partListParam;
    Str = StrParam;
    cmdDelay = 100;  // 毫秒
}
void verifyCurrentThread::run()
{
    // 设置万用表为远程电流模式
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
        writeLog(tr("设置万用表失败\n"));
    } else{
        writeLog(tr("设置万用表成功\n"));
    }

    // 初始化psu
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
            writeLog(tr("该命令缺少截取参数或判断结果参数"));
        }else
            judge = cmd->equalJudge();
        if(i != cmdList->size()-1)
            writeLog("\n");
    }
    paragraph("end");
    if(!judge){
        writeLog(tr("初始化PSU失败\n"));
    }else{
        writeLog(tr("初始化PSU成功\n"));
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
            statusBarShow(tr("part参数错误，退出校准"));
            return;
        }
        if(part == NULL){
            statusBarShow(tr("没有part%1的数据，跳至下一档").arg(partList->at(i)));
            continue;
        }
        // 切换档位
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
                writeLog(tr("该命令缺少截取参数或判断结果参数"));
            }else
                judge = cmd->equalJudge();
            if(i != cmdList->size()-1)
                writeLog("\n");
        }
        paragraph("end");
        if(!judge){
            writeLog(tr("档位切换失败\n"));
        }else{
            writeLog(tr("档位切换成功\n"));
        }
        // 开始校准
        QList<DataItem *> * datas = new QList<DataItem *>;
        QList<DataItem *> * dataList = part->getDataList();
        for(int k = 0; k != dataList->size(); ++k){  // 整理数据
            if(dataList->at(k)->check){
                datas->append(dataList->at(k));
            }
        }
        command * dmmCmdVerify = part->getDmmCmdVerify();
        if(dmmCmdVerify->getStart().isEmpty() || dmmCmdVerify->getEnd().isEmpty() || dmmCmdVerify->getRatio().toFloat() == 0){
            writeLog(tr("电压读取命令缺少截取参数或差比率"));
            return;
        }
        command * meterCmdVerify = part->getMeterCmdVerify();
        if(meterCmdVerify->getRatio().toFloat() == 0){
            writeLog(tr("万用表读取命令缺少差比率"));
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
            // 设置电流
            double set = datas->at(l)->data;
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
            // 保存数据到csv
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
            // 表格表示
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
    statusBarShow(tr("电流校准完成"));
}
testCurrentThread::testCurrentThread(currentItem *psuParam, QList<int> * partListParam, QString StrParam,
                                     QTcpSocket * meterSocket, QTcpSocket * zynqSocket,
                                     QString logFile, QString csvFile, QObject *parent):
    myThread(meterSocket, zynqSocket, logFile, csvFile, parent)
{
    psu = psuParam;
    partList = partListParam;
    Str = StrParam;
    cmdDelay = 100;  // 毫秒
}
void testCurrentThread::run()
{
    // 设置万用表为远程电流模式
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
        writeLog(tr("设置万用表失败\n"));
    } else{
        writeLog(tr("设置万用表成功\n"));
    }

    // 初始化psu
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
            writeLog(tr("该命令缺少截取参数或判断结果参数"));
        }else
            judge = cmd->equalJudge();
        if(i != cmdList->size()-1)
            writeLog("\n");
    }
    paragraph("end");
    if(!judge){
        writeLog(tr("初始化PSU失败\n"));
    }else{
        writeLog(tr("初始化PSU成功\n"));
    }
    int sum = 0;
    for(int i=0; i != partList->size(); ++i){  // 计算得到测试项总数，用于设置进度条
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
            statusBarShow(tr("part参数错误，退出校准"));
            return;
        }
        if(part == NULL){
            statusBarShow(tr("没有part%1的数据，跳至下一档").arg(partList->at(i)));
            continue;
        }
        // 切换档位
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
                writeLog(tr("该命令缺少截取参数或判断结果参数"));
            }else
                judge = cmd->equalJudge();
            if(j != cmdList->size()-1)
                writeLog("\n");
        }
        paragraph("end");
        if(!judge){
            writeLog(tr("档位切换失败\n"));
        }else{
            writeLog(tr("档位切换成功\n"));
        }
        // 开始测试
        QList<DataItem *> * datas = new QList<DataItem *>;
        QList<DataItem *> * dataList = part->getDataList();
        for(int k = 0; k != dataList->size(); ++k){  // 整理数据
            if(dataList->at(k)->check){
                datas->append(dataList->at(k));
            }
        }
        command * dmmCmdTest = part->getDmmCmdTest();
        if(dmmCmdTest->getStart().isEmpty() || dmmCmdTest->getEnd().isEmpty() || dmmCmdTest->getRatio().toFloat() == 0){
            writeLog(tr("电流读取命令缺少截取参数或差比率"));
            return;
        }
        command * meterCmdTest = part->getMeterCmdTest();
        if(meterCmdTest->getRatio().toFloat() == 0){
            writeLog(tr("万用表读取命令缺少差比率"));
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
            // 设置电流
            double set = datas->at(l)->data;
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

            float sub1 = set-dmm;
            float ratio1 = sub1 / set * 100;
            float sub2 = meter-dmm;
            float ratio2 = sub2 / set * 100;

            // 保存数据到csv
            QStringList csvline;
            csvline << QString("%1").arg(set,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(dmm,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(meter,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(sub1,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(ratio1,0,'f',-1).replace(rx,"")+"\%"
                    << QString("%1").arg(sub2,0,'f',-1).replace(rx,"")
                    << QString("%1").arg(ratio2,0,'f',-1).replace(rx,"")+"\%";
            writeCsv(csvline.join(","));
            // 表格表示
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
    statusBarShow(tr("电流测试完成"));
}
