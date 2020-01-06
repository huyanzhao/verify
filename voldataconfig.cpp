#include "voldataconfig.h"
#include "ui_voldataconfig.h"
#include <QDebug>
#include <QScrollBar>
#include <QMessageBox>
#include <QLatin1Char>
#include <QDoubleValidator>
#include "batchadd.h"
#include "qstringinthex.h"
#include "command.h"
#include "testitem.h"
#include "dataitem.h"

volDataConfig::volDataConfig(testItem * ch1, testItem * ch2, QWidget *parent) :
    itemCh1(ch1),
    itemCh2(ch2),
    QDialog(parent),
    ui(new Ui::volDataConfig)
{
    ui->setupUi(this);
    // ch1
    ui->lineEditDataLengthCh1->setValidator(new QIntValidator(1, 100, this));
    ui->lineEditCh1VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditCh1VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditCh1VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditCh1VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditCh1VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditCh1TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditCh1TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    repaintCh1();
    // ch2
    ui->lineEditDataLengthCh2->setValidator(new QIntValidator(1, 100, this));
    ui->lineEditCh2VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditCh2VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditCh2VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditCh2VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditCh2VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditCh2TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditCh2TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    repaintCh2();
}
// 析构
volDataConfig::~volDataConfig()
{
    delete ui;
}
// 退出事件
void volDataConfig::closeEvent(QCloseEvent *event)
{
    if(cmdListCh1Pre->size() != 0 && dataAndAddrListCh1->size() != 0)
        itemCh1 = new testItem(cmdListCh1Pre, dataAndAddrListCh1, dataLengthCh1,
                               setCmdCh1Verify, setCh1Multi,
                               dmmCmdCh1Verify, dmmCh1Multi,
                               meterCmdCh1Verify, meterCh1Multi,
                               setCmdCh1Test, dmmCmdCh1Test, meterCmdCh1Test);
    else
        itemCh1 = NULL;
    if(cmdListCh2Pre->size() != 0 && dataAndAddrListCh2 != 0)
        itemCh2 = new testItem(cmdListCh2Pre, dataAndAddrListCh2, dataLengthCh2,
                               setCmdCh2Verify, setCh2Multi,
                               dmmCmdCh2Verify, dmmCh2Multi,
                               meterCmdCh2Verify, meterCh2Multi,
                               setCmdCh2Test, dmmCmdCh2Test, meterCmdCh2Test);
    else
        itemCh2 = NULL;
    emit returnTestItem(itemCh1, itemCh2);
}
// 退出
void volDataConfig::on_pushBtnExit_clicked()
{
    this->close();
}
// ch1前置命令页
// 添加按钮
void volDataConfig::on_pushBtnCh1PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    newCommand->setRatio(0.0);
    cmdListCh1Pre->append(newCommand);
    showCh1PreCmdList();
}
// 命令框点击事件
void volDataConfig::on_ch1PreCmdList_clicked(const QModelIndex &index)
{
    nowIndexCh1Pre = index.row();  // 保存ch1前置命令页当前命令在列表中的索引
    nowCommandCh1 = cmdListCh1Pre->at(nowIndexCh1Pre);  // 保存ch1前置命令页当前命令的指针
    on_pushBtnCh1PreUndo_clicked();
}
// 刷新命令框
void volDataConfig::showCh1PreCmdList()
{
    ui->ch1PreCmdList->clear();
    for(int i=0; i != cmdListCh1Pre->size(); ++i){
        command *curCommand = cmdListCh1Pre->at(i);
        ui->ch1PreCmdList->addItem(curCommand->getFullName());
    }
}
// 删除按钮
void volDataConfig::on_pushBtnCh1PreDel_clicked()
{
    if(nowIndexCh1Pre == -1){
        return ;
    }
    cmdListCh1Pre->removeAt(nowIndexCh1Pre);
    nowIndexCh1Pre = cmdListCh1Pre->size()-1;
    if(nowIndexCh1Pre != -1)
        nowCommandCh1 = cmdListCh1Pre->at(nowIndexCh1Pre);
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
    cmdListCh1Pre->swap(nowIndexCh1Pre, nowIndexCh1Pre-1);
    nowIndexCh1Pre--;
    showCh1PreCmdList();
}
// 向下按钮
void volDataConfig::on_pushBtnCh1PreDown_clicked()
{
    if((nowIndexCh1Pre == -1)||(nowIndexCh1Pre >= cmdListCh1Pre->size()-1))
        return;
    cmdListCh1Pre->swap(nowIndexCh1Pre, nowIndexCh1Pre+1);
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
    if(nowCommandCh1 != NULL){
        nowCommandCh1->setName(ui->lineEditCh1PreCmdName->text());
        nowCommandCh1->setParam(ui->lineEditCh1PreParam->text());
        nowCommandCh1->setStart(ui->lineEditCh1PreStart->text());
        nowCommandCh1->setEnd(ui->lineEditCh1PreEnd->text());
        nowCommandCh1->setJudge(ui->lineEditCh1PreJudge->text());
        showCh1PreCmdList();
    }
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
    ui->lineEditCh1PreCmdName->setText(nowCommandCh1->getName());
    // 显示命令参数
    ui->lineEditCh1PreParam->setPlaceholderText(tr("参数"));
    ui->lineEditCh1PreParam->setText(nowCommandCh1->getParam());

    // 显示截取参数
    ui->lineEditCh1PreStart->setText(nowCommandCh1->getStart());
    ui->lineEditCh1PreEnd->setText(nowCommandCh1->getEnd());
    // 显示判等参数
    ui->lineEditCh1PreJudge->setText(nowCommandCh1->getJudge());
}

