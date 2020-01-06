#include "testitem.h"
#include "command.h"
#include "dataitem.h"
#include <QPair>
#include <QDebug>

testItem::testItem(QList<command *> * cmdListParam,  // ǰ�������б�
                   QList<DataItem *> * dataListParam, int dataLengthParam, // �����б�����ݳ���
                   command * setVerifyParam, int setMultiParam,  // У׼��������ͱ���
                   command * dmmVerifyParam, int dmmMultiParam,  // У׼��ȡ����ͱ���
                   command * meterVerifyParam, int meterMultiParam,  // У׼�����ñ�����ͱ���
                   command * setTestParam,  // ������������
                   command * dmmTestParam,  // ���Զ�ȡ����
                   command * meterTestParam):  // ���Զ����ñ�����
    cmdList(cmdListParam),              // ǰ�������б�
    dataList(dataListParam),            // �����б�
    dataLength(dataLengthParam),
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
    }
    dataList->clear();
    delete cmdList;  // ����ǰ�������б�
    // ��������б�
    while(!dataList->isEmpty()){
        DataItem * temp = dataList->at(0);
        delete temp;  // �������ݵ�ַ��
        dataList->removeAt(0);
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
void testItem::setDataList(QList<DataItem *> * param)
{
    dataList = param;
}
// �������ݳ���
void testItem::setDatalength(int param)
{
    dataLength = param;
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
QList<DataItem *> * testItem::getDataList()
{
    return dataList;
}
// ��ȡ���ݳ���
int testItem::getDataLength()
{
    return dataLength;
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
testItem * testItem::deepcopy()
{
    if(this == NULL)
        return NULL;
    QList<command *> * newCmdList = new QList<command *>;
    for(int i =0; i != cmdList->size(); ++i){
        newCmdList->append(cmdList->at(i)->deepcopy());
    }
    QList<DataItem *> * newDataList = new QList<DataItem *>;
    for(int i=0; i != dataList->size(); ++i)
        newDataList->append(new DataItem(dataList->at(i)->check, dataList->at(i)->data,
                                         dataList->at(i)->dacAddr, dataList->at(i)->adcAddr, dataList->at(i)->refAddr));
    int newDataLength = dataLength;
    command * newSetCmdVerify = setCmdVerify->deepcopy();
    int newSetMulti = setMulti;
    command * newDmmCmdVerify = dmmCmdVerify->deepcopy();
    int newDmmMulti = dmmMulti;
    command * newMeterCmdVerify = meterCmdVerify->deepcopy();
    int newMeterMulti = meterMulti;
    command * newSetCmdTest = setCmdTest->deepcopy();
    command * newDmmCmdTest = dmmCmdTest->deepcopy();
    command * newMeterCmdTest = meterCmdTest->deepcopy();
    return new testItem(newCmdList, newDataList, newDataLength, newSetCmdVerify, newSetMulti, newDmmCmdVerify,
                    newDmmMulti, newMeterCmdVerify, newMeterMulti, newSetCmdTest, newDmmCmdTest, newMeterCmdTest);
}
