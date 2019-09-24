#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "command.h"
#include "testitem.h"
#include "currentitem.h"

class myThread : public QThread
{
    Q_OBJECT
public:
    explicit myThread(QTcpSocket *, QTcpSocket *, QString, QString, QObject *parent = 0);

    void run();
    bool sendMeter(QString *);                  // 发送万用表
    bool sendZynq(QString *);                   // 发送ZYNQ
    
signals:
    void setProgressMaxSize(int);             // 设置进度条总大小
    void setProgressCurSize(int);             // 设置进度条当前大小
    void statusBarShow(QString);              // 在主窗口状态栏显示
    
public slots:
    void readMeterMessage();                  // 读取万用表信息
    void readZynqMessage();                   // 读取ZYNQ信息
    void writeLog(QString , QString );        // 写入Log
    void writeCsv(QString , QString );        // 写入数据
    
protected:
    QTcpSocket * meter;  // 万用表socket
    QString meterMessage;  // 万用表接收到的信息
    quint16 meterBlockSize;  // 用来存放数据的大小信息

    QTcpSocket * zynq;   // zynq socket
    QString zynqMessage;  // ZYNQ接收到的信息
    quint16 zynqBlockSize;  // 用来存放数据的大小信息

    QString log;
    QString csv;
};

class verifyVoltageThread: public myThread
{
    Q_OBJECT
public:
    explicit verifyVoltageThread(testItem *,QTcpSocket *, QTcpSocket *, QString, QString, QObject *parent = 0);
    void run();
private:
    QList<command *> * cmdList;
    QList<QPair<bool, QPair<QString, QString> *> *> * dataList;  // 数据列表
    command * setCmdVerify;      // 校准设置命令
    int setMulti;                // 校准设置数据写入eeprom时的倍数
    command * dmmCmdVerify;      // 校准读取命令
    int dmmMulti;                // 校准读取数据写入eeprom时的倍数
    command * meterCmdVerify;    // 校准读万用表命令
    int meterMulti;              // 万用表数据写入eeprom时的倍数
    command * setCmdTest;        // 测试设置命令
    command * dmmCmdTest;        // 测试读取命令
    command * meterCmdTest;      // 测试读万用表命令
};

#endif // MYTHREAD_H
