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
    nowIndexCh1Pre = -1;  // ǰ�������ǰѡ������

    nowIndexCh1Data = 0;  // ��ǰ����������
}

volDataConfig::~volDataConfig()
{
    delete ui;
}
// �˳�
void volDataConfig::on_pushBtnExit_clicked()
{
    qDebug() << "Exit voltage config";
    this->close();
}
// ch1ǰ������ҳ
// ��Ӱ�ť
void volDataConfig::on_pushBtnCh1PreAdd_clicked()
{
    command *newCommand = new command(QString("default"));
    qDebug() << "before append new command, size of cmdlist is " << cmdListCh1Pre.size();
    cmdListCh1Pre.append(newCommand);
    qDebug() << "after append new command, size of cmdlist is " << cmdListCh1Pre.size();
    showCh1PreCmdList();
}
// ��������¼�
void volDataConfig::on_ch1PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    qDebug() << index.row();

    nowIndexCh1Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommand = cmdListCh1Pre.at(nowIndexCh1Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnCh1PreUndo_clicked();
}
// ˢ�������
void volDataConfig::showCh1PreCmdList()
{
    ui->ch1PreCmdList->clear();
    for(int i=0; i != cmdListCh1Pre.size(); ++i){
        command *curCommand = cmdListCh1Pre.at(i);
        ui->ch1PreCmdList->addItem(curCommand->getFullName());
    }
}
// ɾ����ť
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
// ���ϰ�ť
void volDataConfig::on_pushBtnCh1PreUp_clicked()
{
    if(nowIndexCh1Pre < 1)
        return;
    cmdListCh1Pre.swap(nowIndexCh1Pre, nowIndexCh1Pre-1);
    nowIndexCh1Pre--;
    showCh1PreCmdList();
}
// ���°�ť
void volDataConfig::on_pushBtnCh1PreDown_clicked()
{
    if((nowIndexCh1Pre == -1)||(nowIndexCh1Pre >= cmdListCh1Pre.size()-1))
        return;
    cmdListCh1Pre.swap(nowIndexCh1Pre, nowIndexCh1Pre+1);
    nowIndexCh1Pre++;
    showCh1PreCmdList();
}
// ���水ť
void volDataConfig::on_pushBtnCh1PreSave_clicked()
{
    nowCommand->setName(ui->lineEditCh1PreCmdName->text());
    nowCommand->setParam(ui->lineEditCh1PreParam->text());
    nowCommand->setStart(ui->lineEditCh1PreStart->text());
    nowCommand->setEnd(ui->lineEditCh1PreEnd->text());
    nowCommand->setJudge(ui->lineEditCh1PreJudge->text());
    showCh1PreCmdList();
}
// ������ť
void volDataConfig::on_pushBtnCh1PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditCh1PreCmdName->clear();
    ui->lineEditCh1PreParam->clear();
    ui->lineEditCh1PreStart->clear();
    ui->lineEditCh1PreEnd->clear();
    ui->lineEditCh1PreJudge->clear();
    // ��ʾ��������
    ui->lineEditCh1PreCmdName->setText(nowCommand->getName());
    // ��ʾ�������
    QString param = nowCommand->getParam();
    if(param.size() == 0){
        ui->lineEditCh1PreParam->setPlaceholderText(tr("��������ʱ�޲���"));
    }else{
        ui->lineEditCh1PreParam->setText(param);
    }
    // ��ʾ��ȡ����
    ui->lineEditCh1PreStart->setText(nowCommand->getStart());
    ui->lineEditCh1PreEnd->setText(nowCommand->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditCh1PreJudge->setText(nowCommand->getJudge());
}

// ch1����ҳ
// ��Ӱ�ť
void volDataConfig::on_pushBtnCh1DataAdd_clicked()
{
    int x, y;
    x = nowIndexCh1Data / 10;
    y = nowIndexCh1Data % 10;
    qDebug() << x << y;
    ui->scrollAreaWidgetContentsCh1->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaCh1Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsCh1);
    newframe->setGeometry(QRect(x*180, y*30+2, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("frameCh1Data_%1").arg(nowIndexCh1Data+1));
    frameListCh1Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexCh1Data+1));
    newcheckbox->setObjectName(QString("checkBoxCh1Data_%1").arg(nowIndexCh1Data+1));
    checkBoxListCh1Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setObjectName(QString("lineEditCh1Data_%1").arg(nowIndexCh1Data+1));
    dataLineEditListCh1Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditCh1Data_%1").arg(nowIndexCh1Data+1));
    addrLineEditListCh1Data.append(addrlineedit);
    addrlineedit->show();
    qDebug() << "�����:" << frameListCh1Data.size()
             << "��ѡ�����:" << checkBoxListCh1Data.size()
             << "���ݿ����:" << dataLineEditListCh1Data.size()
             << "��ַ�����:" << addrLineEditListCh1Data.size();



    nowIndexCh1Data++;
}
