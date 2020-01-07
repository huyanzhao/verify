#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "command.h"
#include "testitem.h"
#include "currentitem.h"
#include "dataitem.h"

class myThread : public QThread
{
    Q_OBJECT
public:
    explicit myThread(QTcpSocket *, QTcpSocket *, QString, QString, QObject *parent = 0);

    void run();
    bool sendMeter(QString);                // 给发送万用表发送信息
    bool sendZynq(QString);                 // 给发送ZYNQ发送信息
    void paragraph(QString);                  // 段落开始与结束
    bool recvMeter();                         // 从万用表读取信息
    bool recvZynq();                          // 从ZYNQ读取信息
    
signals:
    void setProgressMaxSize(int);             // 设置进度条总大小
    void setProgressCurSize(int);             // 设置进度条当前大小
    void statusBarShow(QString);              // 在主窗口状态栏显示
    void showTable(QStringList);              // 在主窗口的表格中显示结果
    
public slots:
    void writeLog(QString );        // 写入Log
    void writeCsv(QString );        // 写入数据
    
protected:
    QTcpSocket * meter;  // 万用表socket
    QString meterMessage;  // 万用表接收到的信息

    QTcpSocket * zynq;   // zynq socket
    QString zynqMessage;  // ZYNQ接收到的信息

    qint64 meterTimeOut;  // 接收万用表时的超时
    qint64 zynqTimeOut;  // 接收zynq时的超时
    qint64 cmdDelay;  // 命令延时

    int dataLength;  // 数据长度

    QString log;
    QString csv;


    QRegExp rx;
};

class verifyVoltageThread: public myThread
{
    Q_OBJECT
public:
    explicit verifyVoltageThread(testItem *,QTcpSocket *, QTcpSocket *, QString, QString, QObject *parent = 0);
    void run();
private:
    QList<command *> * cmdList;
    QList<DataItem *> * dataList;  // 数据列表
    command * setCmdVerify;      // 校准设置命令
    int setMulti;                // 校准设置数据写入eeprom时的倍数
    command * dmmCmdVerify;      // 校准读取命令
    int dmmMulti;                // 校准读取数据写入eeprom时的倍数
    command * meterCmdVerify;    // 校准读万用表命令
    int meterMulti;              // 万用表数据写入eeprom时的倍数
};

class testVoltageThread: public myThread
{
    Q_OBJECT
public:
    explicit testVoltageThread(testItem *,QTcpSocket *, QTcpSocket *, QString, QString, QObject *parent = 0);
    void run();
private:
    QList<command *> * cmdList;
    QList<DataItem *> * dataList;  // 数据列表
    command * setCmdTest;      // 校准设置命令
    command * dmmCmdTest;      // 校准读取命令
    command * meterCmdTest;    // 校准读万用表命令
};

class verifyCurrentThread: public myThread
{
    Q_OBJECT
public:
    explicit verifyCurrentThread(currentItem *, QList<int> *, QString, QTcpSocket *, QTcpSocket *, QString, QString, QObject *parent = 0);
    void run();
private:
    currentItem * psu;
    QList<int> * partList;
    QString Str;
};

class testCurrentThread: public myThread
{
    Q_OBJECT
public:
    explicit testCurrentThread(currentItem *, QList<int> *, QString, QTcpSocket *, QTcpSocket *, QString, QString, QObject *parent = 0);
    void run();
private:
    currentItem * psu;
    QList<int> * partList;
    QString Str;
};
#endif // MYTHREAD_H
