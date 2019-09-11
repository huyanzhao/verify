#ifndef VOLDATACONFIG_H
#define VOLDATACONFIG_H

#include <QDialog>
#include <QModelIndex>
#include "command.h"
#include <QFrame>
#include <QCheckBox>
#include <QLineEdit>
#include <QPair>

namespace Ui {
class volDataConfig;
}

class volDataConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit volDataConfig(QWidget *parent = 0);
    ~volDataConfig();
    
private slots:
    void on_pushBtnExit_clicked();  // 退出按钮
    // ch1前置命令页
    void on_pushBtnCh1PreAdd_clicked();        // 添加按钮
    void on_ch1PreCmdList_clicked(const QModelIndex &index);  // 命令列表
    void showCh1PreCmdList();                  // 刷新命令列表框
    void on_pushBtnCh1PreDel_clicked();        // 删除按钮
    void on_pushBtnCh1PreUp_clicked();         // 向上按钮
    void on_pushBtnCh1PreDown_clicked();       // 向下按钮
    void on_pushBtnCh1PreSave_clicked();       // 保存按钮
    void on_pushBtnCh1PreUndo_clicked();       // 撤消按钮
    // ch1数据页
    void on_pushBtnCh1DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectCh1();               // 判断全选状态
    void on_pushBtnCh1DataAll_clicked();       // 全选按钮
    void on_pushBtnCh1DataDel_clicked();       // 删除按钮
    void on_pushBtnCh1DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnCh1DataClear_clicked();     // 清空数据按钮
    void on_pushBtnCh1DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsCh1(int, double, double, QString, int, bool);  // 处理接收到的批量添加参数
    void on_pushBtnCh1DataSave_clicked();      // 保存按钮
    void on_pushBtnCh1DataUndo_clicked();      // 撤消按钮
    // ch1校准页
    void on_pushBtnCh1VerifySave_clicked();    // 保存按钮
    void on_pushBtnCh1VerifyUndo_clicked();    // 撤消按钮
    // ch1测试页
    void on_pushBtnCh1TestSave_clicked();      // 保存按钮
    void on_pushBtnCh1TestUndo_clicked();      // 撤消按钮

    // ch2前置命令页
    void on_pushBtnCh2PreAdd_clicked();        // 添加按钮
    void on_ch2PreCmdList_clicked(const QModelIndex &index);  // 命令列表
    void showCh2PreCmdList();                  // 刷新命令列表框
    void on_pushBtnCh2PreDel_clicked();        // 删除按钮
    void on_pushBtnCh2PreUp_clicked();         // 向上按钮
    void on_pushBtnCh2PreDown_clicked();       // 向下按钮
    void on_pushBtnCh2PreSave_clicked();       // 保存按钮
    void on_pushBtnCh2PreUndo_clicked();       // 撤消按钮
    // ch2数据页
    void on_pushBtnCh2DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectCh2();               // 判断全选状态
    void on_pushBtnCh2DataAll_clicked();       // 全选按钮
    void on_pushBtnCh2DataDel_clicked();       // 删除按钮
    void on_pushBtnCh2DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnCh2DataClear_clicked();     // 清空数据按钮
    void on_pushBtnCh2DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsCh2(int, double, double, QString, int, bool);  // 处理接收到的批量添加参数
    void on_pushBtnCh2DataSave_clicked();      // 保存按钮
    void on_pushBtnCh2DataUndo_clicked();      // 撤消按钮
    // ch2校准页
    void on_pushBtnCh2VerifySave_clicked();    // 保存按钮
    void on_pushBtnCh2VerifyUndo_clicked();    // 撤消按钮
    // Ch2测试页
    void on_pushBtnCh2TestSave_clicked();      // 保存按钮
    void on_pushBtnCh2TestUndo_clicked();      // 撤消按钮

private:
    Ui::volDataConfig *ui;
    // ch1前置命令页
    QList<command *>  cmdListCh1Pre;             // 命令列表
    int nowIndexCh1Pre;                          // 当前命令索引
    command * nowCommandCh1;                     // 当前命令指针
    // ch1数据页
    int nowIndexCh1Data;                         // 当前数据索引
    QList<QFrame *> frameListCh1Data;            // 框列表
    QList<QCheckBox *> checkBoxListCh1Data;      // 复选框列表
    QList<QLineEdit *> dataLineEditListCh1Data;  // 数据框列表
    QList<QLineEdit *> addrLineEditListCh1Data;  // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListCh1; // 参数列表
    // ch1校准页
    command * setCmdCh1Verify;                   // 校准设置电压命令
    int setCh1Multi;                             // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdCh1Verify;                   // 校准读取电压命令
    int dmmCh1Multi;                             // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdCh1Verify;                 // 校准读万用表命令
    int meterCh1Multi;                           // 写到eeprom时，万用表返回结果要放大的倍数
    // ch1测试页
    command * setCmdCh1Test;                     // 测试设置电压命令
    command * dmmCmdCh1Test;                     // 测试读取电压命令
    command * meterCmdCh1Test;                   // 测试读万用表命令

    // ch2前置命令页
    QList<command *>  cmdListCh2Pre;             // 命令列表
    int nowIndexCh2Pre;                          // 当前命令索引
    command * nowCommandCh2;                     // 当前命令指针
    // ch2数据页
    int nowIndexCh2Data;                         // 当前数据索引
    QList<QFrame *> frameListCh2Data;            // 框列表
    QList<QCheckBox *> checkBoxListCh2Data;      // 复选框列表
    QList<QLineEdit *> dataLineEditListCh2Data;  // 数据框列表
    QList<QLineEdit *> addrLineEditListCh2Data;  // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListCh2; // 参数列表
    // ch2校准页
    command * setCmdCh2Verify;                   // 校准设置电压命令
    int setCh2Multi;                             // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdCh2Verify;                   // 校准读取电压命令
    int dmmCh2Multi;                             // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdCh2Verify;                 // 校准读万用表命令
    int meterCh2Multi;                           // 写到eeprom时，万用表返回结果要放大的倍数
    // Ch2测试页
    command * setCmdCh2Test;                     // 测试设置电压命令
    command * dmmCmdCh2Test;                     // 测试读取电压命令
    command * meterCmdCh2Test;                   // 测试读万用表命令
};

#endif // VOLDATACONFIG_H
