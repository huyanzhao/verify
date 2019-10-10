#ifndef TESTITEM_H
#define TESTITEM_H

#include <QPair>
#include "command.h"

class testItem
{
public:
    testItem(QList<command *> *,
    QList<QPair<bool, QPair<QString, QString> *> *> *, int,
    command *, int,
    command *, int,
    command *, int,
    command *,
    command *,
    command *);    
    ~testItem();

    void setCmdList(QList<command *> *);  // ����ǰ�������б�
    void setDataList(QList<QPair<bool, QPair<QString, QString> *> *> *);  // ���������б�
    void setDatalength(int);              // �������ݳ���
    void setSetCmdVerify(command *);      // ����У׼��������
    void setSetMulti(int);                // ����У׼���ݱ���
    void setDmmCmdVerify(command *);      // ����У׼��ȡ����
    void setDmmMulti(int);                // ����У׼��ȡ����
    void setMeterCmdVerify(command *);    // ����У׼�����ñ�����
    void setMeterMulti(int);              // ����У׼���ñ���
    void setSetCmdTest(command *);        // ���ò�����������
    void setDmmCmdTest(command *);        // ���ò��Զ�ȡ����
    void setMeterCmdTest(command *);      // ���ò��Զ����ñ�����

    QList<command *> * getCmdList();      // ��ȡǰ�������б�
    QList<QPair<bool, QPair<QString, QString> *> *> * getDataList();  // ��ȡ�����б�
    int getDataLength();                  // ��ȡ���ݳ���
    command * getSetCmdVerify();          // ��ȡУ׼��������
    int getSetMulti();                    // ��ȡУ׼���ݱ���
    command * getDmmCmdVerify();          // ��ȡУ׼��ȡ����
    int getDmmMulti();                    // ��ȡУ׼��ȡ����
    command * getMeterCmdVerify();        // ��ȡУ׼�����ñ�����
    int getMeterMulti();                  // ��ȡУ׼���ñ���
    command * getSetCmdTest();            // ��ȡ������������
    command * getDmmCmdTest();            // ��ȡ���Զ�ȡ����
    command * getMeterCmdTest();          // ��ȡ���Զ����ñ�����

    testItem * deepcopy();                // ���Ը���

private:
    QList<command *> * cmdList;  // ǰ�������б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataList;  // �����б�
    int dataLength;              // ���ݳ���
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

#endif // TESTITEM_H