// ch1数据页
// 添加按钮
void volDataConfig::on_pushBtnCh1DataAdd_clicked()
{
    int x, y;
    x = nowIndexCh1Data / 10;
    y = nowIndexCh1Data % 10;
    ui->scrollAreaWidgetContentsCh1->setFixedWidth((x+1)*290);  // 重置滚动区域大小
    QScrollBar *pScrollBar = ui->scrollAreaCh1Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // 自动滚动到最右边
    }
    // 框
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsCh1);
    newframe->setGeometry(QRect(x*290, y*30+2, 280, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    frameListCh1Data.append(newframe);
    newframe->show();
    // 复选框
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexCh1Data+1));
    checkBoxListCh1Data.append(newcheckbox);
    newcheckbox->show();
    // 数据框
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 50, 17));
    datalineedit->setPlaceholderText(tr("数据"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    dataLineEditListCh1Data.append(datalineedit);
    datalineedit->show();
    // dac地址框
    QLineEdit * dacaddrlineedit = new QLineEdit(newframe);
    dacaddrlineedit->setGeometry(QRect(115, 2, 50, 17));
    dacaddrlineedit->setPlaceholderText(tr("dacAddr"));
    dacAddrLineEditListCh1Data.append(dacaddrlineedit);
    dacaddrlineedit->show();
    // adc地址框
    QLineEdit * adcaddrlineedit = new QLineEdit(newframe);
    adcaddrlineedit->setGeometry(QRect(170, 2, 50, 17));
    adcaddrlineedit->setPlaceholderText(tr("adcAddr"));
    adcAddrLineEditListCh1Data.append(adcaddrlineedit);
    adcaddrlineedit->show();
    // ref地址框
    QLineEdit * refaddrlineedit = new QLineEdit(newframe);
    refaddrlineedit->setGeometry(QRect(225, 2, 50, 17));
    refaddrlineedit->setPlaceholderText(tr("refAddr"));
    refAddrLineEditListCh1Data.append(refaddrlineedit);
    refaddrlineedit->show();
    nowIndexCh1Data++;
}
// 判断全选状态
bool volDataConfig::checkBoxAllSelectCh1()
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
    if(checkBoxAllSelectCh1()){  // 已经全选, 设置为全不选
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
            dacAddrLineEditListCh1Data.removeAt(i);
            adcAddrLineEditListCh1Data.removeAt(i);
            refAddrLineEditListCh1Data.removeAt(i);
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
        frameListCh1Data.at(i)->setGeometry(QRect(x*290, y*30+2, 280, 21));
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
    if(!checkBoxAllSelectCh1())  // 没有全选,则全选
        on_pushBtnCh1DataAll_clicked();
    on_pushBtnCh1DataDel_clicked(); // 删除
}
// 清空数据按钮
void volDataConfig::on_pushBtnCh1DataClear_clicked()
{
    for(int i=0; i != nowIndexCh1Data; ++i){
        dataLineEditListCh1Data.at(i)->clear();
        dacAddrLineEditListCh1Data.at(i)->clear();
        adcAddrLineEditListCh1Data.at(i)->clear();
        refAddrLineEditListCh1Data.at(i)->clear();
    }
}
// 批量添加按钮
void volDataConfig::on_pushBtnCh1DataBatchAdd_clicked()
{
    if(nowIndexCh1Data)
        if(QMessageBox::question(this, tr("询问"), tr("是否保留当前已填数据？"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnCh1DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, bool)),
            this, SLOT(handleBatchParamsCh1(int, double, double, QString, bool)));
    batchdialog->show();
}
// 处理接收到的批量添加参数
void volDataConfig::handleBatchParamsCh1(int num, double dataStart,
                                      double dataStep, QString strAddrStart, bool isRise)
{
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
        QString strData, strDacAddr, strAdcAddr, strRefAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strDacAddr = QString("0x%1").arg(addrStart+dataLengthCh1*3*i, 4, 16, QLatin1Char('0'));
            strAdcAddr = QString("0x%1").arg(addrStart+dataLengthCh1*3*i+dataLengthCh1, 4, 16, QLatin1Char('0'));
            strRefAddr = QString("0x%1").arg(addrStart+dataLengthCh1*3*i+dataLengthCh1*2, 4, 16, QLatin1Char('0'));
        }else{
            strDacAddr = QString("%1").arg(addrStart+dataLengthCh1*3*i);
            strAdcAddr = QString("%1").arg(addrStart+dataLengthCh1*3*i+dataLengthCh1);
            strRefAddr = QString("%1").arg(addrStart+dataLengthCh1*3*i+dataLengthCh1*2);
        }
        dataLineEditListCh1Data.at(nowIndexCh1Data-1)->setText(strData);
        dacAddrLineEditListCh1Data.at(nowIndexCh1Data-1)->setText(strDacAddr);
        adcAddrLineEditListCh1Data.at(nowIndexCh1Data-1)->setText(strAdcAddr);
        refAddrLineEditListCh1Data.at(nowIndexCh1Data-1)->setText(strRefAddr);
    }
}
// 数据长度编辑
void volDataConfig::on_lineEditDataLengthCh1_textEdited()
{
    qDebug() << ui->lineEditDataLengthCh1->text().length();
    if(ui->lineEditDataLengthCh1->text().isEmpty()){
        QMessageBox::information(this, tr("错误"), tr("数据长度不能为空！"), QMessageBox::Ok);
        ui->lineEditDataLengthCh1->setText(QString("%1").arg(dataLengthCh1));
        return;
    }
    dataLengthCh1 = ui->lineEditDataLengthCh1->text().toInt();
}
// 保存按钮
void volDataConfig::on_pushBtnCh1DataSave_clicked()
{
    QList<DataItem *> * tempList = new QList<DataItem * >;
    // 保存界面中的数据, 判断里面的参数是否合法
    for(int i=0; i != nowIndexCh1Data; ++i){
        QString data, dacAddr, adcAddr, refAddr;
        data = dataLineEditListCh1Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("错误"), tr("第%1项数据不是有效的数据，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        dacAddr = dacAddrLineEditListCh1Data.at(i)->text();
        if(!QStringIsInt(dacAddr)){
            QMessageBox::information(this, tr("错误"), tr("第%1项dac地址不是有效的地址，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        adcAddr = adcAddrLineEditListCh1Data.at(i)->text();
        if(!QStringIsInt(adcAddr)){
            QMessageBox::information(this, tr("错误"), tr("第%1项adc地址不是有效的地址，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        refAddr = refAddrLineEditListCh1Data.at(i)->text();
        if(!QStringIsInt(refAddr)){
            QMessageBox::information(this, tr("错误"), tr("第%1项ref地址不是有效的地址，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        DataItem * temp = new DataItem(checkBoxListCh1Data.at(i)->checkState(), data.toDouble(), dacAddr, adcAddr, refAddr);
        tempList->append(temp);
    }
    // 请空当前参数列表
    for(int i=0; i != dataAndAddrListCh1->size(); ++i)
        delete dataAndAddrListCh1->at(i);
    dataAndAddrListCh1->clear();
    dataAndAddrListCh1 = tempList;
    if(!dataAndAddrListCh1->isEmpty())
        QMessageBox::information(this, tr("保存成功"), tr("保存成功"), QMessageBox::Ok);
}
// 撤消按钮
void volDataConfig::on_pushBtnCh1DataUndo_clicked()
{
    on_pushBtnCh1DataBatchDel_clicked();  // 清空数据列表框
    for(int i=0; i != dataAndAddrListCh1->size(); ++i){
        on_pushBtnCh1DataAdd_clicked();
        checkBoxListCh1Data.at(i)->setChecked(dataAndAddrListCh1->at(i)->check);
        dataLineEditListCh1Data.at(i)->setText(QString("%1").arg(dataAndAddrListCh1->at(i)->data));
        dacAddrLineEditListCh1Data.at(i)->setText(dataAndAddrListCh1->at(i)->dacAddr);
        adcAddrLineEditListCh1Data.at(i)->setText(dataAndAddrListCh1->at(i)->adcAddr);
        refAddrLineEditListCh1Data.at(i)->setText(dataAndAddrListCh1->at(i)->refAddr);
    }
}

// ch1校准页
// 保存按钮
void volDataConfig::on_pushBtnCh1VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // 读取并判断电压设置命令的参数
    setName = ui->lineEditCh1VerifySetCmd->text();
    setStart = ui->lineEditCh1VerifySetStart->text();
    setEnd = ui->lineEditCh1VerifySetEnd->text();
    setJudge = ui->lineEditCh1VerifySetJudge->text();
    setMulti = ui->lineEditCh1VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("设置电压命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // 读取并判断电压读取命令的参数
    dmmName = ui->lineEditCh1VerifyDMMCmd->text();
    dmmParam = ui->lineEditCh1VerifyDMMParam->text();
    dmmStart = ui->lineEditCh1VerifyDMMStart->text();
    dmmEnd = ui->lineEditCh1VerifyDMMEnd->text();
    dmmJudge = ui->lineEditCh1VerifyDMMJudge->text();
    dmmMulti = ui->lineEditCh1VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("读取DMM电压命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // 读取并判断万用表的读取从参数
    meterJudge = ui->lineEditCh1VerifyMeterJudge->text();
    meterMulti = ui->lineEditCh1VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("读取读万用表命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    setCmdCh1Verify->setName(setName);  // 保存设置电压命令参数
    setCmdCh1Verify->setStart(setStart);
    setCmdCh1Verify->setEnd(setEnd);
    setCmdCh1Verify->setJudge(setJudge);
    setCh1Multi = setMulti.toInt();
    dmmCmdCh1Verify->setName(dmmName); // 保存读取电压命令参数
    dmmCmdCh1Verify->setParam(dmmParam);
    dmmCmdCh1Verify->setStart(dmmStart);
    dmmCmdCh1Verify->setEnd(dmmEnd);
    dmmCmdCh1Verify->setRatio(dmmJudge.toDouble());
    dmmCh1Multi = dmmMulti.toInt();
    meterCmdCh1Verify->setRatio(meterJudge.toDouble());  // 保存读取万用表命令参数
    meterCh1Multi = meterMulti.toInt();
    QMessageBox::information(this, tr("保存成功"), tr("保存成功"), QMessageBox::Ok);
}
// 撤消按钮
void volDataConfig::on_pushBtnCh1VerifyUndo_clicked()
{
    // 重写设置电压参数
    ui->lineEditCh1VerifySetCmd->setText(setCmdCh1Verify->getName());
    ui->lineEditCh1VerifySetStart->setText(setCmdCh1Verify->getStart());
    ui->lineEditCh1VerifySetEnd->setText(setCmdCh1Verify->getEnd());
    ui->lineEditCh1VerifySetJudge->setText(setCmdCh1Verify->getJudge());
    ui->lineEditCh1VerifySetMulti->setText(QString("%1").arg(setCh1Multi));
    // 重写读取电压参数
    ui->lineEditCh1VerifyDMMCmd->setText(dmmCmdCh1Verify->getName());
    ui->lineEditCh1VerifyDMMStart->setText(dmmCmdCh1Verify->getStart());
    ui->lineEditCh1VerifyDMMEnd->setText(dmmCmdCh1Verify->getEnd());
    ui->lineEditCh1VerifyDMMJudge->setText(dmmCmdCh1Verify->getRatio());
    ui->lineEditCh1VerifyDMMMulti->setText(QString("%1").arg(dmmCh1Multi));
    // 重写读取万用表参数
    ui->lineEditCh1VerifyMeterJudge->setText(meterCmdCh1Verify->getRatio());
    ui->lineEditCh1VerifyMeterMulti->setText(QString("%1").arg(meterCh1Multi));
}

// ch1测试页
// 保存按钮
void volDataConfig::on_pushBtnCh1TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // 读取并判断电压设置命令的参数
    setName = ui->lineEditCh1TestSetCmd->text();
    setStart = ui->lineEditCh1TestSetStart->text();
    setEnd = ui->lineEditCh1TestSetEnd->text();
    setJudge = ui->lineEditCh1TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("设置电压命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // 读取并判断电压读取命令的参数
    dmmName = ui->lineEditCh1TestDMMCmd->text();
    dmmStart = ui->lineEditCh1TestDMMStart->text();
    dmmEnd = ui->lineEditCh1TestDMMEnd->text();
    dmmJudge = ui->lineEditCh1TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("读取DMM电压命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // 读取并判断万用表的读取从参数
    meterJudge = ui->lineEditCh1TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("读取读万用表命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    setCmdCh1Test->setName(setName);  // 保存设置电压命令参数
    setCmdCh1Test->setStart(setStart);
    setCmdCh1Test->setEnd(setEnd);
    setCmdCh1Test->setJudge(setJudge);
    dmmCmdCh1Test->setName(dmmName);  // 保存读取电压命令参数
    dmmCmdCh1Test->setStart(dmmStart);
    dmmCmdCh1Test->setEnd(dmmEnd);
    dmmCmdCh1Test->setRatio(dmmJudge.toDouble());
    meterCmdCh1Test->setRatio(meterJudge.toDouble());  // 保存读取万用表命令参数
    QMessageBox::information(this, tr("保存成功"), tr("保存成功"), QMessageBox::Ok);
}
// 撤消按钮
void volDataConfig::on_pushBtnCh1TestUndo_clicked()
{
    // 重写设置电压参数
    ui->lineEditCh1TestSetCmd->setText(setCmdCh1Test->getName());
    ui->lineEditCh1TestSetStart->setText(setCmdCh1Test->getStart());
    ui->lineEditCh1TestSetEnd->setText(setCmdCh1Test->getEnd());
    ui->lineEditCh1TestSetJudge->setText(setCmdCh1Test->getJudge());
    // 重写读取电压参数
    ui->lineEditCh1TestDMMCmd->setText(dmmCmdCh1Test->getName());
    ui->lineEditCh1TestDMMStart->setText(dmmCmdCh1Test->getStart());
    ui->lineEditCh1TestDMMEnd->setText(dmmCmdCh1Test->getEnd());
    ui->lineEditCh1TestDMMJudge->setText(dmmCmdCh1Test->getRatio());
    // 重写读取万用表参数
    ui->lineEditCh1TestMeterJudge->setText(meterCmdCh1Test->getRatio());
}

// ch2前置命令页
// 添加按钮
void volDataConfig::on_pushBtnCh2PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    newCommand->setRatio(0.0);
    cmdListCh2Pre->append(newCommand);
    showCh2PreCmdList();
}
// 命令框点击事件
void volDataConfig::on_ch2PreCmdList_clicked(const QModelIndex &index)
{
    nowIndexCh2Pre = index.row();  // 保存ch2前置命令页当前命令在列表中的索引
    nowCommandCh2 = cmdListCh2Pre->at(nowIndexCh2Pre);  // 保存ch2前置命令页当前命令的指针
    on_pushBtnCh2PreUndo_clicked();
}
// 刷新命令框
void volDataConfig::showCh2PreCmdList()
{
    ui->ch2PreCmdList->clear();
    for(int i=0; i != cmdListCh2Pre->size(); ++i){
        command *curCommand = cmdListCh2Pre->at(i);
        ui->ch2PreCmdList->addItem(curCommand->getFullName());
    }
}
// 删除按钮
void volDataConfig::on_pushBtnCh2PreDel_clicked()
{
    if(nowIndexCh2Pre == -1){
        return ;
    }
    cmdListCh2Pre->removeAt(nowIndexCh2Pre);
    nowIndexCh2Pre = cmdListCh2Pre->size()-1;
    if(nowIndexCh2Pre != -1)
        nowCommandCh2 = cmdListCh2Pre->at(nowIndexCh2Pre);
    showCh2PreCmdList();
    // 清空当前命令详情
    ui->lineEditCh2PreCmdName->clear();
    ui->lineEditCh2PreParam->clear();
    ui->lineEditCh2PreStart->clear();
    ui->lineEditCh2PreEnd->clear();
    ui->lineEditCh2PreJudge->clear();
}
// 向上按钮
void volDataConfig::on_pushBtnCh2PreUp_clicked()
{
    if(nowIndexCh2Pre < 1)
        return;
    cmdListCh2Pre->swap(nowIndexCh2Pre, nowIndexCh2Pre-1);
    nowIndexCh2Pre--;
    showCh2PreCmdList();
}
// 向下按钮
void volDataConfig::on_pushBtnCh2PreDown_clicked()
{
    if((nowIndexCh2Pre == -1)||(nowIndexCh2Pre >= cmdListCh2Pre->size()-1))
        return;
    cmdListCh2Pre->swap(nowIndexCh2Pre, nowIndexCh2Pre+1);
    nowIndexCh2Pre++;
    showCh2PreCmdList();
}
// 保存按钮
void volDataConfig::on_pushBtnCh2PreSave_clicked()
{
    if(ui->lineEditCh2PreCmdName->text().size() == 0 &&
            ui->lineEditCh2PreParam->text().size() == 0 &&
            ui->lineEditCh2PreStart->text().size() == 0 &&
            ui->lineEditCh2PreEnd->text().size() == 0 &&
            ui->lineEditCh2PreJudge->text().size() == 0)
        return;
    if(nowCommandCh2 != NULL){
        nowCommandCh2->setName(ui->lineEditCh2PreCmdName->text());
        nowCommandCh2->setParam(ui->lineEditCh2PreParam->text());
        nowCommandCh2->setStart(ui->lineEditCh2PreStart->text());
        nowCommandCh2->setEnd(ui->lineEditCh2PreEnd->text());
        nowCommandCh2->setJudge(ui->lineEditCh2PreJudge->text());
        showCh2PreCmdList();
    }
}
// 撤消按钮
void volDataConfig::on_pushBtnCh2PreUndo_clicked()
{
    // 清空当前命令详情
    ui->lineEditCh2PreCmdName->clear();
    ui->lineEditCh2PreParam->clear();
    ui->lineEditCh2PreStart->clear();
    ui->lineEditCh2PreEnd->clear();
    ui->lineEditCh2PreJudge->clear();
    // 显示命令名称
    ui->lineEditCh2PreCmdName->setPlaceholderText(tr("命令名称"));
    ui->lineEditCh2PreCmdName->setText(nowCommandCh2->getName());
    // 显示命令参数
    ui->lineEditCh2PreParam->setPlaceholderText(tr("参数"));
    ui->lineEditCh2PreParam->setText(nowCommandCh2->getParam());

    // 显示截取参数
    ui->lineEditCh2PreStart->setText(nowCommandCh2->getStart());
    ui->lineEditCh2PreEnd->setText(nowCommandCh2->getEnd());
    // 显示判等参数
    ui->lineEditCh2PreJudge->setText(nowCommandCh2->getJudge());
}

// Ch2数据页
// 添加按钮
void volDataConfig::on_pushBtnCh2DataAdd_clicked()
{
    int x, y;
    x = nowIndexCh2Data / 10;
    y = nowIndexCh2Data % 10;
    ui->scrollAreaWidgetContentsCh2->setFixedWidth((x+1)*290);  // 重置滚动区域大小
    QScrollBar *pScrollBar = ui->scrollAreaCh2Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // 自动滚动到最右边
    }
    // 框
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsCh2);
    newframe->setGeometry(QRect(x*290, y*30+2, 280, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    frameListCh2Data.append(newframe);
    newframe->show();
    // 复选框
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexCh2Data+1));
    checkBoxListCh2Data.append(newcheckbox);
    newcheckbox->show();
    // 数据框
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 50, 17));
    datalineedit->setPlaceholderText(tr("数据"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    dataLineEditListCh2Data.append(datalineedit);
    datalineedit->show();
    // dac地址框
    QLineEdit * dacaddrlineedit = new QLineEdit(newframe);
    dacaddrlineedit->setGeometry(QRect(115, 2, 50, 17));
    dacaddrlineedit->setPlaceholderText(tr("dacAddr"));
    dacAddrLineEditListCh2Data.append(dacaddrlineedit);
    dacaddrlineedit->show();
    // adc地址框
    QLineEdit * adcaddrlineedit = new QLineEdit(newframe);
    adcaddrlineedit->setGeometry(QRect(170, 2, 50, 17));
    adcaddrlineedit->setPlaceholderText(tr("adcAddr"));
    adcAddrLineEditListCh2Data.append(adcaddrlineedit);
    adcaddrlineedit->show();
    // ref地址框
    QLineEdit * refaddrlineedit = new QLineEdit(newframe);
    refaddrlineedit->setGeometry(QRect(225, 2, 50, 17));
    refaddrlineedit->setPlaceholderText(tr("refAddr"));
    refAddrLineEditListCh2Data.append(refaddrlineedit);
    refaddrlineedit->show();
    nowIndexCh2Data++;
}
// 判断全选状态
bool volDataConfig::checkBoxAllSelectCh2()
{
    for(int i=0; i != checkBoxListCh2Data.size(); ++i){
        if(!checkBoxListCh2Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// 全选按钮
void volDataConfig::on_pushBtnCh2DataAll_clicked()
{
    if(checkBoxAllSelectCh2()){  // 已经全选, 设置为全不选
        for(int i=0; i != checkBoxListCh2Data.size(); ++i){
            checkBoxListCh2Data.at(i)->setChecked(false);
        }
    }else { // 未全选,设置为全选
        for(int i=0; i != checkBoxListCh2Data.size(); ++i){
            checkBoxListCh2Data.at(i)->setChecked(true);
        }
    }
}
// 删除按钮
void volDataConfig::on_pushBtnCh2DataDel_clicked()
{
    for(int i=0; i != checkBoxListCh2Data.size();){
        if(checkBoxListCh2Data.at(i)->isChecked()){  // 如果被选中,则删除该数据项
            checkBoxListCh2Data.removeAt(i);
            dataLineEditListCh2Data.removeAt(i);
            dacAddrLineEditListCh2Data.removeAt(i);
            adcAddrLineEditListCh2Data.removeAt(i);
            refAddrLineEditListCh2Data.removeAt(i);
            QFrame *tempFrame = frameListCh2Data.at(i);
            frameListCh2Data.removeAt(i);
            delete tempFrame;
        }else
            ++i;
    }
    // 重新排列框
    int x, y;
    for(int i=0; i != frameListCh2Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListCh2Data.at(i)->setGeometry(QRect(x*290, y*30+2, 280, 21));
        checkBoxListCh2Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexCh2Data = checkBoxListCh2Data.size();
    int column = nowIndexCh2Data/10 + (nowIndexCh2Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsCh2->setFixedWidth(column*280);  // 重置滚动区域大小
    QScrollBar *pScrollBar = ui->scrollAreaCh2Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // 自动滚动到最右边
    }
}
// 全部删除按钮
void volDataConfig::on_pushBtnCh2DataBatchDel_clicked()
{
    if(!checkBoxAllSelectCh2())  // 没有全选,则全选
        on_pushBtnCh2DataAll_clicked();
    on_pushBtnCh2DataDel_clicked(); // 删除
}
// 清空数据按钮
void volDataConfig::on_pushBtnCh2DataClear_clicked()
{
    for(int i=0; i != nowIndexCh2Data; ++i){
        dataLineEditListCh2Data.at(i)->clear();
        dacAddrLineEditListCh2Data.at(i)->clear();
        adcAddrLineEditListCh2Data.at(i)->clear();
        refAddrLineEditListCh2Data.at(i)->clear();
    }
}
// 批量添加按钮
void volDataConfig::on_pushBtnCh2DataBatchAdd_clicked()
{
    if(nowIndexCh2Data)
        if(QMessageBox::question(this, tr("询问"), tr("是否保留当前已填数据？"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnCh2DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, bool)),
            this, SLOT(handleBatchParamsCh2(int, double, double, QString, bool)));
    batchdialog->show();
}
// 处理接收到的批量添加参数
void volDataConfig::handleBatchParamsCh2(int num, double dataStart,
                                      double dataStep, QString strAddrStart, bool isRise)
{
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
        on_pushBtnCh2DataAdd_clicked();
        QString strData, strDacAddr, strAdcAddr, strRefAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strDacAddr = QString("0x%1").arg(addrStart+dataLengthCh2*3*i, 4, 16, QLatin1Char('0'));
            strAdcAddr = QString("0x%1").arg(addrStart+dataLengthCh2*3*i+dataLengthCh2, 4, 16, QLatin1Char('0'));
            strRefAddr = QString("0x%1").arg(addrStart+dataLengthCh2*3*i+dataLengthCh2*2, 4, 16, QLatin1Char('0'));
        }else{
            strDacAddr = QString("%1").arg(addrStart+dataLengthCh2*3*i);
            strAdcAddr = QString("%1").arg(addrStart+dataLengthCh2*3*i+dataLengthCh2);
            strRefAddr = QString("%1").arg(addrStart+dataLengthCh2*3*i+dataLengthCh2*2);
        }
        dataLineEditListCh2Data.at(nowIndexCh2Data-1)->setText(strData);
        dacAddrLineEditListCh2Data.at(nowIndexCh2Data-1)->setText(strDacAddr);
        adcAddrLineEditListCh2Data.at(nowIndexCh2Data-1)->setText(strAdcAddr);
        refAddrLineEditListCh2Data.at(nowIndexCh2Data-1)->setText(strRefAddr);
    }
}
// 数据长度编辑
void volDataConfig::on_lineEditDataLengthCh2_textEdited()
{
    if(ui->lineEditDataLengthCh2->text().isEmpty()){
        QMessageBox::information(this, tr("错误"), tr("数据长度不能为空！"), QMessageBox::Ok);
        ui->lineEditDataLengthCh2->setText(QString("%1").arg(dataLengthCh2));
        return;
    }
    dataLengthCh2 = ui->lineEditDataLengthCh2->text().toInt();
}
// 保存按钮
void volDataConfig::on_pushBtnCh2DataSave_clicked()
{
    QList<DataItem * >  *tempList = new QList<DataItem * >;
    // 保存界面中的数据, 判断里面的参数是否合法
    for(int i=0; i != nowIndexCh2Data; ++i){
        QString data, dacAddr, adcAddr, refAddr;
        data = dataLineEditListCh2Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("错误"), tr("第%1项数据不是有效的数据，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        dacAddr = dacAddrLineEditListCh2Data.at(i)->text();
        if(!QStringIsInt(dacAddr)){
            QMessageBox::information(this, tr("错误"), tr("第%1项dac地址不是有效的地址，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        adcAddr = adcAddrLineEditListCh2Data.at(i)->text();
        if(!QStringIsInt(adcAddr)){
            QMessageBox::information(this, tr("错误"), tr("第%1项dac地址不是有效的地址，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        refAddr = refAddrLineEditListCh2Data.at(i)->text();
        if(!QStringIsInt(refAddr)){
            QMessageBox::information(this, tr("错误"), tr("第%1项dac地址不是有效的地址，保存失败！").arg(i+1), QMessageBox::Ok);
            return;
        }
        tempList->append(new DataItem(checkBoxListCh2Data.at(i)->checkState(), data.toDouble(), dacAddr, adcAddr, refAddr));
    }
    // 请空当前参数列表
    for(int i=0; i != dataAndAddrListCh2->size(); ++i)
        delete dataAndAddrListCh2->at(i);
    dataAndAddrListCh2->clear();
    dataAndAddrListCh2 = tempList;
    if(!dataAndAddrListCh2->isEmpty())
        QMessageBox::information(this, tr("保存成功"), tr("保存成功"), QMessageBox::Ok);
}
// 撤消按钮
void volDataConfig::on_pushBtnCh2DataUndo_clicked()
{
    on_pushBtnCh2DataBatchDel_clicked();  // 清空数据列表框
    for(int i=0; i != dataAndAddrListCh2->size(); ++i){
        on_pushBtnCh2DataAdd_clicked();
        checkBoxListCh2Data.at(i)->setChecked(dataAndAddrListCh2->at(i)->check);
        dataLineEditListCh2Data.at(i)->setText(QString("%1").arg(dataAndAddrListCh2->at(i)->data));
        dacAddrLineEditListCh2Data.at(i)->setText(dataAndAddrListCh2->at(i)->dacAddr);
        adcAddrLineEditListCh2Data.at(i)->setText(dataAndAddrListCh2->at(i)->adcAddr);
        refAddrLineEditListCh2Data.at(i)->setText(dataAndAddrListCh2->at(i)->refAddr);
    }
}

// Ch2校准页
// 保存按钮
void volDataConfig::on_pushBtnCh2VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // 读取并判断电压设置命令的参数
    setName = ui->lineEditCh2VerifySetCmd->text();
    setStart = ui->lineEditCh2VerifySetStart->text();
    setEnd = ui->lineEditCh2VerifySetEnd->text();
    setJudge = ui->lineEditCh2VerifySetJudge->text();
    setMulti = ui->lineEditCh2VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("设置电压命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // 读取并判断电压读取命令的参数
    dmmName = ui->lineEditCh2VerifyDMMCmd->text();
    dmmParam = ui->lineEditCh2VerifyDMMParam->text();
    dmmStart = ui->lineEditCh2VerifyDMMStart->text();
    dmmEnd = ui->lineEditCh2VerifyDMMEnd->text();
    dmmJudge = ui->lineEditCh2VerifyDMMJudge->text();
    dmmMulti = ui->lineEditCh2VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("读取DMM电压命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // 读取并判断万用表的读取从参数
    meterJudge = ui->lineEditCh2VerifyMeterJudge->text();
    meterMulti = ui->lineEditCh2VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("读取读万用表命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    setCmdCh2Verify->setName(setName);  // 保存设置电压命令参数
    setCmdCh2Verify->setStart(setStart);
    setCmdCh2Verify->setEnd(setEnd);
    setCmdCh2Verify->setJudge(setJudge);
    setCh2Multi = setMulti.toInt();
    dmmCmdCh2Verify->setName(dmmName); // 保存读取电压命令参数
    dmmCmdCh2Verify->setParam(dmmParam);
    dmmCmdCh2Verify->setStart(dmmStart);
    dmmCmdCh2Verify->setEnd(dmmEnd);
    dmmCmdCh2Verify->setRatio(dmmJudge.toDouble());
    dmmCh2Multi = dmmMulti.toInt();
    meterCmdCh2Verify->setRatio(meterJudge.toDouble());  // 保存读取万用表命令参数
    meterCh2Multi = meterMulti.toInt();
    QMessageBox::information(this, tr("保存成功"), tr("保存成功"), QMessageBox::Ok);
}
// 撤消按钮
void volDataConfig::on_pushBtnCh2VerifyUndo_clicked()
{
    // 重写设置电压参数
    ui->lineEditCh2VerifySetCmd->setText(setCmdCh2Verify->getName());
    ui->lineEditCh2VerifySetStart->setText(setCmdCh2Verify->getStart());
    ui->lineEditCh2VerifySetEnd->setText(setCmdCh2Verify->getEnd());
    ui->lineEditCh2VerifySetJudge->setText(setCmdCh2Verify->getJudge());
    ui->lineEditCh2VerifySetMulti->setText(QString("%1").arg(setCh2Multi));
    // 重写读取电压参数
    ui->lineEditCh2VerifyDMMCmd->setText(dmmCmdCh2Verify->getName());
    ui->lineEditCh2VerifyDMMStart->setText(dmmCmdCh2Verify->getStart());
    ui->lineEditCh2VerifyDMMEnd->setText(dmmCmdCh2Verify->getEnd());
    ui->lineEditCh2VerifyDMMJudge->setText(dmmCmdCh2Verify->getRatio());
    ui->lineEditCh2VerifyDMMMulti->setText(QString("%1").arg(dmmCh2Multi));
    // 重写读取万用表参数
    ui->lineEditCh2VerifyMeterJudge->setText(meterCmdCh2Verify->getRatio());
    ui->lineEditCh2VerifyMeterMulti->setText(QString("%1").arg(meterCh2Multi));
}

// Ch2测试页
// 保存按钮
void volDataConfig::on_pushBtnCh2TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // 读取并判断电压设置命令的参数
    setName = ui->lineEditCh2TestSetCmd->text();
    setStart = ui->lineEditCh2TestSetStart->text();
    setEnd = ui->lineEditCh2TestSetEnd->text();
    setJudge = ui->lineEditCh2TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("设置电压命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // 读取并判断电压读取命令的参数
    dmmName = ui->lineEditCh2TestDMMCmd->text();
    dmmStart = ui->lineEditCh2TestDMMStart->text();
    dmmEnd = ui->lineEditCh2TestDMMEnd->text();
    dmmJudge = ui->lineEditCh2TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("读取DMM电压命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // 读取并判断万用表的读取从参数
    meterJudge = ui->lineEditCh2TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("保存失败"), tr("读取读万用表命令的参数填写不完整，请重新填写"), QMessageBox::Ok);
        return;
    }
    setCmdCh2Test->setName(setName);  // 保存设置电压命令参数
    setCmdCh2Test->setStart(setStart);
    setCmdCh2Test->setEnd(setEnd);
    setCmdCh2Test->setJudge(setJudge);
    dmmCmdCh2Test->setName(dmmName);  // 保存读取电压命令参数
    dmmCmdCh2Test->setStart(dmmStart);
    dmmCmdCh2Test->setEnd(dmmEnd);
    dmmCmdCh2Test->setRatio(dmmJudge.toDouble());
    meterCmdCh2Test->setRatio(meterJudge.toDouble());  // 保存读取万用表命令参数
    QMessageBox::information(this, tr("保存成功"), tr("保存成功"), QMessageBox::Ok);
}
// 撤消按钮
void volDataConfig::on_pushBtnCh2TestUndo_clicked()
{
    // 重写设置电压参数
    ui->lineEditCh2TestSetCmd->setText(setCmdCh2Test->getName());
    ui->lineEditCh2TestSetStart->setText(setCmdCh2Test->getStart());
    ui->lineEditCh2TestSetEnd->setText(setCmdCh2Test->getEnd());
    ui->lineEditCh2TestSetJudge->setText(setCmdCh2Test->getJudge());
    // 重写读取电压参数
    ui->lineEditCh2TestDMMCmd->setText(dmmCmdCh2Test->getName());
    ui->lineEditCh2TestDMMStart->setText(dmmCmdCh2Test->getStart());
    ui->lineEditCh2TestDMMEnd->setText(dmmCmdCh2Test->getEnd());
    ui->lineEditCh2TestDMMJudge->setText(dmmCmdCh2Test->getRatio());
    // 重写读取万用表参数
    ui->lineEditCh2TestMeterJudge->setText(meterCmdCh2Test->getRatio());
}
// 重画ch1参数配置页面
void volDataConfig::repaintCh1()
{
    nowIndexCh1Pre = -1;  // 前置命令框当前选项索引
    nowCommandCh1 = NULL;
    nowIndexCh1Data = 0;  // 当前数据项索引
    if(itemCh1 == NULL){
        cmdListCh1Pre = new QList<command *>;
        dataAndAddrListCh1 = new QList<DataItem * >;
        dataLengthCh1 = 6;
        setCmdCh1Verify = new command(QString("PSU1_V"));
        setCmdCh1Verify->setStart(QString("("));
        setCmdCh1Verify->setEnd(QString(";"));
        setCmdCh1Verify->setJudge(QString("DONE"));
        setCmdCh1Verify->setRatio(0);
        setCh1Multi = 1;
        dmmCmdCh1Verify = new command(QString("CH1_Voltage_MEASURE_AD"));
        dmmCmdCh1Verify->setStart(QString("("));
        dmmCmdCh1Verify->setEnd(QString(";"));
        dmmCmdCh1Verify->setRatio(10);
        dmmCh1Multi = 1;
        meterCmdCh1Verify = new command(QString("read?"));
        meterCmdCh1Verify->setRatio(10);
        meterCh1Multi = 1;
        setCmdCh1Test = new command(QString("PSU1_V"));
        setCmdCh1Test->setStart(QString("("));
        setCmdCh1Test->setEnd(QString(";"));
        setCmdCh1Test->setJudge(QString("DONE"));
        setCmdCh1Test->setRatio(0);
        dmmCmdCh1Test = new command(QString("CH1_Voltage_MEASURE"));
        dmmCmdCh1Test->setStart(QString("("));
        dmmCmdCh1Test->setEnd(QString(";"));
        dmmCmdCh1Test->setRatio(0.1);
        meterCmdCh1Test = new command(QString("read?"));
        meterCmdCh1Test->setRatio(0.1);
    }else{
        cmdListCh1Pre = itemCh1->getCmdList();  // 初始化前置命令列表并刷新
        dataAndAddrListCh1 = itemCh1->getDataList();  // 初始化并显示数据列表
        dataLengthCh1 = itemCh1->getDataLength();  // 初始化数据长度
        setCmdCh1Verify = itemCh1->getSetCmdVerify();  // 初始化并显示校准页设置电压命令
        setCh1Multi = itemCh1->getSetMulti();   // 放大倍数
        dmmCmdCh1Verify = itemCh1->getDmmCmdVerify();  // 初始化并显示校准页DMM读取电压命令
        dmmCh1Multi = itemCh1->getDmmMulti();  // 放大倍数
        meterCmdCh1Verify = itemCh1->getMeterCmdVerify();  // 初始化并显示校准页万用表读电压命令
        meterCh1Multi = itemCh1->getMeterMulti();  // 放大倍数
        setCmdCh1Test = itemCh1->getSetCmdTest();  // 初始化并显示测试页设置电压命令
        dmmCmdCh1Test = itemCh1->getDmmCmdTest();  // 初始化并显示测试页DMM读取电压命令
        meterCmdCh1Test = itemCh1->getMeterCmdTest();  // 初始化并显示测试页万用表读电压命令
    }
    showCh1PreCmdList();
    for(int i=0; i != dataAndAddrListCh1->size(); ++i){
        on_pushBtnCh1DataAdd_clicked();
        checkBoxListCh1Data.at(i)->setChecked(dataAndAddrListCh1->at(i)->check);
        dataLineEditListCh1Data.at(i)->setText(QString("%1").arg(dataAndAddrListCh1->at(i)->data));
        dacAddrLineEditListCh1Data.at(i)->setText(dataAndAddrListCh1->at(i)->dacAddr);
        adcAddrLineEditListCh1Data.at(i)->setText(dataAndAddrListCh1->at(i)->adcAddr);
        refAddrLineEditListCh1Data.at(i)->setText(dataAndAddrListCh1->at(i)->refAddr);
    }
    ui->lineEditDataLengthCh1->setText(QString("%1").arg(dataLengthCh1));
    ui->lineEditCh1VerifySetCmd->setText(setCmdCh1Verify->getName());
    ui->lineEditCh1VerifySetStart->setText(setCmdCh1Verify->getStart());
    ui->lineEditCh1VerifySetEnd->setText(setCmdCh1Verify->getEnd());
    ui->lineEditCh1VerifySetJudge->setText(setCmdCh1Verify->getJudge());
    ui->lineEditCh1VerifySetMulti->setText(QString("%1").arg(setCh1Multi));
    ui->lineEditCh1VerifyDMMCmd->setText(dmmCmdCh1Verify->getName());
    ui->lineEditCh1VerifyDMMStart->setText(dmmCmdCh1Verify->getStart());
    ui->lineEditCh1VerifyDMMEnd->setText(dmmCmdCh1Verify->getEnd());
    ui->lineEditCh1VerifyDMMJudge->setText(dmmCmdCh1Verify->getRatio());
    ui->lineEditCh1VerifyDMMMulti->setText(QString("%1").arg(dmmCh1Multi));
    ui->lineEditCh1VerifyMeterCmd->setText(meterCmdCh1Verify->getName());
    ui->lineEditCh1VerifyMeterJudge->setText(meterCmdCh1Verify->getRatio());
    ui->lineEditCh1VerifyMeterMulti->setText(QString("%1").arg(meterCh1Multi));
    ui->lineEditCh1TestSetCmd->setText(setCmdCh1Test->getName());
    ui->lineEditCh1TestSetStart->setText(setCmdCh1Test->getStart());
    ui->lineEditCh1TestSetEnd->setText(setCmdCh1Test->getEnd());
    ui->lineEditCh1TestSetJudge->setText(setCmdCh1Test->getJudge());
    ui->lineEditCh1TestDMMCmd->setText(dmmCmdCh1Test->getName());
    ui->lineEditCh1TestDMMStart->setText(dmmCmdCh1Test->getStart());
    ui->lineEditCh1TestDMMEnd->setText(dmmCmdCh1Test->getEnd());
    ui->lineEditCh1TestDMMJudge->setText(dmmCmdCh1Test->getRatio());
    ui->lineEditCh1TestMeterCmd->setText(meterCmdCh1Test->getName());
    ui->lineEditCh1TestMeterJudge->setText(meterCmdCh1Test->getRatio());
}
// 重画ch2参数配置页面
void volDataConfig::repaintCh2()
{
    nowIndexCh2Pre = -1;  // 前置命令框当前选项索引
    nowCommandCh2 = NULL;
    nowIndexCh2Data = 0;  // 当前数据项索引
    if(itemCh2 == NULL){
        cmdListCh2Pre = new QList<command *>;
        dataAndAddrListCh2 = new QList<DataItem * >;
        dataLengthCh2 = 6;
        setCmdCh2Verify = new command(QString("PSU2_V"));
        setCmdCh2Verify->setStart(QString("("));
        setCmdCh2Verify->setEnd(QString(";"));
        setCmdCh2Verify->setJudge(QString("DONE"));
        setCmdCh2Verify->setRatio(0);
        setCh2Multi = 1;
        dmmCmdCh2Verify = new command(QString("CH2_Voltage_MEASURE_AD"));
        dmmCmdCh2Verify->setStart(QString("("));
        dmmCmdCh2Verify->setEnd(QString(";"));
        dmmCmdCh2Verify->setRatio(10);
        dmmCh2Multi = 1;
        meterCmdCh2Verify = new command(QString("read?"));
        meterCmdCh2Verify->setRatio(10);
        meterCh2Multi = 1;
        setCmdCh2Test = new command(QString("PSU2_V"));
        setCmdCh2Test->setStart(QString("("));
        setCmdCh2Test->setEnd(QString(";"));
        setCmdCh2Test->setJudge(QString("DONE"));
        setCmdCh2Test->setRatio(0);
        dmmCmdCh2Test = new command(QString("CH2_Voltage_MEASURE"));
        dmmCmdCh2Test->setStart(QString("("));
        dmmCmdCh2Test->setEnd(QString(";"));
        dmmCmdCh2Test->setRatio(0.1);
        meterCmdCh2Test = new command(QString("read?"));
        meterCmdCh2Test->setRatio(0.1);
    }else{
        cmdListCh2Pre = itemCh2->getCmdList();  // 初始化前置命令列表并刷新
        dataAndAddrListCh2 = itemCh2->getDataList();  // 初始化并显示数据列表
        dataLengthCh2 = itemCh2->getDataLength();  // 初始化数据长度
        setCmdCh2Verify = itemCh2->getSetCmdVerify();  // 初始化并显示校准页设置电压命令
        setCh2Multi = itemCh2->getSetMulti();   // 放大倍数
        dmmCmdCh2Verify = itemCh2->getDmmCmdVerify();  // 初始化并显示校准页DMM读取电压命令
        dmmCh2Multi = itemCh2->getDmmMulti();  // 放大倍数
        meterCmdCh2Verify = itemCh2->getMeterCmdVerify();  // 初始化并显示校准页万用表读电压命令
        meterCh2Multi = itemCh2->getMeterMulti();  // 放大倍数
        setCmdCh2Test = itemCh2->getSetCmdTest();  // 初始化并显示测试页设置电压命令
        dmmCmdCh2Test = itemCh2->getDmmCmdTest();  // 初始化并显示测试页DMM读取电压命令
        meterCmdCh2Test = itemCh2->getMeterCmdTest();  // 初始化并显示测试页万用表读电压命令
    }
    showCh2PreCmdList();
    for(int i=0; i != dataAndAddrListCh2->size(); ++i){
        on_pushBtnCh2DataAdd_clicked();
        checkBoxListCh2Data.at(i)->setChecked(dataAndAddrListCh2->at(i)->check);
        dataLineEditListCh2Data.at(i)->setText(QString("%1").arg(dataAndAddrListCh2->at(i)->data));
        dacAddrLineEditListCh2Data.at(i)->setText(dataAndAddrListCh2->at(i)->dacAddr);
        adcAddrLineEditListCh2Data.at(i)->setText(dataAndAddrListCh2->at(i)->adcAddr);
        refAddrLineEditListCh2Data.at(i)->setText(dataAndAddrListCh2->at(i)->refAddr);
    }
    ui->lineEditDataLengthCh2->setText(QString("%1").arg(dataLengthCh2));
    ui->lineEditCh2VerifySetCmd->setText(setCmdCh2Verify->getName());
    ui->lineEditCh2VerifySetStart->setText(setCmdCh2Verify->getStart());
    ui->lineEditCh2VerifySetEnd->setText(setCmdCh2Verify->getEnd());
    ui->lineEditCh2VerifySetJudge->setText(setCmdCh2Verify->getJudge());
    ui->lineEditCh2VerifySetMulti->setText(QString("%1").arg(setCh2Multi));
    ui->lineEditCh2VerifyDMMCmd->setText(dmmCmdCh2Verify->getName());
    ui->lineEditCh2VerifyDMMStart->setText(dmmCmdCh2Verify->getStart());
    ui->lineEditCh2VerifyDMMEnd->setText(dmmCmdCh2Verify->getEnd());
    ui->lineEditCh2VerifyDMMJudge->setText(dmmCmdCh2Verify->getRatio());
    ui->lineEditCh2VerifyDMMMulti->setText(QString("%1").arg(dmmCh2Multi));
    ui->lineEditCh2VerifyMeterCmd->setText(meterCmdCh2Verify->getName());
    ui->lineEditCh2VerifyMeterJudge->setText(meterCmdCh2Verify->getRatio());
    ui->lineEditCh2VerifyMeterMulti->setText(QString("%1").arg(meterCh2Multi));
    ui->lineEditCh2TestSetCmd->setText(setCmdCh2Test->getName());
    ui->lineEditCh2TestSetStart->setText(setCmdCh2Test->getStart());
    ui->lineEditCh2TestSetEnd->setText(setCmdCh2Test->getEnd());
    ui->lineEditCh2TestSetJudge->setText(setCmdCh2Test->getJudge());
    ui->lineEditCh2TestDMMCmd->setText(dmmCmdCh2Test->getName());
    ui->lineEditCh2TestDMMStart->setText(dmmCmdCh2Test->getStart());
    ui->lineEditCh2TestDMMEnd->setText(dmmCmdCh2Test->getEnd());
    ui->lineEditCh2TestDMMJudge->setText(dmmCmdCh2Test->getRatio());
    ui->lineEditCh2TestMeterCmd->setText(meterCmdCh2Test->getName());
    ui->lineEditCh2TestMeterJudge->setText(meterCmdCh2Test->getRatio());
}
// 用ch1覆盖ch2
void volDataConfig::on_pushBtnCh1ToCh2_clicked()
{
    delete itemCh2;
    itemCh2 = itemCh1->deepcopy();
    on_pushBtnCh2DataBatchDel_clicked();
    repaintCh2();
    QMessageBox::information(this, tr("复制成功"), tr("复制CH1覆盖CH2成功！"),QMessageBox::Ok);
}
// 用ch2覆盖ch1
void volDataConfig::on_pushBtnCh2ToCh1_clicked()
{
    delete itemCh1;
    itemCh1 = itemCh2->deepcopy();
    on_pushBtnCh1DataBatchDel_clicked();
    repaintCh1();
    QMessageBox::information(this, tr("复制成功"), tr("复制CH2覆盖CH1成功！"),QMessageBox::Ok);
}
