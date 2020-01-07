#ifndef TESTITEM_H
#define TESTITEM_H

#include <QPair>
#include "command.h"
#include "dataitem.h"

class testItem
{
public:
    testItem(QList<command *> *,
    QList<DataItem *> *, int,
    command *, int,
    command *, int,
    command *, int,
    command *,
    command *,
    command *);    
    ~testItem();

    void setCmdList(QList<command *> *);  // 设置前置命令列表
    void setDataList(QList<DataItem *> *);  // 设置数据列表
    void setDatalength(int);              // 设置数据长度
    void setSetCmdVerify(command *);      // 设置校准设置命令
    void setSetMulti(int);                // 设置校准数据倍数
    void setDmmCmdVerify(command *);      // 设置校准读取命令
    void setDmmMulti(int);                // 设置校准读取倍数
    void setMeterCmdVerify(command *);    // 设置校准读万用表命令
    void setMeterMulti(int);              // 设置校准万用表倍数
    void setSetCmdTest(command *);        // 设置测试设置命令
    void setDmmCmdTest(command *);        // 设置测试读取命令
    void setMeterCmdTest(command *);      // 设置测试读万用表命令

    QList<command *> * getCmdList();      // 获取前置命令列表
    QList<DataItem *> * getDataList();  // 获取数据列表
    int getDataLength();                  // 获取数据长度
    command * getSetCmdVerify();          // 获取校准设置命令
    int getSetMulti();                    // 获取校准数据倍数
    command * getDmmCmdVerify();          // 获取校准读取命令
    int getDmmMulti();                    // 获取校准读取倍数
    command * getMeterCmdVerify();        // 获取校准读万用表命令
    int getMeterMulti();                  // 获取校准万用表倍数
    command * getSetCmdTest();            // 获取测试设置命令
    command * getDmmCmdTest();            // 获取测试读取命令
    command * getMeterCmdTest();          // 获取测试读万用表命令

    testItem * deepcopy();                // 尝试复制

private:
    QList<command *> * cmdList;  // 前置命令列表
    QList<DataItem *> * dataList;  // 数据列表
    int dataLength;              // 数据长度
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

#endif // TESTITEM_H
