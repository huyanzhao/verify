#include "testitem.h"
#include "command.h"
#include "dataitem.h"
#include <QPair>
#include <QDebug>

testItem::testItem(QList<command *> * cmdListParam,  // 前置命令列表
                   QList<DataItem *> * dataListParam, int dataLengthParam, // 数据列表和数据长度
                   command * setVerifyParam, int setMultiParam,  // 校准设置命令和倍数
                   command * dmmVerifyParam, int dmmMultiParam,  // 校准读取命令和倍数
                   command * meterVerifyParam, int meterMultiParam,  // 校准读万用表命令和倍数
                   command * setTestParam,  // 测试设置命令
                   command * dmmTestParam,  // 测试读取命令
                   command * meterTestParam):  // 测试读万用表命令
    cmdList(cmdListParam),              // 前置命令列表
    dataList(dataListParam),            // 数据列表
    dataLength(dataLengthParam),
    setCmdVerify(setVerifyParam),       // 校准设置命令和倍数
    setMulti(setMultiParam),
    dmmCmdVerify(dmmVerifyParam),       // 校准读取命令和倍数
    dmmMulti(dmmMultiParam),
    meterCmdVerify(meterVerifyParam),   // 校准读万用表命令和倍数
    meterMulti(meterMultiParam),
    setCmdTest(setTestParam),           // 测试设置命令
    dmmCmdTest(dmmTestParam),           // 测试读取命令
    meterCmdTest(meterTestParam)        // 测试读万用表命令
{
}
testItem::~testItem()
{
    // 清空前置命令列表
    for(int i=0; i != cmdList->size(); ++i){
        command * temp = cmdList->at(i);
        delete temp;
    }
    dataList->clear();
    delete cmdList;  // 销毁前置命令列表
    // 清空数据列表
    while(!dataList->isEmpty()){
        DataItem * temp = dataList->at(0);
        delete temp;  // 销毁数据地址对
        dataList->removeAt(0);
    }
    delete dataList; // 销毁数据列表
    delete setCmdVerify;  // 销毁校准设置命令
    delete dmmCmdVerify;  // 销毁校准读取命令
    delete meterCmdVerify;  // 销毁校准读万用表命令

    delete setCmdTest;  // 销毁测试设置命令
    delete dmmCmdTest;  // 销毁测试读取命令
    delete meterCmdTest;  // 销毁测试读万用表命令
}
// 设置前置命令列表
void testItem::setCmdList(QList<command *> * param)
{
    cmdList = param;
}
// 设置数据列表
void testItem::setDataList(QList<DataItem *> * param)
{
    dataList = param;
}
// 设置数据长度
void testItem::setDatalength(int param)
{
    dataLength = param;
}
// 设置校准设置命令
void testItem::setSetCmdVerify(command * param)
{
    setCmdVerify = param;
}
// 设置校准数据倍数
void testItem::setSetMulti(int param)
{
    setMulti = param;
}
// 设置校准读取命令
void testItem::setDmmCmdVerify(command * param)
{
    dmmCmdVerify = param;
}
// 设置校准读取倍数
void testItem::setDmmMulti(int param)
{
    dmmMulti = param;
}
// 设置校准读万用表命令
void testItem::setMeterCmdVerify(command * param)
{
    meterCmdVerify = param;
}
// 设置校准万用表倍数
void testItem::setMeterMulti(int param)
{
    meterMulti = param;
}
// 设置测试设置命令
void testItem::setSetCmdTest(command * param)
{
    setCmdTest = param;
}
// 设置测试读取命令
void testItem::setDmmCmdTest(command * param)
{
    dmmCmdTest = param;
}
// 设置测试读万用表命令
void testItem::setMeterCmdTest(command * param)
{
    meterCmdTest = param;
}
// 获取前置命令列表
QList<command *> * testItem::getCmdList()
{
    return cmdList;
}
// 获取数据列表
QList<DataItem *> * testItem::getDataList()
{
    return dataList;
}
// 获取数据长度
int testItem::getDataLength()
{
    return dataLength;
}
// 获取校准设置命令
command * testItem::getSetCmdVerify()
{
    return setCmdVerify;
}
// 获取校准数据倍数
int testItem::getSetMulti()
{
    return setMulti;
}
// 获取校准读取命令
command * testItem::getDmmCmdVerify()
{
    return dmmCmdVerify;
}
// 获取校准读取倍数
int testItem::getDmmMulti()
{
    return dmmMulti;
}
// 获取校准读万用表命令
command * testItem::getMeterCmdVerify()
{
    return meterCmdVerify;
}
// 获取校准万用表倍数
int testItem::getMeterMulti()
{
    return meterMulti;
}
// 获取测试设置命令
command * testItem::getSetCmdTest()
{
    return setCmdTest;
}
// 获取测试读取命令
command * testItem::getDmmCmdTest()
{
    return dmmCmdTest;
}
// 获取测试读万用表命令
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
