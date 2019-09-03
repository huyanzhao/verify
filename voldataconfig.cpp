#include "voldataconfig.h"
#include "ui_voldataconfig.h"
#include "command.h"
#include <QDebug>
#include <QScrollBar>

volDataConfig::volDataConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::volDataConfig)
{
    ui->setupUi(this);
    // ch1
    nowIndexCh1Pre = -1;  // 前置命令框当前选项索引

    nowIndexCh1Data = 0;  // 当前数据项索引
}

volDataConfig::~volDataConfig()
{
    delete ui;
}
// 退出
void volDataConfig::on_pushBtnExit_clicked()
{
    qDebug() << "Exit voltage config";
    this->close();
}
// ch1前置命令页
// 添加按钮
void volDataConfig::on_pushBtnCh1PreAdd_clicked()
{
    command *newCommand = new command(QString("default"));
    qDebug() << "before append new command, size of cmdlist is " << cmdListCh1Pre.size();
    cmdListCh1Pre.append(newCommand);
    qDebug() << "after append new command, size of cmdlist is " << cmdListCh1Pre.size();
    showCh1PreCmdList();
}
// 命令框点击事件
void volDataConfig::on_ch1PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    qDebug() << index.row();

    nowIndexCh1Pre = index.row();  // 保存ch1前置命令页当前命令在列表中的索引
    nowCommand = cmdListCh1Pre.at(nowIndexCh1Pre);  // 保存ch1前置命令页当前命令的指针
    on_pushBtnCh1PreUndo_clicked();
}
// 刷新命令框
void volDataConfig::showCh1PreCmdList()
{
    ui->ch1PreCmdList->clear();
    for(int i=0; i != cmdListCh1Pre.size(); ++i){
        command *curCommand = cmdListCh1Pre.at(i);
        ui->ch1PreCmdList->addItem(curCommand->getFullName());
    }
}
// 删除按钮
void volDataConfig::on_pushBtnCh1PreDel_clicked()
{
    if(nowIndexCh1Pre == -1){
        return ;
    }
    cmdListCh1Pre.removeAt(nowIndexCh1Pre);
    nowIndexCh1Pre = 0;
    nowCommand = cmdListCh1Pre.at(nowIndexCh1Pre);
    showCh1PreCmdList();
}
// 向上按钮
void volDataConfig::on_pushBtnCh1PreUp_clicked()
{
    if(nowIndexCh1Pre < 1)
        return;
    cmdListCh1Pre.swap(nowIndexCh1Pre, nowIndexCh1Pre-1);
    nowIndexCh1Pre--;
    showCh1PreCmdList();
}
// 向下按钮
void volDataConfig::on_pushBtnCh1PreDown_clicked()
{
    if((nowIndexCh1Pre == -1)||(nowIndexCh1Pre >= cmdListCh1Pre.size()-1))
        return;
    cmdListCh1Pre.swap(nowIndexCh1Pre, nowIndexCh1Pre+1);
    nowIndexCh1Pre++;
    showCh1PreCmdList();
}
// 保存按钮
void volDataConfig::on_pushBtnCh1PreSave_clicked()
{
    nowCommand->setName(ui->lineEditCh1PreCmdName->text());
    nowCommand->setParam(ui->lineEditCh1PreParam->text());
    nowCommand->setStart(ui->lineEditCh1PreStart->text());
    nowCommand->setEnd(ui->lineEditCh1PreEnd->text());
    nowCommand->setJudge(ui->lineEditCh1PreJudge->text());
    showCh1PreCmdList();
}
// 撤消按钮
void volDataConfig::on_pushBtnCh1PreUndo_clicked()
{
    // 清空当前命令详情
    ui->lineEditCh1PreCmdName->clear();
    ui->lineEditCh1PreParam->clear();
    ui->lineEditCh1PreStart->clear();
    ui->lineEditCh1PreEnd->clear();
    ui->lineEditCh1PreJudge->clear();
    // 显示命令名称
    ui->lineEditCh1PreCmdName->setText(nowCommand->getName());
    // 显示命令参数
    QString param = nowCommand->getParam();
    if(param.size() == 0){
        ui->lineEditCh1PreParam->setPlaceholderText(tr("该命令暂时无参数"));
    }else{
        ui->lineEditCh1PreParam->setText(param);
    }
    // 显示截取参数
    ui->lineEditCh1PreStart->setText(nowCommand->getStart());
    ui->lineEditCh1PreEnd->setText(nowCommand->getEnd());
    // 显示判等参数
    ui->lineEditCh1PreJudge->setText(nowCommand->getJudge());
}

// ch1数据页
// 添加按钮
void volDataConfig::on_pushBtnCh1DataAdd_clicked()
{
    int x, y;
    x = nowIndexCh1Data / 10;
    y = nowIndexCh1Data % 10;
    qDebug() << x << y;
    ui->scrollAreaWidgetContentsCh1->setFixedWidth((x+1)*180);  // 重置滚动区域大小
    QScrollBar *pScrollBar = ui->scrollAreaCh1Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // 自动滚动到最右边
    }
    // 框
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsCh1);
    newframe->setGeometry(QRect(x*180, y*30+2, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("frameCh1Data_%1").arg(nowIndexCh1Data+1));
    frameListCh1Data.append(newframe);
    newframe->show();
    // 复选框
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexCh1Data+1));
    newcheckbox->setObjectName(QString("checkBoxCh1Data_%1").arg(nowIndexCh1Data+1));
    checkBoxListCh1Data.append(newcheckbox);
    newcheckbox->show();
    // 数据框
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("数据"));
    datalineedit->setObjectName(QString("lineEditCh1Data_%1").arg(nowIndexCh1Data+1));
    dataLineEditListCh1Data.append(datalineedit);
    datalineedit->show();
    // 地址框
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("地址"));
    addrlineedit->setObjectName(QString("lineEditCh1Data_%1").arg(nowIndexCh1Data+1));
    addrLineEditListCh1Data.append(addrlineedit);
    addrlineedit->show();
    qDebug() << "框个数:" << frameListCh1Data.size()
             << "复选框个数:" << checkBoxListCh1Data.size()
             << "数据框个数:" << dataLineEditListCh1Data.size()
             << "地址框个数:" << addrLineEditListCh1Data.size();



    nowIndexCh1Data++;
}
