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
    bool sendMeter(QString);                // ���������ñ�����Ϣ
    bool sendZynq(QString);                 // ������ZYNQ������Ϣ
    void paragraph(QString);                  // ���俪ʼ�����
    bool recvMeter();                         // �����ñ��ȡ��Ϣ
    bool recvZynq();                          // ��ZYNQ��ȡ��Ϣ
    
signals:
    void setProgressMaxSize(int);             // ���ý������ܴ�С
    void setProgressCurSize(int);             // ���ý�������ǰ��С
    void statusBarShow(QString);              // ��������״̬����ʾ
    void showTable(QStringList);              // �������ڵı������ʾ���
    
public slots:
    void writeLog(QString );        // д��Log
    void writeCsv(QString );        // д������
    
protected:
    QTcpSocket * meter;  // ���ñ�socket
    QString meterMessage;  // ���ñ���յ�����Ϣ

    QTcpSocket * zynq;   // zynq socket
    QString zynqMessage;  // ZYNQ���յ�����Ϣ

    qint64 meterTimeOut;  // �������ñ�ʱ�ĳ�ʱ
    qint64 zynqTimeOut;  // ����zynqʱ�ĳ�ʱ
    qint64 cmdDelay;  // ������ʱ

    int dataLength;  // ���ݳ���

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
    QList<DataItem *> * dataList;  // �����б�
    command * setCmdVerify;      // У׼��������
    int setMulti;                // У׼��������д��eepromʱ�ı���
    command * dmmCmdVerify;      // У׼��ȡ����
    int dmmMulti;                // У׼��ȡ����д��eepromʱ�ı���
    command * meterCmdVerify;    // У׼�����ñ�����
    int meterMulti;              // ���ñ�����д��eepromʱ�ı���
};

class testVoltageThread: public myThread
{
    Q_OBJECT
public:
    explicit testVoltageThread(testItem *,QTcpSocket *, QTcpSocket *, QString, QString, QObject *parent = 0);
    void run();
private:
    QList<command *> * cmdList;
    QList<DataItem *> * dataList;  // �����б�
    command * setCmdTest;      // У׼��������
    command * dmmCmdTest;      // У׼��ȡ����
    command * meterCmdTest;    // У׼�����ñ�����
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
