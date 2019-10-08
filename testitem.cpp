#include "testitem.h"

testItem::testItem(QList<command *> * cmdListParam,  // 前置命令列表
                   QList<QPair<bool, QPair<QString, QString> *> *> * dataListParam,  // 数据列表
                   command * setVerifyParam, int setMultiParam,  // 校准设置命令和倍数
                   command * dmmVerifyParam, int dmmMultiParam,  // 校准读取命令和倍数
                   command * meterVerifyParam, int meterMultiParam,  // 校准读万用表命令和倍数
                   command * setTestParam,  // 测试设置命令
                   command * dmmTestParam,  // 测试读取命令
                   command * meterTestParam):  // 测试读万用表命令
    cmdList(cmdListParam),              // 前置命令列表
    dataList(dataListParam),            // 数据列表
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
        cmdList->removeAt(i);
    }
    delete cmdList;  // 销毁前置命令列表
    // 清空数据列表
    for(int i=0; i != dataList->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempPair = dataList->at(i);
        QPair<QString, QString> * temp = tempPair->second;
        delete temp;  // 销毁数据地址对
        delete tempPair;  // 销毁数据项,数据项包括标志选中状态的布尔值和数据地址对
        dataList->removeAt(i);
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
void testItem::setDataList(QList<QPair<bool, QPair<QString, QString> *> *> * param)
{
    dataList = param;
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
QList<QPair<bool, QPair<QString, QString> *> *> * testItem::getDataList()
{
    return dataList;
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
