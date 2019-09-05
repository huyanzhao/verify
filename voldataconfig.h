#ifndef VOLDATACONFIG_H
#define VOLDATACONFIG_H

#include <QDialog>
#include <QModelIndex>
#include "command.h"
#include <QFrame>
#include <QCheckBox>
#include <QLineEdit>

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
    // 退出按钮
    void on_pushBtnExit_clicked();

    // ch1前置命令页
    // 添加按钮
    void on_pushBtnCh1PreAdd_clicked();
    // 命令列表
    void on_ch1PreCmdList_clicked(const QModelIndex &index);
    // 刷新命令列表框
    void showCh1PreCmdList();
    // 删除按钮
    void on_pushBtnCh1PreDel_clicked();
    // 向上按钮
    void on_pushBtnCh1PreUp_clicked();
    // 向下按钮
    void on_pushBtnCh1PreDown_clicked();
    // 保存按钮
    void on_pushBtnCh1PreSave_clicked();
    // 撤消按钮
    void on_pushBtnCh1PreUndo_clicked();

    // ch1数据页
    // 添加按钮
    void on_pushBtnCh1DataAdd_clicked();
    // 判断全选状态
    bool checkBoxAllSelect();
    // 全选按钮
    void on_pushBtnCh1DataAll_clicked();
    // 删除按钮
    void on_pushBtnCh1DataDel_clicked();
    // 全部删除按钮
    void on_pushBtnCh1DataBatchDel_clicked();
    // 清空数据按钮
    void on_pushBtnCh1DataClear_clicked();
    // 批量添加按钮
    void on_pushBtnCh1DataBatchAdd_clicked();
    // 处理接收到的批量添加参数
    void handleBatchParams(int, int, int, QString, QString);

private:
    Ui::volDataConfig *ui;
    // ch1前置命令页
    QList<command *>  cmdListCh1Pre;  // 命令列表
    int nowIndexCh1Pre;  // 当前命令索引
    command * nowCommand;  // 当前命令指针
    // ch1数据页
    int nowIndexCh1Data;  // 当前数据索引
    QList<QFrame *> frameListCh1Data;  // 框列表
    QList<QCheckBox *> checkBoxListCh1Data; // 复选框列表
    QList<QLineEdit *> dataLineEditListCh1Data; // 数据框列表
    QList<QLineEdit *> addrLineEditListCh1Data; // 地址框列表

};

#endif // VOLDATACONFIG_H
