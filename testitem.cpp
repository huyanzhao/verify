#include "testitem.h"

testItem::testItem(QList<command *> * cmdListParam,  // ǰ�������б�
                   QList<QPair<bool, QPair<QString, QString> *> *> * dataListParam,  // �����б�
                   command * setVerifyParam, int setMultiParam,  // У׼��������ͱ���
                   command * dmmVerifyParam, int dmmMultiParam,  // У׼��ȡ����ͱ���
                   command * meterVerifyParam, int meterMultiParam,  // У׼�����ñ�����ͱ���
                   command * setTestParam,  // ������������
                   command * dmmTestParam,  // ���Զ�ȡ����
                   command * meterTestParam):  // ���Զ����ñ�����
    cmdList(cmdListParam),              // ǰ�������б�
    dataList(dataListParam),            // �����б�
    setCmdVerify(setVerifyParam),       // У׼��������ͱ���
    setMulti(setMultiParam),
    dmmCmdVerify(dmmVerifyParam),       // У׼��ȡ����ͱ���
    dmmMulti(dmmMultiParam),
    meterCmdVerify(meterVerifyParam),   // У׼�����ñ�����ͱ���
    meterMulti(meterMultiParam),
    setCmdTest(setTestParam),           // ������������
    dmmCmdTest(dmmTestParam),           // ���Զ�ȡ����
    meterCmdTest(meterTestParam)        // ���Զ����ñ�����
{
}

testItem::~testItem()
{
    // ���ǰ�������б�
    for(int i=0; i != cmdList->size(); ++i){
        command * temp = cmdList->at(i);
        delete temp;
        cmdList->removeAt(i);
    }
    delete cmdList;  // ����ǰ�������б�
    // ��������б�
    for(int i=0; i != dataList->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempPair = dataList->at(i);
        QPair<QString, QString> * temp = tempPair->second;
        delete temp;  // �������ݵ�ַ��
        delete tempPair;  // ����������,�����������־ѡ��״̬�Ĳ���ֵ�����ݵ�ַ��
        dataList->removeAt(i);
    }
    delete dataList; // ���������б�

    delete setCmdVerify;  // ����У׼��������
    delete dmmCmdVerify;  // ����У׼��ȡ����
    delete meterCmdVerify;  // ����У׼�����ñ�����

    delete setCmdTest;  // ���ٲ�����������
    delete dmmCmdTest;  // ���ٲ��Զ�ȡ����
    delete meterCmdTest;  // ���ٲ��Զ����ñ�����
}
// ����ǰ�������б�
void testItem::setCmdList(QList<command *> * param)
{
    cmdList = param;
}
// ���������б�
void testItem::setDataList(QList<QPair<bool, QPair<QString, QString> *> *> * param)
{
    dataList = param;
}
// ����У׼��������
void testItem::setSetCmdVerify(command * param)
{
    setCmdVerify = param;
}
// ����У׼���ݱ���
void testItem::setSetMulti(int param)
{
    setMulti = param;
}
// ����У׼��ȡ����
void testItem::setDmmCmdVerify(command * param)
{
    dmmCmdVerify = param;
}
// ����У׼��ȡ����
void testItem::setDmmMulti(int param)
{
    dmmMulti = param;
}
// ����У׼�����ñ�����
void testItem::setMeterCmdVerify(command * param)
{
    meterCmdVerify = param;
}
// ����У׼���ñ���
void testItem::setMeterMulti(int param)
{
    meterMulti = param;
}
// ���ò�����������
void testItem::setSetCmdTest(command * param)
{
    setCmdTest = param;
}
// ���ò��Զ�ȡ����
void testItem::setDmmCmdTest(command * param)
{
    dmmCmdTest = param;
}
// ���ò��Զ����ñ�����
void testItem::setMeterCmdTest(command * param)
{
    meterCmdTest = param;
}
// ��ȡǰ�������б�
QList<command *> * testItem::getCmdList()
{
    return cmdList;
}
// ��ȡ�����б�
QList<QPair<bool, QPair<QString, QString> *> *> * testItem::getDataList()
{
    return dataList;
}
// ��ȡУ׼��������
command * testItem::getSetCmdVerify()
{
    return setCmdVerify;
}
// ��ȡУ׼���ݱ���
int testItem::getSetMulti()
{
    return setMulti;
}
// ��ȡУ׼��ȡ����
command * testItem::getDmmCmdVerify()
{
    return dmmCmdVerify;
}
// ��ȡУ׼��ȡ����
int testItem::getDmmMulti()
{
    return dmmMulti;
}
// ��ȡУ׼�����ñ�����
command * testItem::getMeterCmdVerify()
{
    return meterCmdVerify;
}
// ��ȡУ׼���ñ���
int testItem::getMeterMulti()
{
    return meterMulti;
}
// ��ȡ������������
command * testItem::getSetCmdTest()
{
    return setCmdTest;
}
// ��ȡ���Զ�ȡ����
command * testItem::getDmmCmdTest()
{
    return dmmCmdTest;
}
// ��ȡ���Զ����ñ�����
command * testItem::getMeterCmdTest()
{
    return meterCmdTest;
}
