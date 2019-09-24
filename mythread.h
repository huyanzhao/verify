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
    bool sendMeter(QString *);                  // �������ñ�
    bool sendZynq(QString *);                   // ����ZYNQ
    
signals:
    void setProgressMaxSize(int);             // ���ý������ܴ�С
    void setProgressCurSize(int);             // ���ý�������ǰ��С
    void statusBarShow(QString);              // ��������״̬����ʾ
    
public slots:
    void readMeterMessage();                  // ��ȡ���ñ���Ϣ
    void readZynqMessage();                   // ��ȡZYNQ��Ϣ
    void writeLog(QString , QString );        // д��Log
    void writeCsv(QString , QString );        // д������
    
protected:
    QTcpSocket * meter;  // ���ñ�socket
    QString meterMessage;  // ���ñ���յ�����Ϣ
    quint16 meterBlockSize;  // ����������ݵĴ�С��Ϣ

    QTcpSocket * zynq;   // zynq socket
    QString zynqMessage;  // ZYNQ���յ�����Ϣ
    quint16 zynqBlockSize;  // ����������ݵĴ�С��Ϣ

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
    QList<QPair<bool, QPair<QString, QString> *> *> * dataList;  // �����б�
    command * setCmdVerify;      // У׼��������
    int setMulti;                // У׼��������д��eepromʱ�ı���
    command * dmmCmdVerify;      // У׼��ȡ����
    int dmmMulti;                // У׼��ȡ����д��eepromʱ�ı���
    command * meterCmdVerify;    // У׼�����ñ�����
    int meterMulti;              // ���ñ�����д��eepromʱ�ı���
    command * setCmdTest;        // ������������
    command * dmmCmdTest;        // ���Զ�ȡ����
    command * meterCmdTest;      // ���Զ����ñ�����
};

#endif // MYTHREAD_H
