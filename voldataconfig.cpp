#include "voldataconfig.h"
#include "ui_voldataconfig.h"
#include "command.h"
#include <QDebug>
#include <QScrollBar>
#include <QMessageBox>
#include <QLatin1Char>
#include <QDoubleValidator>
#include <QPair>
#include "batchadd.h"
#include "qstringinthex.h"

volDataConfig::volDataConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::volDataConfig)
{
    ui->setupUi(this);
    // ch1
    nowIndexCh1Pre = -1;  // 前置命令框当前选项索引

    nowIndexCh1Data = 0;  // 当前数据项索引
}
// 析构
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
    nowIndexCh1Pre = cmdListCh1Pre.size()-1;
    if(nowIndexCh1Pre != -1)
        nowCommand = cmdListCh1Pre.at(nowIndexCh1Pre);
    showCh1PreCmdList();
    // 清空当前命令详情
    ui->lineEditCh1PreCmdName->clear();
    ui->lineEditCh1PreParam->clear();
    ui->lineEditCh1PreStart->clear();
    ui->lineEditCh1PreEnd->clear();
    ui->lineEditCh1PreJudge->clear();
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
    if(ui->lineEditCh1PreCmdName->text().size() == 0 &&
            ui->lineEditCh1PreParam->text().size() == 0 &&
            ui->lineEditCh1PreStart->text().size() == 0 &&
            ui->lineEditCh1PreEnd->text().size() == 0 &&
            ui->lineEditCh1PreJudge->text().size() == 0)
        return;
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
    ui->lineEditCh1PreCmdName->setPlaceholderText(tr("命令名称"));
    ui->lineEditCh1PreCmdName->setText(nowCommand->getName());
    // 显示命令参数
    ui->lineEditCh1PreParam->setPlaceholderText(tr("参数"));
    ui->lineEditCh1PreParam->setText(nowCommand->getParam());

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
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
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
//    qDebug() << "frame number:" << frameListCh1Data.size()
//             << "checkBox number:" << checkBoxListCh1Data.size()
//             << "data frame number:" << dataLineEditListCh1Data.size()
//             << "address frame number:" << addrLineEditListCh1Data.size();
    nowIndexCh1Data++;
}
// 判断全选状态
bool volDataConfig::checkBoxAllSelect()
{
    for(int i=0; i != checkBoxListCh1Data.size(); ++i){
        if(!checkBoxListCh1Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// 全选按钮
void volDataConfig::on_pushBtnCh1DataAll_clicked()
{
    if(checkBoxAllSelect()){  // 已经全选, 设置为全不选
        for(int i=0; i != checkBoxListCh1Data.size(); ++i){
            checkBoxListCh1Data.at(i)->setChecked(false);
        }
    }else { // 未全选,设置为全选
        for(int i=0; i != checkBoxListCh1Data.size(); ++i){
            checkBoxListCh1Data.at(i)->setChecked(true);
        }
    }
}
// 删除按钮
void volDataConfig::on_pushBtnCh1DataDel_clicked()
{
    for(int i=0; i != checkBoxListCh1Data.size();){
        if(checkBoxListCh1Data.at(i)->isChecked()){  // 如果被选中,则删除该数据项
            checkBoxListCh1Data.removeAt(i);
            dataLineEditListCh1Data.removeAt(i);
            addrLineEditListCh1Data.removeAt(i);
            QFrame *tempFrame = frameListCh1Data.at(i);
            frameListCh1Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // 重新排列框
    int x, y;
    for(int i=0; i != frameListCh1Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListCh1Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListCh1Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexCh1Data = checkBoxListCh1Data.size();
    int column = nowIndexCh1Data/10 + (nowIndexCh1Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsCh1->setFixedWidth(column*180);  // 重置滚动区域大小
    QScrollBar *pScrollBar = ui->scrollAreaCh1Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // 自动滚动到最右边
    }
}
// 全部删除按钮
void volDataConfig::on_pushBtnCh1DataBatchDel_clicked()
{
    if(!checkBoxAllSelect())  // 没有全选,则全选
        on_pushBtnCh1DataAll_clicked();
    on_pushBtnCh1DataDel_clicked(); // 删除
}
// 清空数据按钮
void volDataConfig::on_pushBtnCh1DataClear_clicked()
{
    for(int i=0; i != nowIndexCh1Data; ++i){
        dataLineEditListCh1Data.at(i)->clear();
        addrLineEditListCh1Data.at(i)->clear();
    }
}
// 批量添加按钮
void volDataConfig::on_pushBtnCh1DataBatchAdd_clicked()
{
    if(nowIndexCh1Data)
        if(QMessageBox::question(this, tr("询问"), tr("是否保留当前已填数据？"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnCh1DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int)),
            this, SLOT(handleBatchParams(int, double, double, QString, int)));
    batchdialog->show();
}
// 处理接收到的批量添加参数
void volDataConfig::handleBatchParams(int num, double dataStart, double dataStep, QString strAddrStart, int addrStep)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
    qDebug() << QString("0x%1").arg(0xff,4,16,QLatin1Char('0'));
    qDebug() << "now index of ch1 data list: " << nowIndexCh1Data;
    bool isHex;
    int addrStart;
    if(QStringIsBase16Int(strAddrStart)){
        isHex = true;
        bool ok;
        addrStart = strAddrStart.toInt(&ok, 16);
    }else{
        isHex = false;
        addrStart = strAddrStart.toInt();
    }
    for(int i=0; i != num; ++i){
        on_pushBtnCh1DataAdd_clicked();
        QString strData, strAddr;
        strData = QString("%1").arg(dataStart+dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }        
        dataLineEditListCh1Data.at(nowIndexCh1Data-1)->setText(strData);
        addrLineEditListCh1Data.at(nowIndexCh1Data-1)->setText(strAddr);
    }
}
// 保存按钮
void volDataConfig::on_pushBtnCh1DataSave_clicked()
{
    QList<QPair<QString, QString>* >  *tempList = new QList<QPair<QString, QString>* >;
    // 保存界面中的数据, 判断里面的参数是否合法
    qDebug() << "owIndexCh1Data= " << nowIndexCh1Data;
    for(int i=0; i != nowIndexCh1Data; ++i){
        qDebug() << "i= " << i;
        QString data, addr;
        data = dataLineEditListCh1Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("错误"), tr("第%1项数据不是有效的数据，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListCh1Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("错误"), tr("第%1项地址不是有效的地址，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new (qMakePair(data, addr));
        tempList->append(&tempPair);
        qDebug() << i << *tempList;
    }
    // 请空当前参数列表
    qDebug() << "size of dataAdnAddrList: " << dataAndAddrList->size();
    for(int i=0; i != dataAndAddrList->size(); ++i){
        QPair<QString, QString> * temp;
        temp = dataAndAddrList->at(i);
        dataAndAddrList->removeAt(i);
        delete temp;
    }
    dataAndAddrList = tempList;
}
