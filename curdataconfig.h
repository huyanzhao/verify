#ifndef CURDATACONFIG_H
#define CURDATACONFIG_H

#include <QDialog>
#include <QModelIndex>
#include <QFrame>
#include <QList>
#include <QCheckBox>
#include <QLineEdit>
#include "command.h"
#include "currentitem.h"

namespace Ui {
class curdataconfig;
}

class curdataconfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit curdataconfig(currentItem *, currentItem *, QWidget *parent = 0);
    ~curdataconfig();

signals:
    void returnTestItem(currentItem *, currentItem *);  // 返回设定的参数给主窗口
    
private slots:
    void on_pushBtnExit_clicked(); // 退出
    // PSU1  前置命令
    void on_pushBtnPsu1PreAdd_clicked();             // 添加按钮
    void showPsu1PreCmdList();                       // 刷新命令列表框
    void on_psu1PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu1PreDel_clicked();             // 删除按钮
    void on_pushBtnPsu1PreUp_clicked();              // 向上按钮
    void on_pushBtnPsu1PreDown_clicked();            // 向下按钮
    void on_pushBtnPsu1PreSave_clicked();            // 保存按钮
    void on_pushBtnPsu1PreUndo_clicked();            // 撤消按钮
    // PSU1  Part1  换档命令
    void on_pushBtnPsu1Part1PreAdd_clicked();        // 添加按钮
    void showPsu1Part1PreCmdList();                  // 刷新命令列表框
    void on_psu1Part1PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu1Part1PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu1Part1PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu1Part1PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu1Part1PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu1Part1PreUndo_clicked();       // 撤消按钮
    // PSU1 Part1 数据
    void on_pushBtnPsu1Part1DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu1Part1();               // 判断全选状态
    void on_pushBtnPsu1Part1DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu1Part1DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu1Part1DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu1Part1DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu1Part1DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu1Part1(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu1Part1_textEdited();  // 数据长度编辑
    void on_pushBtnPsu1Part1DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part1DataUndo_clicked();      // 撤消按钮
    // PSU1 Part1 校准
    void on_pushBtnPsu1Part1VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu1Part1VerifyUndo_clicked();    // 撤消按钮
    // PSU1  Part1  测试页
    void on_pushBtnPsu1Part1TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part1TestUndo_clicked();      // 撤消按钮
    // PSU1  Part2  换档命令
    void on_pushBtnPsu1Part2PreAdd_clicked();        // 添加按钮
    void showPsu1Part2PreCmdList();                  // 刷新命令列表框
    void on_psu1Part2PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu1Part2PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu1Part2PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu1Part2PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu1Part2PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu1Part2PreUndo_clicked();       // 撤消按钮
    // PSU1 Part2 数据
    void on_pushBtnPsu1Part2DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu1Part2();               // 判断全选状态
    void on_pushBtnPsu1Part2DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu1Part2DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu1Part2DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu1Part2DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu1Part2DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu1Part2(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu1Part2_textEdited();  // 数据长度编辑
    void on_pushBtnPsu1Part2DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part2DataUndo_clicked();      // 撤消按钮
    // PSU1 Part2 校准
    void on_pushBtnPsu1Part2VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu1Part2VerifyUndo_clicked();    // 撤消按钮
    // PSU1  Part2  测试页
    void on_pushBtnPsu1Part2TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part2TestUndo_clicked();      // 撤消按钮
    // PSU1  Part3  换档命令
    void on_pushBtnPsu1Part3PreAdd_clicked();        // 添加按钮
    void showPsu1Part3PreCmdList();                  // 刷新命令列表框
    void on_psu1Part3PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu1Part3PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu1Part3PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu1Part3PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu1Part3PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu1Part3PreUndo_clicked();       // 撤消按钮
    // PSU1 Part3 数据
    void on_pushBtnPsu1Part3DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu1Part3();               // 判断全选状态
    void on_pushBtnPsu1Part3DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu1Part3DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu1Part3DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu1Part3DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu1Part3DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu1Part3(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu1Part3_textEdited();  // 数据长度编辑
    void on_pushBtnPsu1Part3DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part3DataUndo_clicked();      // 撤消按钮
    // PSU1 Part3 校准
    void on_pushBtnPsu1Part3VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu1Part3VerifyUndo_clicked();    // 撤消按钮
    // PSU1  Part3  测试页
    void on_pushBtnPsu1Part3TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part3TestUndo_clicked();      // 撤消按钮
    // PSU1  Part4  换档命令
    void on_pushBtnPsu1Part4PreAdd_clicked();        // 添加按钮
    void showPsu1Part4PreCmdList();                  // 刷新命令列表框
    void on_psu1Part4PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu1Part4PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu1Part4PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu1Part4PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu1Part4PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu1Part4PreUndo_clicked();       // 撤消按钮
    // PSU1 Part4 数据
    void on_pushBtnPsu1Part4DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu1Part4();               // 判断全选状态
    void on_pushBtnPsu1Part4DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu1Part4DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu1Part4DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu1Part4DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu1Part4DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu1Part4(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu1Part4_textEdited();  // 数据长度编辑
    void on_pushBtnPsu1Part4DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part4DataUndo_clicked();      // 撤消按钮
    // PSU1 Part4 校准
    void on_pushBtnPsu1Part4VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu1Part4VerifyUndo_clicked();    // 撤消按钮
    // PSU1  Part4  测试页
    void on_pushBtnPsu1Part4TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part4TestUndo_clicked();      // 撤消按钮
    // PSU1  Part5  换档命令
    void on_pushBtnPsu1Part5PreAdd_clicked();        // 添加按钮
    void showPsu1Part5PreCmdList();                  // 刷新命令列表框
    void on_psu1Part5PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu1Part5PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu1Part5PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu1Part5PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu1Part5PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu1Part5PreUndo_clicked();       // 撤消按钮
    // PSU1 Part5 数据
    void on_pushBtnPsu1Part5DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu1Part5();               // 判断全选状态
    void on_pushBtnPsu1Part5DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu1Part5DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu1Part5DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu1Part5DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu1Part5DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu1Part5(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu1Part5_textEdited();  // 数据长度编辑
    void on_pushBtnPsu1Part5DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part5DataUndo_clicked();      // 撤消按钮
    // PSU1 Part5 校准
    void on_pushBtnPsu1Part5VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu1Part5VerifyUndo_clicked();    // 撤消按钮
    // PSU1  Part5  测试页
    void on_pushBtnPsu1Part5TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu1Part5TestUndo_clicked();      // 撤消按钮
    // PSU2  前置命令
    void on_pushBtnPsu2PreAdd_clicked();             // 添加按钮
    void showPsu2PreCmdList();                       // 刷新命令列表框
    void on_psu2PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu2PreDel_clicked();             // 删除按钮
    void on_pushBtnPsu2PreUp_clicked();              // 向上按钮
    void on_pushBtnPsu2PreDown_clicked();            // 向下按钮
    void on_pushBtnPsu2PreSave_clicked();            // 保存按钮
    void on_pushBtnPsu2PreUndo_clicked();            // 撤消按钮
    // PSU2  Part1  换档命令
    void on_pushBtnPsu2Part1PreAdd_clicked();        // 添加按钮
    void showPsu2Part1PreCmdList();                  // 刷新命令列表框
    void on_psu2Part1PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu2Part1PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu2Part1PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu2Part1PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu2Part1PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu2Part1PreUndo_clicked();       // 撤消按钮
    // PSU2 Part1 数据
    void on_pushBtnPsu2Part1DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu2Part1();               // 判断全选状态
    void on_pushBtnPsu2Part1DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu2Part1DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu2Part1DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu2Part1DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu2Part1DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu2Part1(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu2Part1_textEdited();  // 数据长度编辑
    void on_pushBtnPsu2Part1DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part1DataUndo_clicked();      // 撤消按钮
    // PSU2 Part1 校准
    void on_pushBtnPsu2Part1VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu2Part1VerifyUndo_clicked();    // 撤消按钮
    // PSU2  Part1  测试页
    void on_pushBtnPsu2Part1TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part1TestUndo_clicked();      // 撤消按钮
    // PSU2  Part2  换档命令
    void on_pushBtnPsu2Part2PreAdd_clicked();        // 添加按钮
    void showPsu2Part2PreCmdList();                  // 刷新命令列表框
    void on_psu2Part2PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu2Part2PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu2Part2PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu2Part2PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu2Part2PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu2Part2PreUndo_clicked();       // 撤消按钮
    // PSU2 Part2 数据
    void on_pushBtnPsu2Part2DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu2Part2();               // 判断全选状态
    void on_pushBtnPsu2Part2DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu2Part2DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu2Part2DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu2Part2DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu2Part2DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu2Part2(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu2Part2_textEdited();  // 数据长度编辑
    void on_pushBtnPsu2Part2DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part2DataUndo_clicked();      // 撤消按钮
    // PSU2 Part2 校准
    void on_pushBtnPsu2Part2VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu2Part2VerifyUndo_clicked();    // 撤消按钮
    // PSU2  Part2  测试页
    void on_pushBtnPsu2Part2TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part2TestUndo_clicked();      // 撤消按钮
    // PSU2  Part3  换档命令
    void on_pushBtnPsu2Part3PreAdd_clicked();        // 添加按钮
    void showPsu2Part3PreCmdList();                  // 刷新命令列表框
    void on_psu2Part3PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu2Part3PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu2Part3PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu2Part3PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu2Part3PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu2Part3PreUndo_clicked();       // 撤消按钮
    // PSU2 Part3 数据
    void on_pushBtnPsu2Part3DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu2Part3();               // 判断全选状态
    void on_pushBtnPsu2Part3DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu2Part3DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu2Part3DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu2Part3DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu2Part3DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu2Part3(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu2Part3_textEdited();  // 数据长度编辑
    void on_pushBtnPsu2Part3DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part3DataUndo_clicked();      // 撤消按钮
    // PSU2 Part3 校准
    void on_pushBtnPsu2Part3VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu2Part3VerifyUndo_clicked();    // 撤消按钮
    // PSU2  Part3  测试页
    void on_pushBtnPsu2Part3TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part3TestUndo_clicked();      // 撤消按钮
    // PSU2  Part4  换档命令
    void on_pushBtnPsu2Part4PreAdd_clicked();        // 添加按钮
    void showPsu2Part4PreCmdList();                  // 刷新命令列表框
    void on_psu2Part4PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu2Part4PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu2Part4PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu2Part4PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu2Part4PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu2Part4PreUndo_clicked();       // 撤消按钮
    // PSU2 Part4 数据
    void on_pushBtnPsu2Part4DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu2Part4();               // 判断全选状态
    void on_pushBtnPsu2Part4DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu2Part4DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu2Part4DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu2Part4DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu2Part4DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu2Part4(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu2Part4_textEdited();  // 数据长度编辑
    void on_pushBtnPsu2Part4DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part4DataUndo_clicked();      // 撤消按钮
    // PSU2 Part4 校准
    void on_pushBtnPsu2Part4VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu2Part4VerifyUndo_clicked();    // 撤消按钮
    // PSU2  Part4  测试页
    void on_pushBtnPsu2Part4TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part4TestUndo_clicked();      // 撤消按钮
    // PSU2  Part5  换档命令
    void on_pushBtnPsu2Part5PreAdd_clicked();        // 添加按钮
    void showPsu2Part5PreCmdList();                  // 刷新命令列表框
    void on_psu2Part5PreCmdList_clicked(const QModelIndex &index);  // 命令框点击
    void on_pushBtnPsu2Part5PreDel_clicked();        // 删除按钮
    void on_pushBtnPsu2Part5PreUp_clicked();         // 向上按钮
    void on_pushBtnPsu2Part5PreDown_clicked();       // 向下按钮
    void on_pushBtnPsu2Part5PreSave_clicked();       // 保存按钮
    void on_pushBtnPsu2Part5PreUndo_clicked();       // 撤消按钮
    // PSU2 Part5 数据
    void on_pushBtnPsu2Part5DataAdd_clicked();       // 添加按钮
    bool checkBoxAllSelectPsu2Part5();               // 判断全选状态
    void on_pushBtnPsu2Part5DataAll_clicked();       // 全选按钮
    void on_pushBtnPsu2Part5DataDel_clicked();       // 删除按钮
    void on_pushBtnPsu2Part5DataBatchDel_clicked();  // 全部删除按钮
    void on_pushBtnPsu2Part5DataClear_clicked();     // 清空数据按钮
    void on_pushBtnPsu2Part5DataBatchAdd_clicked();  // 批量添加按钮
    void handleBatchParamsPsu2Part5(int, double, double, QString, bool);  // 处理接收到的批量添加参数
    void on_lineEditDataLengthPsu2Part5_textEdited();  // 数据长度编辑
    void on_pushBtnPsu2Part5DataSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part5DataUndo_clicked();      // 撤消按钮
    // PSU2 Part5 校准
    void on_pushBtnPsu2Part5VerifySave_clicked();    // 保存按钮
    void on_pushBtnPsu2Part5VerifyUndo_clicked();    // 撤消按钮
    // PSU2  Part5  测试页
    void on_pushBtnPsu2Part5TestSave_clicked();      // 保存按钮
    void on_pushBtnPsu2Part5TestUndo_clicked();      // 撤消按钮

    void closeEvent(QCloseEvent *);                  // 退出事件

    void on_pushBtnPsu1ToPsu2_clicked();             // 用psu1覆盖psu2
    void on_pushBtnPsu2ToPsu1_clicked();             // 用psu2覆盖psu1

    void repaintPsu1();                              // 重画psu1界面
    void repaintPsu1Part1();                         // 重画psu1 part1界面
    void repaintPsu1Part2();                         // 重画psu1 part2界面
    void repaintPsu1Part3();                         // 重画psu1 part3界面
    void repaintPsu1Part4();                         // 重画psu1 part4界面
    void repaintPsu1Part5();                         // 重画psu1 part5界面
    void repaintPsu2();                              // 重画psu2界面
    void repaintPsu2Part1();                         // 重画psu2 part1界面
    void repaintPsu2Part2();                         // 重画psu2 part2界面
    void repaintPsu2Part3();                         // 重画psu2 part3界面
    void repaintPsu2Part4();                         // 重画psu2 part4界面
    void repaintPsu2Part5();                         // 重画psu2 part5界面

private:
    Ui::curdataconfig *ui;
    currentItem * itemPsu1;
    currentItem * itemPsu2;
    // PSU1  前置
    QList<command *> * cmdListPsu1Pre;   // 命令列表
    int nowIndexPsu1Pre;                // 当前命令索引
    command * nowCommandPsu1;           // 当前命令指针    
    testItem * psu1Part1;
    testItem * psu1Part2;
    testItem * psu1Part3;
    testItem * psu1Part4;
    testItem * psu1Part5;

    // PSU2  前置
    QList<command *> * cmdListPsu2Pre;   // 命令列表
    int nowIndexPsu2Pre;                // 当前命令索引
    command * nowCommandPsu2;           // 当前命令指针
    testItem * psu2Part1;
    testItem * psu2Part2;
    testItem * psu2Part3;
    testItem * psu2Part4;
    testItem * psu2Part5;

    // PSU1  Part1  换档
    QList<command *> * cmdListPsu1Part1Pre;  // 命令列表
    int nowIndexPsu1Part1Pre;           // 当前命令索引
    command * nowCommandPsu1Part1;      // 当前命令指针
    // PSU1  Part1  数据
    int nowIndexPsu1Part1Data;          // 当前数据索引
    QList<QFrame *> frameListPsu1Part1Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu1Part1Data;  // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu1Part1Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu1Part1Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part1; // 参数列表
    int dataLengthPsu1Part1;  // 数据长度
    // PSU1  Part1  校准
    command * setCmdPsu1Part1Verify;    // 校准设置电压命令
    int setPsu1Part1Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu1Part1Verify;    // 校准读取电压命令
    int dmmPsu1Part1Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu1Part1Verify;  // 校准读万用表命令
    int meterPsu1Part1Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU1  Part1  测试
    command * setCmdPsu1Part1Test;      // 测试设置电流命令
    command * dmmCmdPsu1Part1Test;      // 测试读取电流命令
    command * meterCmdPsu1Part1Test;    // 测试读万用表命令
    // PSU1  Part2  换档
    QList<command *> * cmdListPsu1Part2Pre;  // 命令列表
    int nowIndexPsu1Part2Pre;           // 当前命令索引
    command * nowCommandPsu1Part2;      // 当前命令指针
    // PSU1  Part2  数据
    int nowIndexPsu1Part2Data;          // 当前数据索引
    QList<QFrame *> frameListPsu1Part2Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu1Part2Data;  // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu1Part2Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu1Part2Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part2; // 参数列表
    int dataLengthPsu1Part2;  // 数据长度
    // PSU1   Part2  校准
    command * setCmdPsu1Part2Verify;    // 校准设置电压命令
    int setPsu1Part2Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu1Part2Verify;    // 校准读取电压命令
    int dmmPsu1Part2Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu1Part2Verify;  // 校准读万用表命令
    int meterPsu1Part2Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU1  Part2  测试
    command * setCmdPsu1Part2Test;      // 测试设置电流命令
    command * dmmCmdPsu1Part2Test;      // 测试读取电流命令
    command * meterCmdPsu1Part2Test;    // 测试读万用表命令
    // PSU1  Part3  换档
    QList<command *> * cmdListPsu1Part3Pre;  // 命令列表
    int nowIndexPsu1Part3Pre;           // 当前命令索引
    command * nowCommandPsu1Part3;      // 当前命令指针
    // PSU1  Part3  数据
    int nowIndexPsu1Part3Data;          // 当前数据索引
    QList<QFrame *> frameListPsu1Part3Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu1Part3Data;     // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu1Part3Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu1Part3Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part3; // 参数列表
    int dataLengthPsu1Part3;  // 数据长度
    // PSU1  Part3  校准
    command * setCmdPsu1Part3Verify;    // 校准设置电压命令
    int setPsu1Part3Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu1Part3Verify;    // 校准读取电压命令
    int dmmPsu1Part3Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu1Part3Verify;  // 校准读万用表命令
    int meterPsu1Part3Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU1  Part3  测试
    command * setCmdPsu1Part3Test;      // 测试设置电流命令
    command * dmmCmdPsu1Part3Test;      // 测试读取电流命令
    command * meterCmdPsu1Part3Test;    // 测试读万用表命令
    // PSU1  Part4  换档
    QList<command *> * cmdListPsu1Part4Pre;  // 命令列表
    int nowIndexPsu1Part4Pre;           // 当前命令索引
    command * nowCommandPsu1Part4;      // 当前命令指针
    // PSU1  Part4  数据
    int nowIndexPsu1Part4Data;          // 当前数据索引
    QList<QFrame *> frameListPsu1Part4Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu1Part4Data;  // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu1Part4Data;  // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu1Part4Data;  // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part4; // 参数列表
    int dataLengthPsu1Part4;  // 数据长度
    // PSU1  Part4  校准
    command * setCmdPsu1Part4Verify;    // 校准设置电压命令
    int setPsu1Part4Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu1Part4Verify;    // 校准读取电压命令
    int dmmPsu1Part4Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu1Part4Verify;  // 校准读万用表命令
    int meterPsu1Part4Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU1  Part4  测试
    command * setCmdPsu1Part4Test;      // 测试设置电流命令
    command * dmmCmdPsu1Part4Test;      // 测试读取电流命令
    command * meterCmdPsu1Part4Test;    // 测试读万用表命令
    // PSU1  Part5  换档
    QList<command *> * cmdListPsu1Part5Pre;  // 命令列表
    int nowIndexPsu1Part5Pre;           // 当前命令索引
    command * nowCommandPsu1Part5;      // 当前命令指针
    // PSU1  Part5  数据
    int nowIndexPsu1Part5Data;          // 当前数据索引
    QList<QFrame *> frameListPsu1Part5Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu1Part5Data;  // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu1Part5Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu1Part5Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part5; // 参数列表
    int dataLengthPsu1Part5;  // 数据长度
    // PSU1  Part5  校准
    command * setCmdPsu1Part5Verify;    // 校准设置电压命令
    int setPsu1Part5Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu1Part5Verify;    // 校准读取电压命令
    int dmmPsu1Part5Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu1Part5Verify;  // 校准读万用表命令
    int meterPsu1Part5Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU1  Part5  测试
    command * setCmdPsu1Part5Test;      // 测试设置电流命令
    command * dmmCmdPsu1Part5Test;      // 测试读取电流命令
    command * meterCmdPsu1Part5Test;    // 测试读万用表命令

    // PSU2  Part1  换档
    QList<command *> * cmdListPsu2Part1Pre;  // 命令列表
    int nowIndexPsu2Part1Pre;           // 当前命令索引
    command * nowCommandPsu2Part1;      // 当前命令指针
    // PSU2  Part1  数据
    int nowIndexPsu2Part1Data;          // 当前数据索引
    QList<QFrame *> frameListPsu2Part1Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu2Part1Data; // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu2Part1Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu2Part1Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part1; // 参数列表
    int dataLengthPsu2Part1;            // 数据长度
    // PSU2  Part1  校准
    command * setCmdPsu2Part1Verify;    // 校准设置电压命令
    int setPsu2Part1Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu2Part1Verify;    // 校准读取电压命令
    int dmmPsu2Part1Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu2Part1Verify;  // 校准读万用表命令
    int meterPsu2Part1Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU2  Part1  测试
    command * setCmdPsu2Part1Test;      // 测试设置电流命令
    command * dmmCmdPsu2Part1Test;      // 测试读取电流命令
    command * meterCmdPsu2Part1Test;    // 测试读万用表命令
    // PSU2  Part2  换档
    QList<command *> * cmdListPsu2Part2Pre;  // 命令列表
    int nowIndexPsu2Part2Pre;           // 当前命令索引
    command * nowCommandPsu2Part2;      // 当前命令指针
    // PSU2  Part2  数据
    int nowIndexPsu2Part2Data;          // 当前数据索引
    QList<QFrame *> frameListPsu2Part2Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu2Part2Data; // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu2Part2Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu2Part2Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part2; // 参数列表
    int dataLengthPsu2Part2;            // 数据长度
    // PSU2   Part2  校准
    command * setCmdPsu2Part2Verify;    // 校准设置电压命令
    int setPsu2Part2Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu2Part2Verify;    // 校准读取电压命令
    int dmmPsu2Part2Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu2Part2Verify;  // 校准读万用表命令
    int meterPsu2Part2Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU2  Part2  测试
    command * setCmdPsu2Part2Test;      // 测试设置电流命令
    command * dmmCmdPsu2Part2Test;      // 测试读取电流命令
    command * meterCmdPsu2Part2Test;    // 测试读万用表命令
    // PSU2  Part3  换档
    QList<command *> * cmdListPsu2Part3Pre;  // 命令列表
    int nowIndexPsu2Part3Pre;           // 当前命令索引
    command * nowCommandPsu2Part3;      // 当前命令指针
    // PSU2  Part3  数据
    int nowIndexPsu2Part3Data;          // 当前数据索引
    QList<QFrame *> frameListPsu2Part3Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu2Part3Data; // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu2Part3Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu2Part3Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part3; // 参数列表
    int dataLengthPsu2Part3;            // 数据长度
    // PSU2  Part3  校准
    command * setCmdPsu2Part3Verify;    // 校准设置电压命令
    int setPsu2Part3Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu2Part3Verify;    // 校准读取电压命令
    int dmmPsu2Part3Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu2Part3Verify;  // 校准读万用表命令
    int meterPsu2Part3Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU2  Part3  测试
    command * setCmdPsu2Part3Test;      // 测试设置电流命令
    command * dmmCmdPsu2Part3Test;      // 测试读取电流命令
    command * meterCmdPsu2Part3Test;    // 测试读万用表命令
    // PSU2  Part4  换档
    QList<command *> * cmdListPsu2Part4Pre;  // 命令列表
    int nowIndexPsu2Part4Pre;           // 当前命令索引
    command * nowCommandPsu2Part4;      // 当前命令指针
    // PSU2  Part4  数据
    int nowIndexPsu2Part4Data;          // 当前数据索引
    QList<QFrame *> frameListPsu2Part4Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu2Part4Data; // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu2Part4Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu2Part4Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part4; // 参数列表
    int dataLengthPsu2Part4;            // 数据长度
    // PSU2  Part4  校准
    command * setCmdPsu2Part4Verify;    // 校准设置电压命令
    int setPsu2Part4Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu2Part4Verify;    // 校准读取电压命令
    int dmmPsu2Part4Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu2Part4Verify;  // 校准读万用表命令
    int meterPsu2Part4Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU2  Part4  测试
    command * setCmdPsu2Part4Test;      // 测试设置电流命令
    command * dmmCmdPsu2Part4Test;      // 测试读取电流命令
    command * meterCmdPsu2Part4Test;    // 测试读万用表命令
    // PSU2  Part5  换档
    QList<command *> * cmdListPsu2Part5Pre;  // 命令列表
    int nowIndexPsu2Part5Pre;           // 当前命令索引
    command * nowCommandPsu2Part5;      // 当前命令指针
    // PSU2  Part5  数据
    int nowIndexPsu2Part5Data;          // 当前数据索引
    QList<QFrame *> frameListPsu2Part5Data;  // 框列表
    QList<QCheckBox *> checkBoxListPsu2Part5Data; // 复选框列表
    QList<QLineEdit *> dataLineEditListPsu2Part5Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListPsu2Part5Data; // 地址框列表
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part5; // 参数列表
    int dataLengthPsu2Part5;            // 数据长度
    // PSU2  Part5  校准
    command * setCmdPsu2Part5Verify;    // 校准设置电压命令
    int setPsu2Part5Multi;              // 写到eeprom时，设置电压要放大的倍数
    command * dmmCmdPsu2Part5Verify;    // 校准读取电压命令
    int dmmPsu2Part5Multi;              // 写到eeprom时，读取电压要放大的倍数
    command * meterCmdPsu2Part5Verify;  // 校准读万用表命令
    int meterPsu2Part5Multi;            // 写到eeprom时，万用表返回结果要放大的倍数
    // PSU2  Part5  测试
    command * setCmdPsu2Part5Test;      // 测试设置电流命令
    command * dmmCmdPsu2Part5Test;      // 测试读取电流命令
    command * meterCmdPsu2Part5Test;    // 测试读万用表命令
};

#endif // CURDATACONFIG_H
