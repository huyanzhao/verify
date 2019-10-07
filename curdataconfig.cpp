#include "curdataconfig.h"
#include "ui_curdataconfig.h"
#include <QDebug>
#include <QScrollBar>
#include <QMessageBox>
#include "command.h"
#include "qstringinthex.h"
#include "batchadd.h"
#include "currentitem.h"

curdataconfig::curdataconfig(currentItem * psu1, currentItem *psu2, QWidget *parent) :
    itemPsu1(psu1),
    itemPsu2(psu2),
    QDialog(parent),
    ui(new Ui::curdataconfig)
{
    ui->setupUi(this);

    if(itemPsu1 == NULL){
        // PSU1  ǰ������
        cmdListPsu1Pre = new QList<command *>;
        psu1Part1 = NULL;
        psu1Part2 = NULL;
        psu1Part3 = NULL;
        psu1Part4 = NULL;
        psu1Part5 = NULL;
    }else{
        cmdListPsu1Pre = psu1->getPreCmdList();
        showPsu1PreCmdList();
        psu1Part1 = itemPsu1->getPart1();
        psu1Part2 = itemPsu1->getPart2();
        psu1Part3 = itemPsu1->getPart3();
        psu1Part4 = itemPsu1->getPart4();
        psu1Part5 = itemPsu1->getPart5();
    }
    nowIndexPsu1Pre = -1;  // ǰ�������ǰѡ������
    nowCommandPsu1 = NULL;

    nowIndexPsu1Part1Data = 0;  // ��ǰ����������
    if(psu1Part1 == NULL){
        cmdListPsu1Part1Pre = new QList<command *>;
        dataAndAddrListPsu1Part1 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu1Part1Verify = new command(QString("PSU1_I"));
        setCmdPsu1Part1Verify->setStart(QString("("));
        setCmdPsu1Part1Verify->setEnd(QString(";"));
        setCmdPsu1Part1Verify->setJudge(QString("DONE"));
        setCmdPsu1Part1Verify->setRatio(0);
        setPsu1Part1Multi = 1;
        dmmCmdPsu1Part1Verify = new command(QString("PSU1_CH1_Current_MEASURE_Real"));
        dmmCmdPsu1Part1Verify->setParam(QString("AD"));
        dmmCmdPsu1Part1Verify->setStart(QString("("));
        dmmCmdPsu1Part1Verify->setEnd(QString(";"));
        dmmCmdPsu1Part1Verify->setRatio(10);
        dmmPsu1Part1Multi = 1;
        meterCmdPsu1Part1Verify = new command(QString("read?"));
        meterCmdPsu1Part1Verify->setRatio(10);
        meterPsu1Part1Multi = 1;
        setCmdPsu1Part1Test = new command(QString("PSU1_I"));
        setCmdPsu1Part1Test->setStart(QString("("));
        setCmdPsu1Part1Test->setEnd(QString(";"));
        setCmdPsu1Part1Test->setJudge(QString("DONE"));
        setCmdPsu1Part1Test->setRatio(0);
        dmmCmdPsu1Part1Test = new command(QString("PSU1_CH1_Current_MEASURE_Real"));
        dmmCmdPsu1Part1Test->setStart(QString("("));
        dmmCmdPsu1Part1Test->setEnd(QString(";"));
        dmmCmdPsu1Part1Test->setRatio(0.1);
        meterCmdPsu1Part1Test = new command(QString("read?"));
        meterCmdPsu1Part1Test->setRatio(0.1);
    }else{
        cmdListPsu1Part1Pre = psu1Part1->getCmdList();        
        dataAndAddrListPsu1Part1 = psu1Part1->getDataList();        
        setCmdPsu1Part1Verify = psu1Part1->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����        
        setPsu1Part1Multi = psu1Part1->getSetMulti();   // �Ŵ���        
        dmmCmdPsu1Part1Verify = psu1Part1->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����        
        dmmPsu1Part1Multi = psu1Part1->getDmmMulti();  // �Ŵ���        
        meterCmdPsu1Part1Verify = psu1Part1->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu1Part1Multi = psu1Part1->getMeterMulti();  // �Ŵ���        
        setCmdPsu1Part1Test = psu1Part1->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����        
        dmmCmdPsu1Part1Test = psu1Part1->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����        
        meterCmdPsu1Part1Test = psu1Part1->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����        
    }
    showPsu1Part1PreCmdList();
    for(int i=0; i != dataAndAddrListPsu1Part1->size(); ++i){
        on_pushBtnPsu1Part1DataAdd_clicked();
        checkBoxListPsu1Part1Data.at(i)->setChecked(dataAndAddrListPsu1Part1->at(i)->first);
        dataLineEditListPsu1Part1Data.at(i)->setText(dataAndAddrListPsu1Part1->at(i)->second->first);
        addrLineEditListPsu1Part1Data.at(i)->setText(dataAndAddrListPsu1Part1->at(i)->second->second);
    }
    nowIndexPsu1Part1Pre = -1;  // ���������ǰѡ������
    nowCommandPsu1Part1 = NULL;
    ui->lineEditPsu1Part1VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part1VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part1VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part1VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part1VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu1Part1TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part1TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part1VerifySetCmd->setText(setCmdPsu1Part1Verify->getName());
    ui->lineEditPsu1Part1VerifySetStart->setText(setCmdPsu1Part1Verify->getStart());
    ui->lineEditPsu1Part1VerifySetEnd->setText(setCmdPsu1Part1Verify->getEnd());
    ui->lineEditPsu1Part1VerifySetJudge->setText(setCmdPsu1Part1Verify->getJudge());
    ui->lineEditPsu1Part1VerifySetMulti->setText(QString("%1").arg(setPsu1Part1Multi));
    ui->lineEditPsu1Part1VerifyDMMCmd->setText(dmmCmdPsu1Part1Verify->getName());
    ui->lineEditPsu1Part1VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu1Part1VerifyDMMStart->setText(dmmCmdPsu1Part1Verify->getStart());
    ui->lineEditPsu1Part1VerifyDMMEnd->setText(dmmCmdPsu1Part1Verify->getEnd());
    ui->lineEditPsu1Part1VerifyDMMJudge->setText(dmmCmdPsu1Part1Verify->getRatio());
    ui->lineEditPsu1Part1VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part1Multi));
    ui->lineEditPsu1Part1VerifyMeterCmd->setText(meterCmdPsu1Part1Verify->getName());
    ui->lineEditPsu1Part1VerifyMeterJudge->setText(meterCmdPsu1Part1Verify->getRatio());
    ui->lineEditPsu1Part1VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part1Multi));
    ui->lineEditPsu1Part1TestSetCmd->setText(setCmdPsu1Part1Test->getName());
    ui->lineEditPsu1Part1TestSetStart->setText(setCmdPsu1Part1Test->getStart());
    ui->lineEditPsu1Part1TestSetEnd->setText(setCmdPsu1Part1Test->getEnd());
    ui->lineEditPsu1Part1TestSetJudge->setText(setCmdPsu1Part1Test->getJudge());
    ui->lineEditPsu1Part1TestDMMCmd->setText(dmmCmdPsu1Part1Test->getName());
    ui->lineEditPsu1Part1TestDMMStart->setText(dmmCmdPsu1Part1Test->getStart());
    ui->lineEditPsu1Part1TestDMMEnd->setText(dmmCmdPsu1Part1Test->getEnd());
    ui->lineEditPsu1Part1TestDMMJudge->setText(dmmCmdPsu1Part1Test->getRatio());
    ui->lineEditPsu1Part1TestMeterCmd->setText(meterCmdPsu1Part1Test->getName());
    ui->lineEditPsu1Part1TestMeterJudge->setText(meterCmdPsu1Part1Test->getRatio());

    nowIndexPsu1Part2Data = 0;  // ��ǰ����������
    if(psu1Part2 == NULL){
        cmdListPsu1Part2Pre = new QList<command *>;
        dataAndAddrListPsu1Part2 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu1Part2Verify = new command(QString("PSU1_I"));
        setCmdPsu1Part2Verify->setStart(QString("("));
        setCmdPsu1Part2Verify->setEnd(QString(";"));
        setCmdPsu1Part2Verify->setJudge(QString("DONE"));
        setCmdPsu1Part2Verify->setRatio(0);
        setPsu1Part2Multi = 1;
        dmmCmdPsu1Part2Verify = new command(QString("PSU1_CH2_Current_MEASURE_Real"));
        dmmCmdPsu1Part2Verify->setParam(QString("AD"));
        dmmCmdPsu1Part2Verify->setStart(QString("("));
        dmmCmdPsu1Part2Verify->setEnd(QString(";"));
        dmmCmdPsu1Part2Verify->setRatio(10);
        dmmPsu1Part2Multi = 1;
        meterCmdPsu1Part2Verify = new command(QString("read?"));
        meterCmdPsu1Part2Verify->setRatio(10);
        meterPsu1Part2Multi = 1;
        setCmdPsu1Part2Test = new command(QString("PSU1_I"));
        setCmdPsu1Part2Test->setStart(QString("("));
        setCmdPsu1Part2Test->setEnd(QString(";"));
        setCmdPsu1Part2Test->setJudge(QString("DONE"));
        setCmdPsu1Part2Test->setRatio(0);
        dmmCmdPsu1Part2Test = new command(QString("PSU1_CH2_Current_MEASURE_Real"));
        dmmCmdPsu1Part2Test->setStart(QString("("));
        dmmCmdPsu1Part2Test->setEnd(QString(";"));
        dmmCmdPsu1Part2Test->setRatio(0.1);
        meterCmdPsu1Part2Test = new command(QString("read?"));
        meterCmdPsu1Part2Test->setRatio(0.1);
    }else{
        cmdListPsu1Part2Pre = psu1Part2->getCmdList();
        dataAndAddrListPsu1Part2 = psu1Part2->getDataList();
        setCmdPsu1Part2Verify = psu1Part2->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu1Part2Multi = psu1Part2->getSetMulti();   // �Ŵ���
        dmmCmdPsu1Part2Verify = psu1Part2->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu1Part2Multi = psu1Part2->getDmmMulti();  // �Ŵ���
        meterCmdPsu1Part2Verify = psu1Part2->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu1Part2Multi = psu1Part2->getMeterMulti();  // �Ŵ���
        setCmdPsu1Part2Test = psu1Part2->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu1Part2Test = psu1Part2->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu1Part2Test = psu1Part2->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu1Part2PreCmdList();
    for(int i=0; i != dataAndAddrListPsu1Part2->size(); ++i){
        on_pushBtnPsu1Part2DataAdd_clicked();
        checkBoxListPsu1Part2Data.at(i)->setChecked(dataAndAddrListPsu1Part2->at(i)->first);
        dataLineEditListPsu1Part2Data.at(i)->setText(dataAndAddrListPsu1Part2->at(i)->second->first);
        addrLineEditListPsu1Part2Data.at(i)->setText(dataAndAddrListPsu1Part2->at(i)->second->second);
    }
    nowIndexPsu1Part2Pre = -1;  // ���������ǰѡ������
    nowCommandPsu1Part2 = NULL;
    ui->lineEditPsu1Part2VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part2VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part2VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part2VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part2VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu1Part2TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part2TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part2VerifySetCmd->setText(setCmdPsu1Part2Verify->getName());
    ui->lineEditPsu1Part2VerifySetStart->setText(setCmdPsu1Part2Verify->getStart());
    ui->lineEditPsu1Part2VerifySetEnd->setText(setCmdPsu1Part2Verify->getEnd());
    ui->lineEditPsu1Part2VerifySetJudge->setText(setCmdPsu1Part2Verify->getJudge());
    ui->lineEditPsu1Part2VerifySetMulti->setText(QString("%1").arg(setPsu1Part2Multi));
    ui->lineEditPsu1Part2VerifyDMMCmd->setText(dmmCmdPsu1Part2Verify->getName());
    ui->lineEditPsu1Part2VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu1Part2VerifyDMMStart->setText(dmmCmdPsu1Part2Verify->getStart());
    ui->lineEditPsu1Part2VerifyDMMEnd->setText(dmmCmdPsu1Part2Verify->getEnd());
    ui->lineEditPsu1Part2VerifyDMMJudge->setText(dmmCmdPsu1Part2Verify->getRatio());
    ui->lineEditPsu1Part2VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part2Multi));
    ui->lineEditPsu1Part2VerifyMeterCmd->setText(meterCmdPsu1Part2Verify->getName());
    ui->lineEditPsu1Part2VerifyMeterJudge->setText(meterCmdPsu1Part2Verify->getRatio());
    ui->lineEditPsu1Part2VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part2Multi));
    ui->lineEditPsu1Part2TestSetCmd->setText(setCmdPsu1Part2Test->getName());
    ui->lineEditPsu1Part2TestSetStart->setText(setCmdPsu1Part2Test->getStart());
    ui->lineEditPsu1Part2TestSetEnd->setText(setCmdPsu1Part2Test->getEnd());
    ui->lineEditPsu1Part2TestSetJudge->setText(setCmdPsu1Part2Test->getJudge());
    ui->lineEditPsu1Part2TestDMMCmd->setText(dmmCmdPsu1Part2Test->getName());
    ui->lineEditPsu1Part2TestDMMStart->setText(dmmCmdPsu1Part2Test->getStart());
    ui->lineEditPsu1Part2TestDMMEnd->setText(dmmCmdPsu1Part2Test->getEnd());
    ui->lineEditPsu1Part2TestDMMJudge->setText(dmmCmdPsu1Part2Test->getRatio());
    ui->lineEditPsu1Part2TestMeterCmd->setText(meterCmdPsu1Part2Test->getName());
    ui->lineEditPsu1Part2TestMeterJudge->setText(meterCmdPsu1Part2Test->getRatio());

    nowIndexPsu1Part3Data = 0;  // ��ǰ����������
    if(psu1Part3 == NULL){
        cmdListPsu1Part3Pre = new QList<command *>;
        dataAndAddrListPsu1Part3 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu1Part3Verify = new command(QString("PSU1_I"));
        setCmdPsu1Part3Verify->setStart(QString("("));
        setCmdPsu1Part3Verify->setEnd(QString(";"));
        setCmdPsu1Part3Verify->setJudge(QString("DONE"));
        setCmdPsu1Part3Verify->setRatio(0);
        setPsu1Part3Multi = 1;
        dmmCmdPsu1Part3Verify = new command(QString("PSU1_CH3_Current_MEASURE_Real"));
        dmmCmdPsu1Part3Verify->setParam(QString("AD"));
        dmmCmdPsu1Part3Verify->setStart(QString("("));
        dmmCmdPsu1Part3Verify->setEnd(QString(";"));
        dmmCmdPsu1Part3Verify->setRatio(10);
        dmmPsu1Part3Multi = 1;
        meterCmdPsu1Part3Verify = new command(QString("read?"));
        meterCmdPsu1Part3Verify->setRatio(10);
        meterPsu1Part3Multi = 1;
        setCmdPsu1Part3Test = new command(QString("PSU1_I"));
        setCmdPsu1Part3Test->setStart(QString("("));
        setCmdPsu1Part3Test->setEnd(QString(";"));
        setCmdPsu1Part3Test->setJudge(QString("DONE"));
        setCmdPsu1Part3Test->setRatio(0);
        dmmCmdPsu1Part3Test = new command(QString("PSU1_CH3_Current_MEASURE_Real"));
        dmmCmdPsu1Part3Test->setStart(QString("("));
        dmmCmdPsu1Part3Test->setEnd(QString(";"));
        dmmCmdPsu1Part3Test->setRatio(0.1);
        meterCmdPsu1Part3Test = new command(QString("read?"));
        meterCmdPsu1Part3Test->setRatio(0.1);
    }else{
        cmdListPsu1Part3Pre = psu1Part3->getCmdList();
        dataAndAddrListPsu1Part3 = psu1Part3->getDataList();
        setCmdPsu1Part3Verify = psu1Part3->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu1Part3Multi = psu1Part3->getSetMulti();   // �Ŵ���
        dmmCmdPsu1Part3Verify = psu1Part3->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu1Part3Multi = psu1Part3->getDmmMulti();  // �Ŵ���
        meterCmdPsu1Part3Verify = psu1Part3->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu1Part3Multi = psu1Part3->getMeterMulti();  // �Ŵ���
        setCmdPsu1Part3Test = psu1Part3->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu1Part3Test = psu1Part3->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu1Part3Test = psu1Part3->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu1Part3PreCmdList();
    for(int i=0; i != dataAndAddrListPsu1Part3->size(); ++i){
        on_pushBtnPsu1Part3DataAdd_clicked();
        checkBoxListPsu1Part3Data.at(i)->setChecked(dataAndAddrListPsu1Part3->at(i)->first);
        dataLineEditListPsu1Part3Data.at(i)->setText(dataAndAddrListPsu1Part3->at(i)->second->first);
        addrLineEditListPsu1Part3Data.at(i)->setText(dataAndAddrListPsu1Part3->at(i)->second->second);
    }
    nowIndexPsu1Part3Pre = -1;  // ���������ǰѡ������
    nowCommandPsu1Part3 = NULL;
    ui->lineEditPsu1Part3VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part3VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part3VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part3VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part3VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu1Part3TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part3TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part3VerifySetCmd->setText(setCmdPsu1Part3Verify->getName());
    ui->lineEditPsu1Part3VerifySetStart->setText(setCmdPsu1Part3Verify->getStart());
    ui->lineEditPsu1Part3VerifySetEnd->setText(setCmdPsu1Part3Verify->getEnd());
    ui->lineEditPsu1Part3VerifySetJudge->setText(setCmdPsu1Part3Verify->getJudge());
    ui->lineEditPsu1Part3VerifySetMulti->setText(QString("%1").arg(setPsu1Part3Multi));
    ui->lineEditPsu1Part3VerifyDMMCmd->setText(dmmCmdPsu1Part3Verify->getName());
    ui->lineEditPsu1Part3VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu1Part3VerifyDMMStart->setText(dmmCmdPsu1Part3Verify->getStart());
    ui->lineEditPsu1Part3VerifyDMMEnd->setText(dmmCmdPsu1Part3Verify->getEnd());
    ui->lineEditPsu1Part3VerifyDMMJudge->setText(dmmCmdPsu1Part3Verify->getRatio());
    ui->lineEditPsu1Part3VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part3Multi));
    ui->lineEditPsu1Part3VerifyMeterCmd->setText(meterCmdPsu1Part3Verify->getName());
    ui->lineEditPsu1Part3VerifyMeterJudge->setText(meterCmdPsu1Part3Verify->getRatio());
    ui->lineEditPsu1Part3VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part3Multi));
    ui->lineEditPsu1Part3TestSetCmd->setText(setCmdPsu1Part3Test->getName());
    ui->lineEditPsu1Part3TestSetStart->setText(setCmdPsu1Part3Test->getStart());
    ui->lineEditPsu1Part3TestSetEnd->setText(setCmdPsu1Part3Test->getEnd());
    ui->lineEditPsu1Part3TestSetJudge->setText(setCmdPsu1Part3Test->getJudge());
    ui->lineEditPsu1Part3TestDMMCmd->setText(dmmCmdPsu1Part3Test->getName());
    ui->lineEditPsu1Part3TestDMMStart->setText(dmmCmdPsu1Part3Test->getStart());
    ui->lineEditPsu1Part3TestDMMEnd->setText(dmmCmdPsu1Part3Test->getEnd());
    ui->lineEditPsu1Part3TestDMMJudge->setText(dmmCmdPsu1Part3Test->getRatio());
    ui->lineEditPsu1Part3TestMeterCmd->setText(meterCmdPsu1Part3Test->getName());
    ui->lineEditPsu1Part3TestMeterJudge->setText(meterCmdPsu1Part3Test->getRatio());

    nowIndexPsu1Part4Data = 0;  // ��ǰ����������
    if(psu1Part4 == NULL){
        cmdListPsu1Part4Pre = new QList<command *>;
        dataAndAddrListPsu1Part4 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu1Part4Verify = new command(QString("PSU1_I"));
        setCmdPsu1Part4Verify->setStart(QString("("));
        setCmdPsu1Part4Verify->setEnd(QString(";"));
        setCmdPsu1Part4Verify->setJudge(QString("DONE"));
        setCmdPsu1Part4Verify->setRatio(0);
        setPsu1Part4Multi = 1;
        dmmCmdPsu1Part4Verify = new command(QString("PSU1_CH4_Current_MEASURE_Real"));
        dmmCmdPsu1Part4Verify->setParam(QString("AD"));
        dmmCmdPsu1Part4Verify->setStart(QString("("));
        dmmCmdPsu1Part4Verify->setEnd(QString(";"));
        dmmCmdPsu1Part4Verify->setRatio(10);
        dmmPsu1Part4Multi = 1;
        meterCmdPsu1Part4Verify = new command(QString("read?"));
        meterCmdPsu1Part4Verify->setRatio(10);
        meterPsu1Part4Multi = 1;
        setCmdPsu1Part4Test = new command(QString("PSU1_I"));
        setCmdPsu1Part4Test->setStart(QString("("));
        setCmdPsu1Part4Test->setEnd(QString(";"));
        setCmdPsu1Part4Test->setJudge(QString("DONE"));
        setCmdPsu1Part4Test->setRatio(0);
        dmmCmdPsu1Part4Test = new command(QString("PSU1_CH4_Current_MEASURE_Real"));
        dmmCmdPsu1Part4Test->setStart(QString("("));
        dmmCmdPsu1Part4Test->setEnd(QString(";"));
        dmmCmdPsu1Part4Test->setRatio(0.1);
        meterCmdPsu1Part4Test = new command(QString("read?"));
        meterCmdPsu1Part4Test->setRatio(0.1);
    }else{
        cmdListPsu1Part4Pre = psu1Part4->getCmdList();
        dataAndAddrListPsu1Part4 = psu1Part4->getDataList();
        setCmdPsu1Part4Verify = psu1Part4->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu1Part4Multi = psu1Part4->getSetMulti();   // �Ŵ���
        dmmCmdPsu1Part4Verify = psu1Part4->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu1Part4Multi = psu1Part4->getDmmMulti();  // �Ŵ���
        meterCmdPsu1Part4Verify = psu1Part4->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu1Part4Multi = psu1Part4->getMeterMulti();  // �Ŵ���
        setCmdPsu1Part4Test = psu1Part4->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu1Part4Test = psu1Part4->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu1Part4Test = psu1Part4->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu1Part4PreCmdList();
    for(int i=0; i != dataAndAddrListPsu1Part4->size(); ++i){
        on_pushBtnPsu1Part4DataAdd_clicked();
        checkBoxListPsu1Part4Data.at(i)->setChecked(dataAndAddrListPsu1Part4->at(i)->first);
        dataLineEditListPsu1Part4Data.at(i)->setText(dataAndAddrListPsu1Part4->at(i)->second->first);
        addrLineEditListPsu1Part4Data.at(i)->setText(dataAndAddrListPsu1Part4->at(i)->second->second);
    }
    nowIndexPsu1Part4Pre = -1;  // ���������ǰѡ������
    nowCommandPsu1Part4 = NULL;
    ui->lineEditPsu1Part4VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part4VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part4VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part4VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part4VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu1Part4TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part4TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part4VerifySetCmd->setText(setCmdPsu1Part4Verify->getName());
    ui->lineEditPsu1Part4VerifySetStart->setText(setCmdPsu1Part4Verify->getStart());
    ui->lineEditPsu1Part4VerifySetEnd->setText(setCmdPsu1Part4Verify->getEnd());
    ui->lineEditPsu1Part4VerifySetJudge->setText(setCmdPsu1Part4Verify->getJudge());
    ui->lineEditPsu1Part4VerifySetMulti->setText(QString("%1").arg(setPsu1Part4Multi));
    ui->lineEditPsu1Part4VerifyDMMCmd->setText(dmmCmdPsu1Part4Verify->getName());
    ui->lineEditPsu1Part4VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu1Part4VerifyDMMStart->setText(dmmCmdPsu1Part4Verify->getStart());
    ui->lineEditPsu1Part4VerifyDMMEnd->setText(dmmCmdPsu1Part4Verify->getEnd());
    ui->lineEditPsu1Part4VerifyDMMJudge->setText(dmmCmdPsu1Part4Verify->getRatio());
    ui->lineEditPsu1Part4VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part4Multi));
    ui->lineEditPsu1Part4VerifyMeterCmd->setText(meterCmdPsu1Part4Verify->getName());
    ui->lineEditPsu1Part4VerifyMeterJudge->setText(meterCmdPsu1Part4Verify->getRatio());
    ui->lineEditPsu1Part4VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part4Multi));
    ui->lineEditPsu1Part4TestSetCmd->setText(setCmdPsu1Part4Test->getName());
    ui->lineEditPsu1Part4TestSetStart->setText(setCmdPsu1Part4Test->getStart());
    ui->lineEditPsu1Part4TestSetEnd->setText(setCmdPsu1Part4Test->getEnd());
    ui->lineEditPsu1Part4TestSetJudge->setText(setCmdPsu1Part4Test->getJudge());
    ui->lineEditPsu1Part4TestDMMCmd->setText(dmmCmdPsu1Part4Test->getName());
    ui->lineEditPsu1Part4TestDMMStart->setText(dmmCmdPsu1Part4Test->getStart());
    ui->lineEditPsu1Part4TestDMMEnd->setText(dmmCmdPsu1Part4Test->getEnd());
    ui->lineEditPsu1Part4TestDMMJudge->setText(dmmCmdPsu1Part4Test->getRatio());
    ui->lineEditPsu1Part4TestMeterCmd->setText(meterCmdPsu1Part4Test->getName());
    ui->lineEditPsu1Part4TestMeterJudge->setText(meterCmdPsu1Part4Test->getRatio());

    nowIndexPsu1Part5Data = 0;  // ��ǰ����������
    if(psu1Part5 == NULL){
        cmdListPsu1Part5Pre = new QList<command *>;
        dataAndAddrListPsu1Part5 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu1Part5Verify = new command(QString("PSU1_I"));
        setCmdPsu1Part5Verify->setStart(QString("("));
        setCmdPsu1Part5Verify->setEnd(QString(";"));
        setCmdPsu1Part5Verify->setJudge(QString("DONE"));
        setCmdPsu1Part5Verify->setRatio(0);
        setPsu1Part5Multi = 1;
        dmmCmdPsu1Part5Verify = new command(QString("PSU1_CH5_Current_MEASURE_Real"));
        dmmCmdPsu1Part5Verify->setParam(QString("AD"));
        dmmCmdPsu1Part5Verify->setStart(QString("("));
        dmmCmdPsu1Part5Verify->setEnd(QString(";"));
        dmmCmdPsu1Part5Verify->setRatio(10);
        dmmPsu1Part5Multi = 1;
        meterCmdPsu1Part5Verify = new command(QString("read?"));
        meterCmdPsu1Part5Verify->setRatio(10);
        meterPsu1Part5Multi = 1;
        setCmdPsu1Part5Test = new command(QString("PSU1_I"));
        setCmdPsu1Part5Test->setStart(QString("("));
        setCmdPsu1Part5Test->setEnd(QString(";"));
        setCmdPsu1Part5Test->setJudge(QString("DONE"));
        setCmdPsu1Part5Test->setRatio(0);
        dmmCmdPsu1Part5Test = new command(QString("PSU1_CH5_Current_MEASURE_Real"));
        dmmCmdPsu1Part5Test->setStart(QString("("));
        dmmCmdPsu1Part5Test->setEnd(QString(";"));
        dmmCmdPsu1Part5Test->setRatio(0.1);
        meterCmdPsu1Part5Test = new command(QString("read?"));
        meterCmdPsu1Part5Test->setRatio(0.1);
    }else{
        cmdListPsu1Part5Pre = psu1Part5->getCmdList();
        dataAndAddrListPsu1Part5 = psu1Part5->getDataList();
        setCmdPsu1Part5Verify = psu1Part5->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu1Part5Multi = psu1Part5->getSetMulti();   // �Ŵ���
        dmmCmdPsu1Part5Verify = psu1Part5->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu1Part5Multi = psu1Part5->getDmmMulti();  // �Ŵ���
        meterCmdPsu1Part5Verify = psu1Part5->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu1Part5Multi = psu1Part5->getMeterMulti();  // �Ŵ���
        setCmdPsu1Part5Test = psu1Part5->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu1Part5Test = psu1Part5->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu1Part5Test = psu1Part5->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu1Part5PreCmdList();
    for(int i=0; i != dataAndAddrListPsu1Part5->size(); ++i){
        on_pushBtnPsu1Part5DataAdd_clicked();
        checkBoxListPsu1Part5Data.at(i)->setChecked(dataAndAddrListPsu1Part5->at(i)->first);
        dataLineEditListPsu1Part5Data.at(i)->setText(dataAndAddrListPsu1Part5->at(i)->second->first);
        addrLineEditListPsu1Part5Data.at(i)->setText(dataAndAddrListPsu1Part5->at(i)->second->second);
    }
    nowIndexPsu1Part5Pre = -1;  // ���������ǰѡ������
    nowCommandPsu1Part5 = NULL;
    ui->lineEditPsu1Part5VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part5VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part5VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu1Part5VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part5VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu1Part5TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part5TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu1Part5VerifySetCmd->setText(setCmdPsu1Part5Verify->getName());
    ui->lineEditPsu1Part5VerifySetStart->setText(setCmdPsu1Part5Verify->getStart());
    ui->lineEditPsu1Part5VerifySetEnd->setText(setCmdPsu1Part5Verify->getEnd());
    ui->lineEditPsu1Part5VerifySetJudge->setText(setCmdPsu1Part5Verify->getJudge());
    ui->lineEditPsu1Part5VerifySetMulti->setText(QString("%1").arg(setPsu1Part5Multi));
    ui->lineEditPsu1Part5VerifyDMMCmd->setText(dmmCmdPsu1Part5Verify->getName());
    ui->lineEditPsu1Part5VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu1Part5VerifyDMMStart->setText(dmmCmdPsu1Part5Verify->getStart());
    ui->lineEditPsu1Part5VerifyDMMEnd->setText(dmmCmdPsu1Part5Verify->getEnd());
    ui->lineEditPsu1Part5VerifyDMMJudge->setText(dmmCmdPsu1Part5Verify->getRatio());
    ui->lineEditPsu1Part5VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part5Multi));
    ui->lineEditPsu1Part5VerifyMeterCmd->setText(meterCmdPsu1Part5Verify->getName());
    ui->lineEditPsu1Part5VerifyMeterJudge->setText(meterCmdPsu1Part5Verify->getRatio());
    ui->lineEditPsu1Part5VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part5Multi));
    ui->lineEditPsu1Part5TestSetCmd->setText(setCmdPsu1Part5Test->getName());
    ui->lineEditPsu1Part5TestSetStart->setText(setCmdPsu1Part5Test->getStart());
    ui->lineEditPsu1Part5TestSetEnd->setText(setCmdPsu1Part5Test->getEnd());
    ui->lineEditPsu1Part5TestSetJudge->setText(setCmdPsu1Part5Test->getJudge());
    ui->lineEditPsu1Part5TestDMMCmd->setText(dmmCmdPsu1Part5Test->getName());
    ui->lineEditPsu1Part5TestDMMStart->setText(dmmCmdPsu1Part5Test->getStart());
    ui->lineEditPsu1Part5TestDMMEnd->setText(dmmCmdPsu1Part5Test->getEnd());
    ui->lineEditPsu1Part5TestDMMJudge->setText(dmmCmdPsu1Part5Test->getRatio());
    ui->lineEditPsu1Part5TestMeterCmd->setText(meterCmdPsu1Part5Test->getName());
    ui->lineEditPsu1Part5TestMeterJudge->setText(meterCmdPsu1Part5Test->getRatio());

    if(itemPsu2 == NULL){
        // PSU2  ǰ������
        cmdListPsu2Pre = new QList<command *>;
        psu2Part1 = NULL;
        psu2Part2 = NULL;
        psu2Part3 = NULL;
        psu2Part4 = NULL;
        psu2Part5 = NULL;
    }else{
        cmdListPsu2Pre = psu2->getPreCmdList();
        showPsu2PreCmdList();
        psu2Part1 = itemPsu2->getPart1();
        psu2Part2 = itemPsu2->getPart2();
        psu2Part3 = itemPsu2->getPart3();
        psu2Part4 = itemPsu2->getPart4();
        psu2Part5 = itemPsu2->getPart5();
    }
    nowIndexPsu2Pre = -1;  // ǰ�������ǰѡ������
    nowCommandPsu2 = NULL;

    nowIndexPsu2Part1Data = 0;  // ��ǰ����������
    if(psu2Part1 == NULL){
        cmdListPsu2Part1Pre = new QList<command *>;
        dataAndAddrListPsu2Part1 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu2Part1Verify = new command(QString("PSU2_I"));
        setCmdPsu2Part1Verify->setStart(QString("("));
        setCmdPsu2Part1Verify->setEnd(QString(";"));
        setCmdPsu2Part1Verify->setJudge(QString("DONE"));
        setCmdPsu2Part1Verify->setRatio(0);
        setPsu2Part1Multi = 1;
        dmmCmdPsu2Part1Verify = new command(QString("PSU2_CH1_Current_MEASURE_Real"));
        dmmCmdPsu2Part1Verify->setParam(QString("AD"));
        dmmCmdPsu2Part1Verify->setStart(QString("("));
        dmmCmdPsu2Part1Verify->setEnd(QString(";"));
        dmmCmdPsu2Part1Verify->setRatio(10);
        dmmPsu2Part1Multi = 1;
        meterCmdPsu2Part1Verify = new command(QString("read?"));
        meterCmdPsu2Part1Verify->setRatio(10);
        meterPsu2Part1Multi = 1;
        setCmdPsu2Part1Test = new command(QString("PSU2_I"));
        setCmdPsu2Part1Test->setStart(QString("("));
        setCmdPsu2Part1Test->setEnd(QString(";"));
        setCmdPsu2Part1Test->setJudge(QString("DONE"));
        setCmdPsu2Part1Test->setRatio(0);
        dmmCmdPsu2Part1Test = new command(QString("PSU2_CH1_Current_MEASURE_Real"));
        dmmCmdPsu2Part1Test->setStart(QString("("));
        dmmCmdPsu2Part1Test->setEnd(QString(";"));
        dmmCmdPsu2Part1Test->setRatio(0.1);
        meterCmdPsu2Part1Test = new command(QString("read?"));
        meterCmdPsu2Part1Test->setRatio(0.1);
    }else{
        cmdListPsu2Part1Pre = psu2Part1->getCmdList();
        dataAndAddrListPsu2Part1 = psu2Part1->getDataList();
        setCmdPsu2Part1Verify = psu2Part1->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu2Part1Multi = psu2Part1->getSetMulti();   // �Ŵ���
        dmmCmdPsu2Part1Verify = psu2Part1->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu2Part1Multi = psu2Part1->getDmmMulti();  // �Ŵ���
        meterCmdPsu2Part1Verify = psu2Part1->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu2Part1Multi = psu2Part1->getMeterMulti();  // �Ŵ���
        setCmdPsu2Part1Test = psu2Part1->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu2Part1Test = psu2Part1->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu2Part1Test = psu2Part1->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu2Part1PreCmdList();
    for(int i=0; i != dataAndAddrListPsu2Part1->size(); ++i){
        on_pushBtnPsu2Part1DataAdd_clicked();
        checkBoxListPsu2Part1Data.at(i)->setChecked(dataAndAddrListPsu2Part1->at(i)->first);
        dataLineEditListPsu2Part1Data.at(i)->setText(dataAndAddrListPsu2Part1->at(i)->second->first);
        addrLineEditListPsu2Part1Data.at(i)->setText(dataAndAddrListPsu2Part1->at(i)->second->second);
    }
    nowIndexPsu2Part1Pre = -1;  // ���������ǰѡ������
    nowCommandPsu2Part1 = NULL;
    ui->lineEditPsu2Part1VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part1VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part1VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part1VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part1VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu2Part1TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part1TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part1VerifySetCmd->setText(setCmdPsu2Part1Verify->getName());
    ui->lineEditPsu2Part1VerifySetStart->setText(setCmdPsu2Part1Verify->getStart());
    ui->lineEditPsu2Part1VerifySetEnd->setText(setCmdPsu2Part1Verify->getEnd());
    ui->lineEditPsu2Part1VerifySetJudge->setText(setCmdPsu2Part1Verify->getJudge());
    ui->lineEditPsu2Part1VerifySetMulti->setText(QString("%1").arg(setPsu2Part1Multi));
    ui->lineEditPsu2Part1VerifyDMMCmd->setText(dmmCmdPsu2Part1Verify->getName());
    ui->lineEditPsu2Part1VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu2Part1VerifyDMMStart->setText(dmmCmdPsu2Part1Verify->getStart());
    ui->lineEditPsu2Part1VerifyDMMEnd->setText(dmmCmdPsu2Part1Verify->getEnd());
    ui->lineEditPsu2Part1VerifyDMMJudge->setText(dmmCmdPsu2Part1Verify->getRatio());
    ui->lineEditPsu2Part1VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part1Multi));
    ui->lineEditPsu2Part1VerifyMeterCmd->setText(meterCmdPsu2Part1Verify->getName());
    ui->lineEditPsu2Part1VerifyMeterJudge->setText(meterCmdPsu2Part1Verify->getRatio());
    ui->lineEditPsu2Part1VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part1Multi));
    ui->lineEditPsu2Part1TestSetCmd->setText(setCmdPsu2Part1Test->getName());
    ui->lineEditPsu2Part1TestSetStart->setText(setCmdPsu2Part1Test->getStart());
    ui->lineEditPsu2Part1TestSetEnd->setText(setCmdPsu2Part1Test->getEnd());
    ui->lineEditPsu2Part1TestSetJudge->setText(setCmdPsu2Part1Test->getJudge());
    ui->lineEditPsu2Part1TestDMMCmd->setText(dmmCmdPsu2Part1Test->getName());
    ui->lineEditPsu2Part1TestDMMStart->setText(dmmCmdPsu2Part1Test->getStart());
    ui->lineEditPsu2Part1TestDMMEnd->setText(dmmCmdPsu2Part1Test->getEnd());
    ui->lineEditPsu2Part1TestDMMJudge->setText(dmmCmdPsu2Part1Test->getRatio());
    ui->lineEditPsu2Part1TestMeterCmd->setText(meterCmdPsu2Part1Test->getName());
    ui->lineEditPsu2Part1TestMeterJudge->setText(meterCmdPsu2Part1Test->getRatio());

    nowIndexPsu2Part2Data = 0;  // ��ǰ����������
    if(psu2Part2 == NULL){
        cmdListPsu2Part2Pre = new QList<command *>;
        dataAndAddrListPsu2Part2 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu2Part2Verify = new command(QString("PSU2_I"));
        setCmdPsu2Part2Verify->setStart(QString("("));
        setCmdPsu2Part2Verify->setEnd(QString(";"));
        setCmdPsu2Part2Verify->setJudge(QString("DONE"));
        setCmdPsu2Part2Verify->setRatio(0);
        setPsu2Part2Multi = 1;
        dmmCmdPsu2Part2Verify = new command(QString("PSU2_CH2_Current_MEASURE_Real"));
        dmmCmdPsu2Part2Verify->setParam(QString("AD"));
        dmmCmdPsu2Part2Verify->setStart(QString("("));
        dmmCmdPsu2Part2Verify->setEnd(QString(";"));
        dmmCmdPsu2Part2Verify->setRatio(10);
        dmmPsu2Part2Multi = 1;
        meterCmdPsu2Part2Verify = new command(QString("read?"));
        meterCmdPsu2Part2Verify->setRatio(10);
        meterPsu2Part2Multi = 1;
        setCmdPsu2Part2Test = new command(QString("PSU2_I"));
        setCmdPsu2Part2Test->setStart(QString("("));
        setCmdPsu2Part2Test->setEnd(QString(";"));
        setCmdPsu2Part2Test->setJudge(QString("DONE"));
        setCmdPsu2Part2Test->setRatio(0);
        dmmCmdPsu2Part2Test = new command(QString("PSU2_CH2_Current_MEASURE_Real"));
        dmmCmdPsu2Part2Test->setStart(QString("("));
        dmmCmdPsu2Part2Test->setEnd(QString(";"));
        dmmCmdPsu2Part2Test->setRatio(0.1);
        meterCmdPsu2Part2Test = new command(QString("read?"));
        meterCmdPsu2Part2Test->setRatio(0.1);
    }else{
        cmdListPsu2Part2Pre = psu2Part2->getCmdList();
        dataAndAddrListPsu2Part2 = psu2Part2->getDataList();
        setCmdPsu2Part2Verify = psu2Part2->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu2Part2Multi = psu2Part2->getSetMulti();   // �Ŵ���
        dmmCmdPsu2Part2Verify = psu2Part2->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu2Part2Multi = psu2Part2->getDmmMulti();  // �Ŵ���
        meterCmdPsu2Part2Verify = psu2Part2->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu2Part2Multi = psu2Part2->getMeterMulti();  // �Ŵ���
        setCmdPsu2Part2Test = psu2Part2->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu2Part2Test = psu2Part2->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu2Part2Test = psu2Part2->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu2Part2PreCmdList();
    for(int i=0; i != dataAndAddrListPsu2Part2->size(); ++i){
        on_pushBtnPsu2Part2DataAdd_clicked();
        checkBoxListPsu2Part2Data.at(i)->setChecked(dataAndAddrListPsu2Part2->at(i)->first);
        dataLineEditListPsu2Part2Data.at(i)->setText(dataAndAddrListPsu2Part2->at(i)->second->first);
        addrLineEditListPsu2Part2Data.at(i)->setText(dataAndAddrListPsu2Part2->at(i)->second->second);
    }
    nowIndexPsu2Part2Pre = -1;  // ���������ǰѡ������
    nowCommandPsu2Part2 = NULL;
    ui->lineEditPsu2Part2VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part2VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part2VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part2VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part2VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu2Part2TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part2TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part2VerifySetCmd->setText(setCmdPsu2Part2Verify->getName());
    ui->lineEditPsu2Part2VerifySetStart->setText(setCmdPsu2Part2Verify->getStart());
    ui->lineEditPsu2Part2VerifySetEnd->setText(setCmdPsu2Part2Verify->getEnd());
    ui->lineEditPsu2Part2VerifySetJudge->setText(setCmdPsu2Part2Verify->getJudge());
    ui->lineEditPsu2Part2VerifySetMulti->setText(QString("%1").arg(setPsu2Part2Multi));
    ui->lineEditPsu2Part2VerifyDMMCmd->setText(dmmCmdPsu2Part2Verify->getName());
    ui->lineEditPsu2Part2VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu2Part2VerifyDMMStart->setText(dmmCmdPsu2Part2Verify->getStart());
    ui->lineEditPsu2Part2VerifyDMMEnd->setText(dmmCmdPsu2Part2Verify->getEnd());
    ui->lineEditPsu2Part2VerifyDMMJudge->setText(dmmCmdPsu2Part2Verify->getRatio());
    ui->lineEditPsu2Part2VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part2Multi));
    ui->lineEditPsu2Part2VerifyMeterCmd->setText(meterCmdPsu2Part2Verify->getName());
    ui->lineEditPsu2Part2VerifyMeterJudge->setText(meterCmdPsu2Part2Verify->getRatio());
    ui->lineEditPsu2Part2VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part2Multi));
    ui->lineEditPsu2Part2TestSetCmd->setText(setCmdPsu2Part2Test->getName());
    ui->lineEditPsu2Part2TestSetStart->setText(setCmdPsu2Part2Test->getStart());
    ui->lineEditPsu2Part2TestSetEnd->setText(setCmdPsu2Part2Test->getEnd());
    ui->lineEditPsu2Part2TestSetJudge->setText(setCmdPsu2Part2Test->getJudge());
    ui->lineEditPsu2Part2TestDMMCmd->setText(dmmCmdPsu2Part2Test->getName());
    ui->lineEditPsu2Part2TestDMMStart->setText(dmmCmdPsu2Part2Test->getStart());
    ui->lineEditPsu2Part2TestDMMEnd->setText(dmmCmdPsu2Part2Test->getEnd());
    ui->lineEditPsu2Part2TestDMMJudge->setText(dmmCmdPsu2Part2Test->getRatio());
    ui->lineEditPsu2Part2TestMeterCmd->setText(meterCmdPsu2Part2Test->getName());
    ui->lineEditPsu2Part2TestMeterJudge->setText(meterCmdPsu2Part2Test->getRatio());

    nowIndexPsu2Part3Data = 0;  // ��ǰ����������
    if(psu2Part3 == NULL){
        cmdListPsu2Part3Pre = new QList<command *>;
        dataAndAddrListPsu2Part3 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu2Part3Verify = new command(QString("PSU2_I"));
        setCmdPsu2Part3Verify->setStart(QString("("));
        setCmdPsu2Part3Verify->setEnd(QString(";"));
        setCmdPsu2Part3Verify->setJudge(QString("DONE"));
        setCmdPsu2Part3Verify->setRatio(0);
        setPsu2Part3Multi = 1;
        dmmCmdPsu2Part3Verify = new command(QString("PSU2_CH3_Current_MEASURE_Real"));
        dmmCmdPsu2Part3Verify->setParam(QString("AD"));
        dmmCmdPsu2Part3Verify->setStart(QString("("));
        dmmCmdPsu2Part3Verify->setEnd(QString(";"));
        dmmCmdPsu2Part3Verify->setRatio(10);
        dmmPsu2Part3Multi = 1;
        meterCmdPsu2Part3Verify = new command(QString("read?"));
        meterCmdPsu2Part3Verify->setRatio(10);
        meterPsu2Part3Multi = 1;
        setCmdPsu2Part3Test = new command(QString("PSU2_I"));
        setCmdPsu2Part3Test->setStart(QString("("));
        setCmdPsu2Part3Test->setEnd(QString(";"));
        setCmdPsu2Part3Test->setJudge(QString("DONE"));
        setCmdPsu2Part3Test->setRatio(0);
        dmmCmdPsu2Part3Test = new command(QString("PSU2_CH3_Current_MEASURE_Real"));
        dmmCmdPsu2Part3Test->setStart(QString("("));
        dmmCmdPsu2Part3Test->setEnd(QString(";"));
        dmmCmdPsu2Part3Test->setRatio(0.1);
        meterCmdPsu2Part3Test = new command(QString("read?"));
        meterCmdPsu2Part3Test->setRatio(0.1);
    }else{
        cmdListPsu2Part3Pre = psu2Part3->getCmdList();
        dataAndAddrListPsu2Part3 = psu2Part3->getDataList();
        setCmdPsu2Part3Verify = psu2Part3->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu2Part3Multi = psu2Part3->getSetMulti();   // �Ŵ���
        dmmCmdPsu2Part3Verify = psu2Part3->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu2Part3Multi = psu2Part3->getDmmMulti();  // �Ŵ���
        meterCmdPsu2Part3Verify = psu2Part3->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu2Part3Multi = psu2Part3->getMeterMulti();  // �Ŵ���
        setCmdPsu2Part3Test = psu2Part3->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu2Part3Test = psu2Part3->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu2Part3Test = psu2Part3->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu2Part3PreCmdList();
    for(int i=0; i != dataAndAddrListPsu2Part3->size(); ++i){
        on_pushBtnPsu2Part3DataAdd_clicked();
        checkBoxListPsu2Part3Data.at(i)->setChecked(dataAndAddrListPsu2Part3->at(i)->first);
        dataLineEditListPsu2Part3Data.at(i)->setText(dataAndAddrListPsu2Part3->at(i)->second->first);
        addrLineEditListPsu2Part3Data.at(i)->setText(dataAndAddrListPsu2Part3->at(i)->second->second);
    }
    nowIndexPsu2Part3Pre = -1;  // ���������ǰѡ������
    nowCommandPsu2Part3 = NULL;
    ui->lineEditPsu2Part3VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part3VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part3VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part3VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part3VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu2Part3TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part3TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part3VerifySetCmd->setText(setCmdPsu2Part3Verify->getName());
    ui->lineEditPsu2Part3VerifySetStart->setText(setCmdPsu2Part3Verify->getStart());
    ui->lineEditPsu2Part3VerifySetEnd->setText(setCmdPsu2Part3Verify->getEnd());
    ui->lineEditPsu2Part3VerifySetJudge->setText(setCmdPsu2Part3Verify->getJudge());
    ui->lineEditPsu2Part3VerifySetMulti->setText(QString("%1").arg(setPsu2Part3Multi));
    ui->lineEditPsu2Part3VerifyDMMCmd->setText(dmmCmdPsu2Part3Verify->getName());
    ui->lineEditPsu2Part3VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu2Part3VerifyDMMStart->setText(dmmCmdPsu2Part3Verify->getStart());
    ui->lineEditPsu2Part3VerifyDMMEnd->setText(dmmCmdPsu2Part3Verify->getEnd());
    ui->lineEditPsu2Part3VerifyDMMJudge->setText(dmmCmdPsu2Part3Verify->getRatio());
    ui->lineEditPsu2Part3VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part3Multi));
    ui->lineEditPsu2Part3VerifyMeterCmd->setText(meterCmdPsu2Part3Verify->getName());
    ui->lineEditPsu2Part3VerifyMeterJudge->setText(meterCmdPsu2Part3Verify->getRatio());
    ui->lineEditPsu2Part3VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part3Multi));
    ui->lineEditPsu2Part3TestSetCmd->setText(setCmdPsu2Part3Test->getName());
    ui->lineEditPsu2Part3TestSetStart->setText(setCmdPsu2Part3Test->getStart());
    ui->lineEditPsu2Part3TestSetEnd->setText(setCmdPsu2Part3Test->getEnd());
    ui->lineEditPsu2Part3TestSetJudge->setText(setCmdPsu2Part3Test->getJudge());
    ui->lineEditPsu2Part3TestDMMCmd->setText(dmmCmdPsu2Part3Test->getName());
    ui->lineEditPsu2Part3TestDMMStart->setText(dmmCmdPsu2Part3Test->getStart());
    ui->lineEditPsu2Part3TestDMMEnd->setText(dmmCmdPsu2Part3Test->getEnd());
    ui->lineEditPsu2Part3TestDMMJudge->setText(dmmCmdPsu2Part3Test->getRatio());
    ui->lineEditPsu2Part3TestMeterCmd->setText(meterCmdPsu2Part3Test->getName());
    ui->lineEditPsu2Part3TestMeterJudge->setText(meterCmdPsu2Part3Test->getRatio());

    nowIndexPsu2Part4Data = 0;  // ��ǰ����������
    if(psu2Part4 == NULL){
        cmdListPsu2Part4Pre = new QList<command *>;
        dataAndAddrListPsu2Part4 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu2Part4Verify = new command(QString("PSU2_I"));
        setCmdPsu2Part4Verify->setStart(QString("("));
        setCmdPsu2Part4Verify->setEnd(QString(";"));
        setCmdPsu2Part4Verify->setJudge(QString("DONE"));
        setCmdPsu2Part4Verify->setRatio(0);
        setPsu2Part4Multi = 1;
        dmmCmdPsu2Part4Verify = new command(QString("PSU2_CH4_Current_MEASURE_Real"));
        dmmCmdPsu2Part4Verify->setParam(QString("AD"));
        dmmCmdPsu2Part4Verify->setStart(QString("("));
        dmmCmdPsu2Part4Verify->setEnd(QString(";"));
        dmmCmdPsu2Part4Verify->setRatio(10);
        dmmPsu2Part4Multi = 1;
        meterCmdPsu2Part4Verify = new command(QString("read?"));
        meterCmdPsu2Part4Verify->setRatio(10);
        meterPsu2Part4Multi = 1;
        setCmdPsu2Part4Test = new command(QString("PSU2_I"));
        setCmdPsu2Part4Test->setStart(QString("("));
        setCmdPsu2Part4Test->setEnd(QString(";"));
        setCmdPsu2Part4Test->setJudge(QString("DONE"));
        setCmdPsu2Part4Test->setRatio(0);
        dmmCmdPsu2Part4Test = new command(QString("PSU2_CH4_Current_MEASURE_Real"));
        dmmCmdPsu2Part4Test->setStart(QString("("));
        dmmCmdPsu2Part4Test->setEnd(QString(";"));
        dmmCmdPsu2Part4Test->setRatio(0.1);
        meterCmdPsu2Part4Test = new command(QString("read?"));
        meterCmdPsu2Part4Test->setRatio(0.1);
    }else{
        cmdListPsu2Part4Pre = psu2Part4->getCmdList();
        dataAndAddrListPsu2Part4 = psu2Part4->getDataList();
        setCmdPsu2Part4Verify = psu2Part4->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu2Part4Multi = psu2Part4->getSetMulti();   // �Ŵ���
        dmmCmdPsu2Part4Verify = psu2Part4->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu2Part4Multi = psu2Part4->getDmmMulti();  // �Ŵ���
        meterCmdPsu2Part4Verify = psu2Part4->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu2Part4Multi = psu2Part4->getMeterMulti();  // �Ŵ���
        setCmdPsu2Part4Test = psu2Part4->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu2Part4Test = psu2Part4->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu2Part4Test = psu2Part4->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu2Part4PreCmdList();
    for(int i=0; i != dataAndAddrListPsu2Part4->size(); ++i){
        on_pushBtnPsu2Part4DataAdd_clicked();
        checkBoxListPsu2Part4Data.at(i)->setChecked(dataAndAddrListPsu2Part4->at(i)->first);
        dataLineEditListPsu2Part4Data.at(i)->setText(dataAndAddrListPsu2Part4->at(i)->second->first);
        addrLineEditListPsu2Part4Data.at(i)->setText(dataAndAddrListPsu2Part4->at(i)->second->second);
    }
    nowIndexPsu2Part4Pre = -1;  // ���������ǰѡ������
    nowCommandPsu2Part4 = NULL;
    ui->lineEditPsu2Part4VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part4VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part4VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part4VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part4VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu2Part4TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part4TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part4VerifySetCmd->setText(setCmdPsu2Part4Verify->getName());
    ui->lineEditPsu2Part4VerifySetStart->setText(setCmdPsu2Part4Verify->getStart());
    ui->lineEditPsu2Part4VerifySetEnd->setText(setCmdPsu2Part4Verify->getEnd());
    ui->lineEditPsu2Part4VerifySetJudge->setText(setCmdPsu2Part4Verify->getJudge());
    ui->lineEditPsu2Part4VerifySetMulti->setText(QString("%1").arg(setPsu2Part4Multi));
    ui->lineEditPsu2Part4VerifyDMMCmd->setText(dmmCmdPsu2Part4Verify->getName());
    ui->lineEditPsu2Part4VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu2Part4VerifyDMMStart->setText(dmmCmdPsu2Part4Verify->getStart());
    ui->lineEditPsu2Part4VerifyDMMEnd->setText(dmmCmdPsu2Part4Verify->getEnd());
    ui->lineEditPsu2Part4VerifyDMMJudge->setText(dmmCmdPsu2Part4Verify->getRatio());
    ui->lineEditPsu2Part4VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part4Multi));
    ui->lineEditPsu2Part4VerifyMeterCmd->setText(meterCmdPsu2Part4Verify->getName());
    ui->lineEditPsu2Part4VerifyMeterJudge->setText(meterCmdPsu2Part4Verify->getRatio());
    ui->lineEditPsu2Part4VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part4Multi));
    ui->lineEditPsu2Part4TestSetCmd->setText(setCmdPsu2Part4Test->getName());
    ui->lineEditPsu2Part4TestSetStart->setText(setCmdPsu2Part4Test->getStart());
    ui->lineEditPsu2Part4TestSetEnd->setText(setCmdPsu2Part4Test->getEnd());
    ui->lineEditPsu2Part4TestSetJudge->setText(setCmdPsu2Part4Test->getJudge());
    ui->lineEditPsu2Part4TestDMMCmd->setText(dmmCmdPsu2Part4Test->getName());
    ui->lineEditPsu2Part4TestDMMStart->setText(dmmCmdPsu2Part4Test->getStart());
    ui->lineEditPsu2Part4TestDMMEnd->setText(dmmCmdPsu2Part4Test->getEnd());
    ui->lineEditPsu2Part4TestDMMJudge->setText(dmmCmdPsu2Part4Test->getRatio());
    ui->lineEditPsu2Part4TestMeterCmd->setText(meterCmdPsu2Part4Test->getName());
    ui->lineEditPsu2Part4TestMeterJudge->setText(meterCmdPsu2Part4Test->getRatio());

    nowIndexPsu2Part5Data = 0;  // ��ǰ����������
    if(psu2Part5 == NULL){
        cmdListPsu2Part5Pre = new QList<command *>;
        dataAndAddrListPsu2Part5 = new QList<QPair<bool, QPair<QString, QString> *> *>;
        setCmdPsu2Part5Verify = new command(QString("PSU2_I"));
        setCmdPsu2Part5Verify->setStart(QString("("));
        setCmdPsu2Part5Verify->setEnd(QString(";"));
        setCmdPsu2Part5Verify->setJudge(QString("DONE"));
        setCmdPsu2Part5Verify->setRatio(0);
        setPsu2Part5Multi = 1;
        dmmCmdPsu2Part5Verify = new command(QString("PSU2_CH5_Current_MEASURE_Real"));
        dmmCmdPsu2Part5Verify->setParam(QString("AD"));
        dmmCmdPsu2Part5Verify->setStart(QString("("));
        dmmCmdPsu2Part5Verify->setEnd(QString(";"));
        dmmCmdPsu2Part5Verify->setRatio(10);
        dmmPsu2Part5Multi = 1;
        meterCmdPsu2Part5Verify = new command(QString("read?"));
        meterCmdPsu2Part5Verify->setRatio(10);
        meterPsu2Part5Multi = 1;
        setCmdPsu2Part5Test = new command(QString("PSU2_I"));
        setCmdPsu2Part5Test->setStart(QString("("));
        setCmdPsu2Part5Test->setEnd(QString(";"));
        setCmdPsu2Part5Test->setJudge(QString("DONE"));
        setCmdPsu2Part5Test->setRatio(0);
        dmmCmdPsu2Part5Test = new command(QString("PSU2_CH5_Current_MEASURE_Real"));
        dmmCmdPsu2Part5Test->setStart(QString("("));
        dmmCmdPsu2Part5Test->setEnd(QString(";"));
        dmmCmdPsu2Part5Test->setRatio(0.1);
        meterCmdPsu2Part5Test = new command(QString("read?"));
        meterCmdPsu2Part5Test->setRatio(0.1);
    }else{
        cmdListPsu2Part5Pre = psu2Part5->getCmdList();
        dataAndAddrListPsu2Part5 = psu2Part5->getDataList();
        setCmdPsu2Part5Verify = psu2Part5->getSetCmdVerify();  // ��ʼ������ʾУ׼ҳ���õ�ѹ����
        setPsu2Part5Multi = psu2Part5->getSetMulti();   // �Ŵ���
        dmmCmdPsu2Part5Verify = psu2Part5->getDmmCmdVerify();  // ��ʼ������ʾУ׼ҳDMM��ȡ��ѹ����
        dmmPsu2Part5Multi = psu2Part5->getDmmMulti();  // �Ŵ���
        meterCmdPsu2Part5Verify = psu2Part5->getMeterCmdVerify();  // ��ʼ������ʾУ׼ҳ���ñ����ѹ����
        meterPsu2Part5Multi = psu2Part5->getMeterMulti();  // �Ŵ���
        setCmdPsu2Part5Test = psu2Part5->getSetCmdTest();  // ��ʼ������ʾ����ҳ���õ�ѹ����
        dmmCmdPsu2Part5Test = psu2Part5->getDmmCmdTest();  // ��ʼ������ʾ����ҳDMM��ȡ��ѹ����
        meterCmdPsu2Part5Test = psu2Part5->getMeterCmdTest();  // ��ʼ������ʾ����ҳ���ñ����ѹ����
    }
    showPsu2Part5PreCmdList();
    for(int i=0; i != dataAndAddrListPsu2Part5->size(); ++i){
        on_pushBtnPsu2Part5DataAdd_clicked();
        checkBoxListPsu2Part5Data.at(i)->setChecked(dataAndAddrListPsu2Part5->at(i)->first);
        dataLineEditListPsu2Part5Data.at(i)->setText(dataAndAddrListPsu2Part5->at(i)->second->first);
        addrLineEditListPsu2Part5Data.at(i)->setText(dataAndAddrListPsu2Part5->at(i)->second->second);
    }
    nowIndexPsu2Part5Pre = -1;  // ���������ǰѡ������
    nowCommandPsu2Part5 = NULL;
    ui->lineEditPsu2Part5VerifySetMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part5VerifyDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part5VerifyDMMMulti->setValidator(new QIntValidator(1, 1000000, this));
    ui->lineEditPsu2Part5VerifyMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part5VerifyMeterMulti->setValidator(new QIntValidator(1, 1000000000, this));
    ui->lineEditPsu2Part5TestDMMJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part5TestMeterJudge->setValidator(new QDoubleValidator(0.0001, 100, 4, this));
    ui->lineEditPsu2Part5VerifySetCmd->setText(setCmdPsu2Part5Verify->getName());
    ui->lineEditPsu2Part5VerifySetStart->setText(setCmdPsu2Part5Verify->getStart());
    ui->lineEditPsu2Part5VerifySetEnd->setText(setCmdPsu2Part5Verify->getEnd());
    ui->lineEditPsu2Part5VerifySetJudge->setText(setCmdPsu2Part5Verify->getJudge());
    ui->lineEditPsu2Part5VerifySetMulti->setText(QString("%1").arg(setPsu2Part5Multi));
    ui->lineEditPsu2Part5VerifyDMMCmd->setText(dmmCmdPsu2Part5Verify->getName());
    ui->lineEditPsu2Part5VerifyDMMParam->setText(dmmCmdPsu1Part1Verify->getParam());
    ui->lineEditPsu2Part5VerifyDMMStart->setText(dmmCmdPsu2Part5Verify->getStart());
    ui->lineEditPsu2Part5VerifyDMMEnd->setText(dmmCmdPsu2Part5Verify->getEnd());
    ui->lineEditPsu2Part5VerifyDMMJudge->setText(dmmCmdPsu2Part5Verify->getRatio());
    ui->lineEditPsu2Part5VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part5Multi));
    ui->lineEditPsu2Part5VerifyMeterCmd->setText(meterCmdPsu2Part5Verify->getName());
    ui->lineEditPsu2Part5VerifyMeterJudge->setText(meterCmdPsu2Part5Verify->getRatio());
    ui->lineEditPsu2Part5VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part5Multi));
    ui->lineEditPsu2Part5TestSetCmd->setText(setCmdPsu2Part5Test->getName());
    ui->lineEditPsu2Part5TestSetStart->setText(setCmdPsu2Part5Test->getStart());
    ui->lineEditPsu2Part5TestSetEnd->setText(setCmdPsu2Part5Test->getEnd());
    ui->lineEditPsu2Part5TestSetJudge->setText(setCmdPsu2Part5Test->getJudge());
    ui->lineEditPsu2Part5TestDMMCmd->setText(dmmCmdPsu2Part5Test->getName());
    ui->lineEditPsu2Part5TestDMMStart->setText(dmmCmdPsu2Part5Test->getStart());
    ui->lineEditPsu2Part5TestDMMEnd->setText(dmmCmdPsu2Part5Test->getEnd());
    ui->lineEditPsu2Part5TestDMMJudge->setText(dmmCmdPsu2Part5Test->getRatio());
    ui->lineEditPsu2Part5TestMeterCmd->setText(meterCmdPsu2Part5Test->getName());
    ui->lineEditPsu2Part5TestMeterJudge->setText(meterCmdPsu2Part5Test->getRatio());
}
// ����
curdataconfig::~curdataconfig()
{
    delete ui;
}
// �˳��¼�
void curdataconfig::closeEvent(QCloseEvent * event)
{
    emit returnTestItem(itemPsu1, itemPsu2);
}
// �˳�
void curdataconfig::on_pushBtnExit_clicked()
{
    this->close();
}
// PSU1  ǰ������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu1Pre->append(newCommand);
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPreCmdList(cmdListPsu1Pre);
    showPsu1PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu1PreCmdList()
{
    ui->psu1PreCmdList->clear();
    for(int i=0; i != cmdListPsu1Pre->size(); ++i){
        command *curCommand = cmdListPsu1Pre->at(i);
        ui->psu1PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu1PreCmdList_clicked(const QModelIndex &index)
{
    nowIndexPsu1Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu1 = cmdListPsu1Pre->at(nowIndexPsu1Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu1PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1PreDel_clicked()
{
    if(nowIndexPsu1Pre == -1){
        return ;
    }
    cmdListPsu1Pre->removeAt(nowIndexPsu1Pre);
    nowIndexPsu1Pre = cmdListPsu1Pre->size()-1;
    if(nowIndexPsu1Pre != -1)
        nowCommandPsu1 = cmdListPsu1Pre->at(nowIndexPsu1Pre);
    showPsu1PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu1PreCmdName->clear();
    ui->lineEditPsu1PreParam->clear();
    ui->lineEditPsu1PreStart->clear();
    ui->lineEditPsu1PreEnd->clear();
    ui->lineEditPsu1PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu1PreUp_clicked()
{
    if(nowIndexPsu1Pre < 1)
        return;
    cmdListPsu1Pre->swap(nowIndexPsu1Pre, nowIndexPsu1Pre-1);
    nowIndexPsu1Pre--;
    showPsu1PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu1PreDown_clicked()
{
    if((nowIndexPsu1Pre == -1)||(nowIndexPsu1Pre >= cmdListPsu1Pre->size()-1))
        return;
    cmdListPsu1Pre->swap(nowIndexPsu1Pre, nowIndexPsu1Pre+1);
    nowIndexPsu1Pre++;
    showPsu1PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu1PreSave_clicked()
{
    if(ui->lineEditPsu1PreCmdName->text().size() == 0 &&
            ui->lineEditPsu1PreParam->text().size() == 0 &&
            ui->lineEditPsu1PreStart->text().size() == 0 &&
            ui->lineEditPsu1PreEnd->text().size() == 0 &&
            ui->lineEditPsu1PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu1 != NULL){
        nowCommandPsu1->setName(ui->lineEditPsu1PreCmdName->text());
        nowCommandPsu1->setParam(ui->lineEditPsu1PreParam->text());
        nowCommandPsu1->setStart(ui->lineEditPsu1PreStart->text());
        nowCommandPsu1->setEnd(ui->lineEditPsu1PreEnd->text());
        nowCommandPsu1->setJudge(ui->lineEditPsu1PreJudge->text());
        showPsu1PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu1PreCmdName->clear();
    ui->lineEditPsu1PreParam->clear();
    ui->lineEditPsu1PreStart->clear();
    ui->lineEditPsu1PreEnd->clear();
    ui->lineEditPsu1PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu1PreCmdName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu1PreCmdName->setText(nowCommandPsu1->getName());
    // ��ʾ�������
    ui->lineEditPsu1PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu1PreParam->setText(nowCommandPsu1->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu1PreStart->setText(nowCommandPsu1->getStart());
    ui->lineEditPsu1PreEnd->setText(nowCommandPsu1->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu1PreJudge->setText(nowCommandPsu1->getJudge());
}
// PSU1  Part1  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part1PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu1Part1Pre->append(newCommand);
    if(psu1Part1 == NULL){
        psu1Part1 = new testItem(cmdListPsu1Part1Pre, dataAndAddrListPsu1Part1,
                                 setCmdPsu1Part1Verify, setPsu1Part1Multi,
                                 dmmCmdPsu1Part1Verify, dmmPsu1Part1Multi,
                                 meterCmdPsu1Part1Verify, meterPsu1Part1Multi,
                                 setCmdPsu1Part1Test, dmmCmdPsu1Part1Test, meterCmdPsu1Part1Test);
    }else
        psu1Part1->setCmdList(cmdListPsu1Part1Pre);
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart1(psu1Part1);
    showPsu1Part1PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu1Part1PreCmdList()
{
    ui->psu1Part1PreCmdList->clear();
    for(int i=0; i != cmdListPsu1Part1Pre->size(); ++i){
        command *curCommand = cmdListPsu1Part1Pre->at(i);
        ui->psu1Part1PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu1Part1PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu1Part1Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu1Part1 = cmdListPsu1Part1Pre->at(nowIndexPsu1Part1Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu1Part1PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part1PreDel_clicked()
{
    if(nowIndexPsu1Part1Pre == -1){
        return ;
    }
    cmdListPsu1Part1Pre->removeAt(nowIndexPsu1Part1Pre);
    nowIndexPsu1Part1Pre = cmdListPsu1Part1Pre->size()-1;
    if(nowIndexPsu1Part1Pre != -1)
        nowCommandPsu1Part1 = cmdListPsu1Part1Pre->at(nowIndexPsu1Part1Pre);
    showPsu1Part1PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu1Part1PreName->clear();
    ui->lineEditPsu1Part1PreParam->clear();
    ui->lineEditPsu1Part1PreStart->clear();
    ui->lineEditPsu1Part1PreEnd->clear();
    ui->lineEditPsu1Part1PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu1Part1PreUp_clicked()
{
    if(nowIndexPsu1Part1Pre < 1)
        return;
    cmdListPsu1Part1Pre->swap(nowIndexPsu1Part1Pre, nowIndexPsu1Part1Pre-1);
    nowIndexPsu1Part1Pre--;
    showPsu1Part1PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu1Part1PreDown_clicked()
{
    if((nowIndexPsu1Part1Pre == -1)||(nowIndexPsu1Part1Pre >= cmdListPsu1Part1Pre->size()-1))
        return;
    cmdListPsu1Part1Pre->swap(nowIndexPsu1Part1Pre, nowIndexPsu1Part1Pre+1);
    nowIndexPsu1Part1Pre++;
    showPsu1Part1PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part1PreSave_clicked()
{
    if(ui->lineEditPsu1Part1PreName->text().size() == 0 &&
            ui->lineEditPsu1Part1PreParam->text().size() == 0 &&
            ui->lineEditPsu1Part1PreStart->text().size() == 0 &&
            ui->lineEditPsu1Part1PreEnd->text().size() == 0 &&
            ui->lineEditPsu1Part1PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu1Part1 != NULL){
        nowCommandPsu1Part1->setName(ui->lineEditPsu1Part1PreName->text());
        nowCommandPsu1Part1->setParam(ui->lineEditPsu1Part1PreParam->text());
        nowCommandPsu1Part1->setStart(ui->lineEditPsu1Part1PreStart->text());
        nowCommandPsu1Part1->setEnd(ui->lineEditPsu1Part1PreEnd->text());
        nowCommandPsu1Part1->setJudge(ui->lineEditPsu1Part1PreJudge->text());
        showPsu1Part1PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part1PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu1Part1PreName->clear();
    ui->lineEditPsu1Part1PreParam->clear();
    ui->lineEditPsu1Part1PreStart->clear();
    ui->lineEditPsu1Part1PreEnd->clear();
    ui->lineEditPsu1Part1PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu1Part1PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu1Part1PreName->setText(nowCommandPsu1Part1->getName());
    // ��ʾ�������
    ui->lineEditPsu1Part1PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu1Part1PreParam->setText(nowCommandPsu1Part1->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu1Part1PreStart->setText(nowCommandPsu1Part1->getStart());
    ui->lineEditPsu1Part1PreEnd->setText(nowCommandPsu1Part1->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu1Part1PreJudge->setText(nowCommandPsu1Part1->getJudge());
}
// PSU1  Part1  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part1DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu1Part1Data / 10;
    y = nowIndexPsu1Part1Data % 10;
    ui->scrollAreaWidgetContentsPsu1Part1->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part1Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu1Part1);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu1Part1Data_%1").arg(nowIndexPsu1Part1Data+1));
    frameListPsu1Part1Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu1Part1Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu1Part1Data_%1").arg(nowIndexPsu1Part1Data+1));
    checkBoxListPsu1Part1Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu1Part1Data_%1").arg(nowIndexPsu1Part1Data+1));
    dataLineEditListPsu1Part1Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu1Part1Data_%1").arg(nowIndexPsu1Part1Data+1));
    addrLineEditListPsu1Part1Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu1Part1Data++;    
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu1Part1()
{
    for(int i=0; i != checkBoxListPsu1Part1Data.size(); ++i){
        if(!checkBoxListPsu1Part1Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu1Part1DataAll_clicked()
{
    if(checkBoxAllSelectPsu1Part1()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu1Part1Data.size(); ++i){
            checkBoxListPsu1Part1Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu1Part1Data.size(); ++i){
            checkBoxListPsu1Part1Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part1DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu1Part1Data.size();){
        if(checkBoxListPsu1Part1Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu1Part1Data.removeAt(i);
            dataLineEditListPsu1Part1Data.removeAt(i);
            addrLineEditListPsu1Part1Data.removeAt(i);
            QFrame *tempFrame = frameListPsu1Part1Data.at(i);
            frameListPsu1Part1Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu1Part1Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu1Part1Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu1Part1Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu1Part1Data = checkBoxListPsu1Part1Data.size();
    int column = nowIndexPsu1Part1Data/10 + (nowIndexPsu1Part1Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu1Part1->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part1Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu1Part1DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu1Part1())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu1Part1DataAll_clicked();
    on_pushBtnPsu1Part1DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu1Part1DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu1Part1Data; ++i){
        dataLineEditListPsu1Part1Data.at(i)->clear();
        addrLineEditListPsu1Part1Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part1DataBatchAdd_clicked()
{
    if(nowIndexPsu1Part1Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu1Part1DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu1Part1(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu1Part1(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu1Part1DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu1Part1Data.at(nowIndexPsu1Part1Data-1)->setText(strData);
        addrLineEditListPsu1Part1Data.at(nowIndexPsu1Part1Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part1DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu1Part1Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu1Part1Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu1Part1Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu1Part1Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu1Part1->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu1Part1->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu1Part1->clear();
    dataAndAddrListPsu1Part1 = tempList;
    if(!dataAndAddrListPsu1Part1->isEmpty()){
        if(psu1Part1 == NULL){
            psu1Part1 = new testItem(cmdListPsu1Part1Pre, dataAndAddrListPsu1Part1,
                                     setCmdPsu1Part1Verify, setPsu1Part1Multi,
                                     dmmCmdPsu1Part1Verify, dmmPsu1Part1Multi,
                                     meterCmdPsu1Part1Verify, meterPsu1Part1Multi,
                                     setCmdPsu1Part1Test, dmmCmdPsu1Part1Test, meterCmdPsu1Part1Test);
        }else
            psu1Part1->setDataList(dataAndAddrListPsu1Part1);
        if(itemPsu1 == NULL)
            itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
        else
            itemPsu1->setPart1(psu1Part1);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part1DataUndo_clicked()
{
    on_pushBtnPsu1Part1DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu1Part1->size(); ++i){
        on_pushBtnPsu1Part1DataAdd_clicked();
        checkBoxListPsu1Part1Data.at(i)->setChecked(dataAndAddrListPsu1Part1->at(i)->first);
        dataLineEditListPsu1Part1Data.at(i)->setText(dataAndAddrListPsu1Part1->at(i)->second->first);
        addrLineEditListPsu1Part1Data.at(i)->setText(dataAndAddrListPsu1Part1->at(i)->second->second);
    }
}
// PSU1  Part1 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu1Part1VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part1VerifySetCmd->text();
    setStart = ui->lineEditPsu1Part1VerifySetStart->text();
    setEnd = ui->lineEditPsu1Part1VerifySetEnd->text();
    setJudge = ui->lineEditPsu1Part1VerifySetJudge->text();
    setMulti = ui->lineEditPsu1Part1VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part1VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu1Part1VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu1Part1VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part1VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part1VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu1Part1VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part1VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu1Part1VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part1Verify->setName(setName);  // �������õ����������
    setCmdPsu1Part1Verify->setStart(setStart);
    setCmdPsu1Part1Verify->setEnd(setEnd);
    setCmdPsu1Part1Verify->setJudge(setJudge);
    setPsu1Part1Multi = setMulti.toInt();
    dmmCmdPsu1Part1Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu1Part1Verify->setParam(dmmParam);
    dmmCmdPsu1Part1Verify->setStart(dmmStart);
    dmmCmdPsu1Part1Verify->setEnd(dmmEnd);
    dmmCmdPsu1Part1Verify->setRatio(dmmJudge.toDouble());
    dmmPsu1Part1Multi = dmmMulti.toInt();
    meterCmdPsu1Part1Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu1Part1Multi = meterMulti.toInt();
    if(psu1Part1 == NULL){
        psu1Part1 = new testItem(cmdListPsu1Part1Pre, dataAndAddrListPsu1Part1,
                                 setCmdPsu1Part1Verify, setPsu1Part1Multi,
                                 dmmCmdPsu1Part1Verify, dmmPsu1Part1Multi,
                                 meterCmdPsu1Part1Verify, meterPsu1Part1Multi,
                                 setCmdPsu1Part1Test, dmmCmdPsu1Part1Test, meterCmdPsu1Part1Test);
    }else{
        psu1Part1->setSetCmdVerify(setCmdPsu1Part1Verify);
        psu1Part1->setDmmCmdVerify(dmmCmdPsu1Part1Verify);
        psu1Part1->setMeterCmdVerify(meterCmdPsu1Part1Verify);
        psu1Part1->setSetMulti(setPsu1Part1Multi);
        psu1Part1->setDmmMulti(dmmPsu1Part1Multi);
        psu1Part1->setMeterMulti(meterPsu1Part1Multi);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart1(psu1Part1);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part1VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part1VerifySetCmd->setText(setCmdPsu1Part1Verify->getName());
    ui->lineEditPsu1Part1VerifySetStart->setText(setCmdPsu1Part1Verify->getStart());
    ui->lineEditPsu1Part1VerifySetEnd->setText(setCmdPsu1Part1Verify->getEnd());
    ui->lineEditPsu1Part1VerifySetJudge->setText(setCmdPsu1Part1Verify->getJudge());
    ui->lineEditPsu1Part1VerifySetMulti->setText(QString("%1").arg(setPsu1Part1Multi));
    // ��д��ȡ��������
    ui->lineEditPsu1Part1VerifyDMMCmd->setText(dmmCmdPsu1Part1Verify->getName());
    ui->lineEditPsu1Part1VerifyDMMStart->setText(dmmCmdPsu1Part1Verify->getStart());
    ui->lineEditPsu1Part1VerifyDMMEnd->setText(dmmCmdPsu1Part1Verify->getEnd());
    ui->lineEditPsu1Part1VerifyDMMJudge->setText(dmmCmdPsu1Part1Verify->getRatio());
    ui->lineEditPsu1Part1VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part1Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part1VerifyMeterJudge->setText(meterCmdPsu1Part1Verify->getRatio());
    ui->lineEditPsu1Part1VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part1Multi));
}
// PSU1  Part1  ����
// ���水ť
void curdataconfig::on_pushBtnPsu1Part1TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part1TestSetCmd->text();
    setStart = ui->lineEditPsu1Part1TestSetStart->text();
    setEnd = ui->lineEditPsu1Part1TestSetEnd->text();
    setJudge = ui->lineEditPsu1Part1TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part1TestDMMCmd->text();
    dmmStart = ui->lineEditPsu1Part1TestDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part1TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part1TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part1TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part1Test->setName(setName);  // �������õ����������
    setCmdPsu1Part1Test->setStart(setStart);
    setCmdPsu1Part1Test->setEnd(setEnd);
    setCmdPsu1Part1Test->setJudge(setJudge);
    dmmCmdPsu1Part1Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu1Part1Test->setStart(dmmStart);
    dmmCmdPsu1Part1Test->setEnd(dmmEnd);
    dmmCmdPsu1Part1Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu1Part1Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu1Part1 == NULL){
        psu1Part1 = new testItem(cmdListPsu1Part1Pre, dataAndAddrListPsu1Part1,
                                 setCmdPsu1Part1Verify, setPsu1Part1Multi,
                                 dmmCmdPsu1Part1Verify, dmmPsu1Part1Multi,
                                 meterCmdPsu1Part1Verify, meterPsu1Part1Multi,
                                 setCmdPsu1Part1Test, dmmCmdPsu1Part1Test, meterCmdPsu1Part1Test);
    }else{
        psu1Part1->setSetCmdTest(setCmdPsu1Part1Test);
        psu1Part1->setDmmCmdTest(dmmCmdPsu1Part1Test);
        psu1Part1->setMeterCmdTest(meterCmdPsu1Part1Test);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart1(psu1Part1);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part1TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part1TestSetCmd->setText(setCmdPsu1Part1Test->getName());
    ui->lineEditPsu1Part1TestSetStart->setText(setCmdPsu1Part1Test->getStart());
    ui->lineEditPsu1Part1TestSetEnd->setText(setCmdPsu1Part1Test->getEnd());
    ui->lineEditPsu1Part1TestSetJudge->setText(setCmdPsu1Part1Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu1Part1TestDMMCmd->setText(dmmCmdPsu1Part1Test->getName());
    ui->lineEditPsu1Part1TestDMMStart->setText(dmmCmdPsu1Part1Test->getStart());
    ui->lineEditPsu1Part1TestDMMEnd->setText(dmmCmdPsu1Part1Test->getEnd());
    ui->lineEditPsu1Part1TestDMMJudge->setText(dmmCmdPsu1Part1Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part1TestMeterJudge->setText(meterCmdPsu1Part1Test->getRatio());
}
// PSU1  Part2  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part2PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu1Part2Pre->append(newCommand);
    if(psu1Part2 == NULL){
        psu1Part2 = new testItem(cmdListPsu1Part2Pre, dataAndAddrListPsu1Part2,
                                     setCmdPsu1Part2Verify, setPsu1Part2Multi,
                                     dmmCmdPsu1Part2Verify, dmmPsu1Part2Multi,
                                     meterCmdPsu1Part2Verify, meterPsu1Part2Multi,
                                     setCmdPsu1Part2Test, dmmCmdPsu1Part2Test, meterCmdPsu1Part2Test);
    }else
        psu1Part2->setCmdList(cmdListPsu1Part2Pre);
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart2(psu1Part2);
    showPsu1Part2PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu1Part2PreCmdList()
{
    ui->psu1Part2PreCmdList->clear();
    for(int i=0; i != cmdListPsu1Part2Pre->size(); ++i){
        command *curCommand = cmdListPsu1Part2Pre->at(i);
        ui->psu1Part2PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu1Part2PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu1Part2Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu1Part2 = cmdListPsu1Part2Pre->at(nowIndexPsu1Part2Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu1Part2PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part2PreDel_clicked()
{
    if(nowIndexPsu1Part2Pre == -1){
        return ;
    }
    cmdListPsu1Part2Pre->removeAt(nowIndexPsu1Part2Pre);
    nowIndexPsu1Part2Pre = cmdListPsu1Part2Pre->size()-1;
    if(nowIndexPsu1Part2Pre != -1)
        nowCommandPsu1Part2 = cmdListPsu1Part2Pre->at(nowIndexPsu1Part2Pre);
    showPsu1Part2PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu1Part2PreName->clear();
    ui->lineEditPsu1Part2PreParam->clear();
    ui->lineEditPsu1Part2PreStart->clear();
    ui->lineEditPsu1Part2PreEnd->clear();
    ui->lineEditPsu1Part2PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu1Part2PreUp_clicked()
{
    if(nowIndexPsu1Part2Pre < 1)
        return;
    cmdListPsu1Part2Pre->swap(nowIndexPsu1Part2Pre, nowIndexPsu1Part2Pre-1);
    nowIndexPsu1Part2Pre--;
    showPsu1Part2PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu1Part2PreDown_clicked()
{
    if((nowIndexPsu1Part2Pre == -1)||(nowIndexPsu1Part2Pre >= cmdListPsu1Part2Pre->size()-1))
        return;
    cmdListPsu1Part2Pre->swap(nowIndexPsu1Part2Pre, nowIndexPsu1Part2Pre+1);
    nowIndexPsu1Part2Pre++;
    showPsu1Part2PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part2PreSave_clicked()
{
    if(ui->lineEditPsu1Part2PreName->text().size() == 0 &&
            ui->lineEditPsu1Part2PreParam->text().size() == 0 &&
            ui->lineEditPsu1Part2PreStart->text().size() == 0 &&
            ui->lineEditPsu1Part2PreEnd->text().size() == 0 &&
            ui->lineEditPsu1Part2PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu1Part2 != NULL){
        nowCommandPsu1Part2->setName(ui->lineEditPsu1Part2PreName->text());
        nowCommandPsu1Part2->setParam(ui->lineEditPsu1Part2PreParam->text());
        nowCommandPsu1Part2->setStart(ui->lineEditPsu1Part2PreStart->text());
        nowCommandPsu1Part2->setEnd(ui->lineEditPsu1Part2PreEnd->text());
        nowCommandPsu1Part2->setJudge(ui->lineEditPsu1Part2PreJudge->text());
        showPsu1Part2PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part2PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu1Part2PreName->clear();
    ui->lineEditPsu1Part2PreParam->clear();
    ui->lineEditPsu1Part2PreStart->clear();
    ui->lineEditPsu1Part2PreEnd->clear();
    ui->lineEditPsu1Part2PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu1Part2PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu1Part2PreName->setText(nowCommandPsu1Part2->getName());
    // ��ʾ�������
    ui->lineEditPsu1Part2PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu1Part2PreParam->setText(nowCommandPsu1Part2->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu1Part2PreStart->setText(nowCommandPsu1Part2->getStart());
    ui->lineEditPsu1Part2PreEnd->setText(nowCommandPsu1Part2->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu1Part2PreJudge->setText(nowCommandPsu1Part2->getJudge());
}
// PSU1  Part2  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part2DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu1Part2Data / 10;
    y = nowIndexPsu1Part2Data % 10;
    ui->scrollAreaWidgetContentsPsu1Part2->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part2Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu1Part2);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu1Part2Data_%1").arg(nowIndexPsu1Part2Data+1));
    frameListPsu1Part2Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu1Part2Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu1Part2Data_%1").arg(nowIndexPsu1Part2Data+1));
    checkBoxListPsu1Part2Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu1Part2Data_%1").arg(nowIndexPsu1Part2Data+1));
    dataLineEditListPsu1Part2Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu1Part2Data_%1").arg(nowIndexPsu1Part2Data+1));
    addrLineEditListPsu1Part2Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu1Part2Data++;
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu1Part2()
{
    for(int i=0; i != checkBoxListPsu1Part2Data.size(); ++i){
        if(!checkBoxListPsu1Part2Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu1Part2DataAll_clicked()
{
    if(checkBoxAllSelectPsu1Part2()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu1Part2Data.size(); ++i){
            checkBoxListPsu1Part2Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu1Part2Data.size(); ++i){
            checkBoxListPsu1Part2Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part2DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu1Part2Data.size();){
        if(checkBoxListPsu1Part2Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu1Part2Data.removeAt(i);
            dataLineEditListPsu1Part2Data.removeAt(i);
            addrLineEditListPsu1Part2Data.removeAt(i);
            QFrame *tempFrame = frameListPsu1Part2Data.at(i);
            frameListPsu1Part2Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu1Part2Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu1Part2Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu1Part2Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu1Part2Data = checkBoxListPsu1Part2Data.size();
    int column = nowIndexPsu1Part2Data/10 + (nowIndexPsu1Part2Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu1Part2->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part2Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu1Part2DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu1Part2())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu1Part2DataAll_clicked();
    on_pushBtnPsu1Part2DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu1Part2DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu1Part2Data; ++i){
        dataLineEditListPsu1Part2Data.at(i)->clear();
        addrLineEditListPsu1Part2Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part2DataBatchAdd_clicked()
{
    if(nowIndexPsu1Part2Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu1Part2DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu1Part2(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu1Part2(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu1Part2DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu1Part2Data.at(nowIndexPsu1Part2Data-1)->setText(strData);
        addrLineEditListPsu1Part2Data.at(nowIndexPsu1Part2Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part2DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu1Part2Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu1Part2Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu1Part2Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu1Part2Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu1Part2->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu1Part2->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu1Part2->clear();
    dataAndAddrListPsu1Part2 = tempList;
    if(!dataAndAddrListPsu1Part2->isEmpty()){
        if(psu1Part2 == NULL){
            psu1Part2 = new testItem(cmdListPsu1Part2Pre, dataAndAddrListPsu1Part2,
                                     setCmdPsu1Part2Verify, setPsu1Part2Multi,
                                     dmmCmdPsu1Part2Verify, dmmPsu1Part2Multi,
                                     meterCmdPsu1Part2Verify, meterPsu1Part2Multi,
                                     setCmdPsu1Part2Test, dmmCmdPsu1Part2Test, meterCmdPsu1Part2Test);
        }else
            psu1Part2->setDataList(dataAndAddrListPsu1Part2);
        if(itemPsu1 == NULL)
            itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
        else
            itemPsu1->setPart2(psu1Part2);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part2DataUndo_clicked()
{
    on_pushBtnPsu1Part2DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu1Part2->size(); ++i){
        on_pushBtnPsu1Part2DataAdd_clicked();
        checkBoxListPsu1Part2Data.at(i)->setChecked(dataAndAddrListPsu1Part2->at(i)->first);
        dataLineEditListPsu1Part2Data.at(i)->setText(dataAndAddrListPsu1Part2->at(i)->second->first);
        addrLineEditListPsu1Part2Data.at(i)->setText(dataAndAddrListPsu1Part2->at(i)->second->second);
    }
}
// PSU1  Part2 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu1Part2VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part2VerifySetCmd->text();
    setStart = ui->lineEditPsu1Part2VerifySetStart->text();
    setEnd = ui->lineEditPsu1Part2VerifySetEnd->text();
    setJudge = ui->lineEditPsu1Part2VerifySetJudge->text();
    setMulti = ui->lineEditPsu1Part2VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part2VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu1Part2VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu1Part2VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part2VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part2VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu1Part2VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part2VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu1Part2VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part2Verify->setName(setName);  // �������õ����������
    setCmdPsu1Part2Verify->setStart(setStart);
    setCmdPsu1Part2Verify->setEnd(setEnd);
    setCmdPsu1Part2Verify->setJudge(setJudge);
    setPsu1Part2Multi = setMulti.toInt();
    dmmCmdPsu1Part2Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu1Part2Verify->setParam(dmmParam);
    dmmCmdPsu1Part2Verify->setStart(dmmStart);
    dmmCmdPsu1Part2Verify->setEnd(dmmEnd);
    dmmCmdPsu1Part2Verify->setRatio(dmmJudge.toDouble());
    dmmPsu1Part2Multi = dmmMulti.toInt();
    meterCmdPsu1Part2Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu1Part2Multi = meterMulti.toInt();
    if(psu1Part2 == NULL){
        psu1Part2 = new testItem(cmdListPsu1Part2Pre, dataAndAddrListPsu1Part2,
                                 setCmdPsu1Part2Verify, setPsu1Part2Multi,
                                 dmmCmdPsu1Part2Verify, dmmPsu1Part2Multi,
                                 meterCmdPsu1Part2Verify, meterPsu1Part2Multi,
                                 setCmdPsu1Part2Test, dmmCmdPsu1Part2Test, meterCmdPsu1Part2Test);
    }else{
        psu1Part2->setSetCmdVerify(setCmdPsu1Part2Verify);
        psu1Part2->setDmmCmdVerify(dmmCmdPsu1Part2Verify);
        psu1Part2->setMeterCmdVerify(meterCmdPsu1Part2Verify);
        psu1Part2->setSetMulti(setPsu1Part2Multi);
        psu1Part2->setDmmMulti(dmmPsu1Part2Multi);
        psu1Part2->setMeterMulti(meterPsu1Part2Multi);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart2(psu1Part2);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part2VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part2VerifySetCmd->setText(setCmdPsu1Part2Verify->getName());
    ui->lineEditPsu1Part2VerifySetStart->setText(setCmdPsu1Part2Verify->getStart());
    ui->lineEditPsu1Part2VerifySetEnd->setText(setCmdPsu1Part2Verify->getEnd());
    ui->lineEditPsu1Part2VerifySetJudge->setText(setCmdPsu1Part2Verify->getJudge());
    ui->lineEditPsu1Part2VerifySetMulti->setText(QString("%1").arg(setPsu1Part2Multi));
    // ��д��ȡ��������
    ui->lineEditPsu1Part2VerifyDMMCmd->setText(dmmCmdPsu1Part2Verify->getName());
    ui->lineEditPsu1Part2VerifyDMMStart->setText(dmmCmdPsu1Part2Verify->getStart());
    ui->lineEditPsu1Part2VerifyDMMEnd->setText(dmmCmdPsu1Part2Verify->getEnd());
    ui->lineEditPsu1Part2VerifyDMMJudge->setText(dmmCmdPsu1Part2Verify->getRatio());
    ui->lineEditPsu1Part2VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part2Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part2VerifyMeterJudge->setText(meterCmdPsu1Part2Verify->getRatio());
    ui->lineEditPsu1Part2VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part2Multi));
}
// PSU1  Part2  ����
// ���水ť
void curdataconfig::on_pushBtnPsu1Part2TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part2TestSetCmd->text();
    setStart = ui->lineEditPsu1Part2TestSetStart->text();
    setEnd = ui->lineEditPsu1Part2TestSetEnd->text();
    setJudge = ui->lineEditPsu1Part2TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part2TestDMMCmd->text();
    dmmStart = ui->lineEditPsu1Part2TestDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part2TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part2TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part2TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part2Test->setName(setName);  // �������õ����������
    setCmdPsu1Part2Test->setStart(setStart);
    setCmdPsu1Part2Test->setEnd(setEnd);
    setCmdPsu1Part2Test->setJudge(setJudge);
    dmmCmdPsu1Part2Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu1Part2Test->setStart(dmmStart);
    dmmCmdPsu1Part2Test->setEnd(dmmEnd);
    dmmCmdPsu1Part2Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu1Part2Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu1Part2 == NULL){
        psu1Part2 = new testItem(cmdListPsu1Part2Pre, dataAndAddrListPsu1Part2,
                                 setCmdPsu1Part2Verify, setPsu1Part2Multi,
                                 dmmCmdPsu1Part2Verify, dmmPsu1Part2Multi,
                                 meterCmdPsu1Part2Verify, meterPsu1Part2Multi,
                                 setCmdPsu1Part2Test, dmmCmdPsu1Part2Test, meterCmdPsu1Part2Test);
    }else{
        psu1Part2->setSetCmdTest(setCmdPsu1Part2Test);
        psu1Part2->setDmmCmdTest(dmmCmdPsu1Part2Test);
        psu1Part2->setMeterCmdTest(meterCmdPsu1Part2Test);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart2(psu1Part2);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part2TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part2TestSetCmd->setText(setCmdPsu1Part2Test->getName());
    ui->lineEditPsu1Part2TestSetStart->setText(setCmdPsu1Part2Test->getStart());
    ui->lineEditPsu1Part2TestSetEnd->setText(setCmdPsu1Part2Test->getEnd());
    ui->lineEditPsu1Part2TestSetJudge->setText(setCmdPsu1Part2Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu1Part2TestDMMCmd->setText(dmmCmdPsu1Part2Test->getName());
    ui->lineEditPsu1Part2TestDMMStart->setText(dmmCmdPsu1Part2Test->getStart());
    ui->lineEditPsu1Part2TestDMMEnd->setText(dmmCmdPsu1Part2Test->getEnd());
    ui->lineEditPsu1Part2TestDMMJudge->setText(dmmCmdPsu1Part2Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part2TestMeterJudge->setText(meterCmdPsu1Part2Test->getRatio());
}
// PSU1  Part3  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part3PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu1Part3Pre->append(newCommand);
    if(psu1Part3 == NULL){
        psu1Part3 = new testItem(cmdListPsu1Part3Pre, dataAndAddrListPsu1Part3,
                                 setCmdPsu1Part3Verify, setPsu1Part3Multi,
                                 dmmCmdPsu1Part3Verify, dmmPsu1Part3Multi,
                                 meterCmdPsu1Part3Verify, meterPsu1Part3Multi,
                                 setCmdPsu1Part3Test, dmmCmdPsu1Part3Test, meterCmdPsu1Part3Test);
    }else
        psu1Part3->setCmdList(cmdListPsu1Part3Pre);
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart3(psu1Part3);
    showPsu1Part3PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu1Part3PreCmdList()
{
    ui->psu1Part3PreCmdList->clear();
    for(int i=0; i != cmdListPsu1Part3Pre->size(); ++i){
        command *curCommand = cmdListPsu1Part3Pre->at(i);
        ui->psu1Part3PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu1Part3PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu1Part3Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu1Part3 = cmdListPsu1Part3Pre->at(nowIndexPsu1Part3Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu1Part3PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part3PreDel_clicked()
{
    if(nowIndexPsu1Part3Pre == -1){
        return ;
    }
    cmdListPsu1Part3Pre->removeAt(nowIndexPsu1Part3Pre);
    nowIndexPsu1Part3Pre = cmdListPsu1Part3Pre->size()-1;
    if(nowIndexPsu1Part3Pre != -1)
        nowCommandPsu1Part3 = cmdListPsu1Part3Pre->at(nowIndexPsu1Part3Pre);
    showPsu1Part3PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu1Part3PreName->clear();
    ui->lineEditPsu1Part3PreParam->clear();
    ui->lineEditPsu1Part3PreStart->clear();
    ui->lineEditPsu1Part3PreEnd->clear();
    ui->lineEditPsu1Part3PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu1Part3PreUp_clicked()
{
    if(nowIndexPsu1Part3Pre < 1)
        return;
    cmdListPsu1Part3Pre->swap(nowIndexPsu1Part3Pre, nowIndexPsu1Part3Pre-1);
    nowIndexPsu1Part3Pre--;
    showPsu1Part3PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu1Part3PreDown_clicked()
{
    if((nowIndexPsu1Part3Pre == -1)||(nowIndexPsu1Part3Pre >= cmdListPsu1Part3Pre->size()-1))
        return;
    cmdListPsu1Part3Pre->swap(nowIndexPsu1Part3Pre, nowIndexPsu1Part3Pre+1);
    nowIndexPsu1Part3Pre++;
    showPsu1Part3PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part3PreSave_clicked()
{
    if(ui->lineEditPsu1Part3PreName->text().size() == 0 &&
            ui->lineEditPsu1Part3PreParam->text().size() == 0 &&
            ui->lineEditPsu1Part3PreStart->text().size() == 0 &&
            ui->lineEditPsu1Part3PreEnd->text().size() == 0 &&
            ui->lineEditPsu1Part3PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu1Part3 != NULL){
        nowCommandPsu1Part3->setName(ui->lineEditPsu1Part3PreName->text());
        nowCommandPsu1Part3->setParam(ui->lineEditPsu1Part3PreParam->text());
        nowCommandPsu1Part3->setStart(ui->lineEditPsu1Part3PreStart->text());
        nowCommandPsu1Part3->setEnd(ui->lineEditPsu1Part3PreEnd->text());
        nowCommandPsu1Part3->setJudge(ui->lineEditPsu1Part3PreJudge->text());
        showPsu1Part3PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part3PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu1Part3PreName->clear();
    ui->lineEditPsu1Part3PreParam->clear();
    ui->lineEditPsu1Part3PreStart->clear();
    ui->lineEditPsu1Part3PreEnd->clear();
    ui->lineEditPsu1Part3PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu1Part3PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu1Part3PreName->setText(nowCommandPsu1Part3->getName());
    // ��ʾ�������
    ui->lineEditPsu1Part3PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu1Part3PreParam->setText(nowCommandPsu1Part3->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu1Part3PreStart->setText(nowCommandPsu1Part3->getStart());
    ui->lineEditPsu1Part3PreEnd->setText(nowCommandPsu1Part3->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu1Part3PreJudge->setText(nowCommandPsu1Part3->getJudge());
}
// PSU1  Part3  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part3DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu1Part3Data / 10;
    y = nowIndexPsu1Part3Data % 10;
    ui->scrollAreaWidgetContentsPsu1Part3->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part3Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu1Part3);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu1Part3Data_%1").arg(nowIndexPsu1Part3Data+1));
    frameListPsu1Part3Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu1Part3Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu1Part3Data_%1").arg(nowIndexPsu1Part3Data+1));
    checkBoxListPsu1Part3Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu1Part3Data_%1").arg(nowIndexPsu1Part3Data+1));
    dataLineEditListPsu1Part3Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu1Part3Data_%1").arg(nowIndexPsu1Part3Data+1));
    addrLineEditListPsu1Part3Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu1Part3Data++;    
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu1Part3()
{
    for(int i=0; i != checkBoxListPsu1Part3Data.size(); ++i){
        if(!checkBoxListPsu1Part3Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu1Part3DataAll_clicked()
{
    if(checkBoxAllSelectPsu1Part3()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu1Part3Data.size(); ++i){
            checkBoxListPsu1Part3Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu1Part3Data.size(); ++i){
            checkBoxListPsu1Part3Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part3DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu1Part3Data.size();){
        if(checkBoxListPsu1Part3Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu1Part3Data.removeAt(i);
            dataLineEditListPsu1Part3Data.removeAt(i);
            addrLineEditListPsu1Part3Data.removeAt(i);
            QFrame *tempFrame = frameListPsu1Part3Data.at(i);
            frameListPsu1Part3Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu1Part3Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu1Part3Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu1Part3Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu1Part3Data = checkBoxListPsu1Part3Data.size();
    int column = nowIndexPsu1Part3Data/10 + (nowIndexPsu1Part3Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu1Part3->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part3Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu1Part3DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu1Part3())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu1Part3DataAll_clicked();
    on_pushBtnPsu1Part3DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu1Part3DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu1Part3Data; ++i){
        dataLineEditListPsu1Part3Data.at(i)->clear();
        addrLineEditListPsu1Part3Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part3DataBatchAdd_clicked()
{
    if(nowIndexPsu1Part3Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu1Part3DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu1Part3(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu1Part3(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu1Part3DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu1Part3Data.at(nowIndexPsu1Part3Data-1)->setText(strData);
        addrLineEditListPsu1Part3Data.at(nowIndexPsu1Part3Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part3DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu1Part3Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu1Part3Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu1Part3Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu1Part3Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu1Part3->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu1Part3->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu1Part3->clear();
    dataAndAddrListPsu1Part3 = tempList;
    if(!dataAndAddrListPsu1Part3->isEmpty()){
        if(psu1Part3 == NULL){
            psu1Part3 = new testItem(cmdListPsu1Part3Pre, dataAndAddrListPsu1Part3,
                                     setCmdPsu1Part3Verify, setPsu1Part3Multi,
                                     dmmCmdPsu1Part3Verify, dmmPsu1Part3Multi,
                                     meterCmdPsu1Part3Verify, meterPsu1Part3Multi,
                                     setCmdPsu1Part3Test, dmmCmdPsu1Part3Test, meterCmdPsu1Part3Test);
        }else
            psu1Part3->setDataList(dataAndAddrListPsu1Part3);
        if(itemPsu1 == NULL)
            itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
        else
            itemPsu1->setPart3(psu1Part3);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part3DataUndo_clicked()
{
    on_pushBtnPsu1Part3DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu1Part3->size(); ++i){
        on_pushBtnPsu1Part3DataAdd_clicked();
        checkBoxListPsu1Part3Data.at(i)->setChecked(dataAndAddrListPsu1Part3->at(i)->first);
        dataLineEditListPsu1Part3Data.at(i)->setText(dataAndAddrListPsu1Part3->at(i)->second->first);
        addrLineEditListPsu1Part3Data.at(i)->setText(dataAndAddrListPsu1Part3->at(i)->second->second);
    }
}
// PSU1  Part3 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu1Part3VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part3VerifySetCmd->text();
    setStart = ui->lineEditPsu1Part3VerifySetStart->text();
    setEnd = ui->lineEditPsu1Part3VerifySetEnd->text();
    setJudge = ui->lineEditPsu1Part3VerifySetJudge->text();
    setMulti = ui->lineEditPsu1Part3VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part3VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu1Part3VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu1Part3VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part3VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part3VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu1Part3VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part3VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu1Part3VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part3Verify->setName(setName);  // �������õ����������
    setCmdPsu1Part3Verify->setStart(setStart);
    setCmdPsu1Part3Verify->setEnd(setEnd);
    setCmdPsu1Part3Verify->setJudge(setJudge);
    setPsu1Part3Multi = setMulti.toInt();
    dmmCmdPsu1Part3Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu1Part3Verify->setParam(dmmParam);
    dmmCmdPsu1Part3Verify->setStart(dmmStart);
    dmmCmdPsu1Part3Verify->setEnd(dmmEnd);
    dmmCmdPsu1Part3Verify->setRatio(dmmJudge.toDouble());
    dmmPsu1Part3Multi = dmmMulti.toInt();
    meterCmdPsu1Part3Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu1Part3Multi = meterMulti.toInt();
    if(psu1Part3 == NULL){
        psu1Part3 = new testItem(cmdListPsu1Part3Pre, dataAndAddrListPsu1Part3,
                                 setCmdPsu1Part3Verify, setPsu1Part3Multi,
                                 dmmCmdPsu1Part3Verify, dmmPsu1Part3Multi,
                                 meterCmdPsu1Part3Verify, meterPsu1Part3Multi,
                                 setCmdPsu1Part3Test, dmmCmdPsu1Part3Test, meterCmdPsu1Part3Test);
        if(itemPsu1 == NULL)
            itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
        else
            itemPsu1->setPart3(psu1Part3);
    }else{
        psu1Part3->setSetCmdVerify(setCmdPsu1Part3Verify);
        psu1Part3->setDmmCmdVerify(dmmCmdPsu1Part3Verify);
        psu1Part3->setMeterCmdVerify(meterCmdPsu1Part3Verify);
        psu1Part3->setSetMulti(setPsu1Part3Multi);
        psu1Part3->setDmmMulti(dmmPsu1Part3Multi);
        psu1Part3->setMeterMulti(meterPsu1Part3Multi);
    }
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part3VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part3VerifySetCmd->setText(setCmdPsu1Part3Verify->getName());
    ui->lineEditPsu1Part3VerifySetStart->setText(setCmdPsu1Part3Verify->getStart());
    ui->lineEditPsu1Part3VerifySetEnd->setText(setCmdPsu1Part3Verify->getEnd());
    ui->lineEditPsu1Part3VerifySetJudge->setText(setCmdPsu1Part3Verify->getJudge());
    ui->lineEditPsu1Part3VerifySetMulti->setText(QString("%1").arg(setPsu1Part3Multi));
    // ��д��ȡ��������
    ui->lineEditPsu1Part3VerifyDMMCmd->setText(dmmCmdPsu1Part3Verify->getName());
    ui->lineEditPsu1Part3VerifyDMMStart->setText(dmmCmdPsu1Part3Verify->getStart());
    ui->lineEditPsu1Part3VerifyDMMEnd->setText(dmmCmdPsu1Part3Verify->getEnd());
    ui->lineEditPsu1Part3VerifyDMMJudge->setText(dmmCmdPsu1Part3Verify->getRatio());
    ui->lineEditPsu1Part3VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part3Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part3VerifyMeterJudge->setText(meterCmdPsu1Part3Verify->getRatio());
    ui->lineEditPsu1Part3VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part3Multi));
}
// PSU1  Part3  ����
// ���水ť
void curdataconfig::on_pushBtnPsu1Part3TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part3TestSetCmd->text();
    setStart = ui->lineEditPsu1Part3TestSetStart->text();
    setEnd = ui->lineEditPsu1Part3TestSetEnd->text();
    setJudge = ui->lineEditPsu1Part3TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part3TestDMMCmd->text();
    dmmStart = ui->lineEditPsu1Part3TestDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part3TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part3TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part3TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part3Test->setName(setName);  // �������õ����������
    setCmdPsu1Part3Test->setStart(setStart);
    setCmdPsu1Part3Test->setEnd(setEnd);
    setCmdPsu1Part3Test->setJudge(setJudge);
    dmmCmdPsu1Part3Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu1Part3Test->setStart(dmmStart);
    dmmCmdPsu1Part3Test->setEnd(dmmEnd);
    dmmCmdPsu1Part3Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu1Part3Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu1Part3 == NULL){
        psu1Part3 = new testItem(cmdListPsu1Part3Pre, dataAndAddrListPsu1Part3,
                                 setCmdPsu1Part3Verify, setPsu1Part3Multi,
                                 dmmCmdPsu1Part3Verify, dmmPsu1Part3Multi,
                                 meterCmdPsu1Part3Verify, meterPsu1Part3Multi,
                                 setCmdPsu1Part3Test, dmmCmdPsu1Part3Test, meterCmdPsu1Part3Test);
    }else{
        psu1Part3->setSetCmdTest(setCmdPsu1Part3Test);
        psu1Part3->setDmmCmdTest(dmmCmdPsu1Part3Test);
        psu1Part3->setMeterCmdTest(meterCmdPsu1Part3Test);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart3(psu1Part3);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part3TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part3TestSetCmd->setText(setCmdPsu1Part3Test->getName());
    ui->lineEditPsu1Part3TestSetStart->setText(setCmdPsu1Part3Test->getStart());
    ui->lineEditPsu1Part3TestSetEnd->setText(setCmdPsu1Part3Test->getEnd());
    ui->lineEditPsu1Part3TestSetJudge->setText(setCmdPsu1Part3Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu1Part3TestDMMCmd->setText(dmmCmdPsu1Part3Test->getName());
    ui->lineEditPsu1Part3TestDMMStart->setText(dmmCmdPsu1Part3Test->getStart());
    ui->lineEditPsu1Part3TestDMMEnd->setText(dmmCmdPsu1Part3Test->getEnd());
    ui->lineEditPsu1Part3TestDMMJudge->setText(dmmCmdPsu1Part3Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part3TestMeterJudge->setText(meterCmdPsu1Part3Test->getRatio());
}
// PSU1  Part4  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part4PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu1Part4Pre->append(newCommand);
    if(psu1Part4 == NULL){
        psu1Part4 = new testItem(cmdListPsu1Part4Pre, dataAndAddrListPsu1Part4,
                                 setCmdPsu1Part4Verify, setPsu1Part4Multi,
                                 dmmCmdPsu1Part4Verify, dmmPsu1Part4Multi,
                                 meterCmdPsu1Part4Verify, meterPsu1Part4Multi,
                                 setCmdPsu1Part4Test, dmmCmdPsu1Part4Test, meterCmdPsu1Part4Test);
    }else
        psu1Part4->setCmdList(cmdListPsu1Part4Pre);
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart4(psu1Part4);
    showPsu1Part4PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu1Part4PreCmdList()
{
    ui->psu1Part4PreCmdList->clear();
    for(int i=0; i != cmdListPsu1Part4Pre->size(); ++i){
        command *curCommand = cmdListPsu1Part4Pre->at(i);
        ui->psu1Part4PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu1Part4PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu1Part4Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu1Part4 = cmdListPsu1Part4Pre->at(nowIndexPsu1Part4Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu1Part4PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part4PreDel_clicked()
{
    if(nowIndexPsu1Part4Pre == -1){
        return ;
    }
    cmdListPsu1Part4Pre->removeAt(nowIndexPsu1Part4Pre);
    nowIndexPsu1Part4Pre = cmdListPsu1Part4Pre->size()-1;
    if(nowIndexPsu1Part4Pre != -1)
        nowCommandPsu1Part4 = cmdListPsu1Part4Pre->at(nowIndexPsu1Part4Pre);
    showPsu1Part4PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu1Part4PreName->clear();
    ui->lineEditPsu1Part4PreParam->clear();
    ui->lineEditPsu1Part4PreStart->clear();
    ui->lineEditPsu1Part4PreEnd->clear();
    ui->lineEditPsu1Part4PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu1Part4PreUp_clicked()
{
    if(nowIndexPsu1Part4Pre < 1)
        return;
    cmdListPsu1Part4Pre->swap(nowIndexPsu1Part4Pre, nowIndexPsu1Part4Pre-1);
    nowIndexPsu1Part4Pre--;
    showPsu1Part4PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu1Part4PreDown_clicked()
{
    if((nowIndexPsu1Part4Pre == -1)||(nowIndexPsu1Part4Pre >= cmdListPsu1Part4Pre->size()-1))
        return;
    cmdListPsu1Part4Pre->swap(nowIndexPsu1Part4Pre, nowIndexPsu1Part4Pre+1);
    nowIndexPsu1Part4Pre++;
    showPsu1Part4PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part4PreSave_clicked()
{
    if(ui->lineEditPsu1Part4PreName->text().size() == 0 &&
            ui->lineEditPsu1Part4PreParam->text().size() == 0 &&
            ui->lineEditPsu1Part4PreStart->text().size() == 0 &&
            ui->lineEditPsu1Part4PreEnd->text().size() == 0 &&
            ui->lineEditPsu1Part4PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu1Part4 != NULL){
        nowCommandPsu1Part4->setName(ui->lineEditPsu1Part4PreName->text());
        nowCommandPsu1Part4->setParam(ui->lineEditPsu1Part4PreParam->text());
        nowCommandPsu1Part4->setStart(ui->lineEditPsu1Part4PreStart->text());
        nowCommandPsu1Part4->setEnd(ui->lineEditPsu1Part4PreEnd->text());
        nowCommandPsu1Part4->setJudge(ui->lineEditPsu1Part4PreJudge->text());
        showPsu1Part4PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part4PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu1Part4PreName->clear();
    ui->lineEditPsu1Part4PreParam->clear();
    ui->lineEditPsu1Part4PreStart->clear();
    ui->lineEditPsu1Part4PreEnd->clear();
    ui->lineEditPsu1Part4PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu1Part4PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu1Part4PreName->setText(nowCommandPsu1Part4->getName());
    // ��ʾ�������
    ui->lineEditPsu1Part4PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu1Part4PreParam->setText(nowCommandPsu1Part4->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu1Part4PreStart->setText(nowCommandPsu1Part4->getStart());
    ui->lineEditPsu1Part4PreEnd->setText(nowCommandPsu1Part4->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu1Part4PreJudge->setText(nowCommandPsu1Part4->getJudge());
}
// PSU1  Part4  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part4DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu1Part4Data / 10;
    y = nowIndexPsu1Part4Data % 10;
    ui->scrollAreaWidgetContentsPsu1Part4->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part4Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu1Part4);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu1Part4Data_%1").arg(nowIndexPsu1Part4Data+1));
    frameListPsu1Part4Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu1Part4Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu1Part4Data_%1").arg(nowIndexPsu1Part4Data+1));
    checkBoxListPsu1Part4Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu1Part4Data_%1").arg(nowIndexPsu1Part4Data+1));
    dataLineEditListPsu1Part4Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu1Part4Data_%1").arg(nowIndexPsu1Part4Data+1));
    addrLineEditListPsu1Part4Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu1Part4Data++;    
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu1Part4()
{
    for(int i=0; i != checkBoxListPsu1Part4Data.size(); ++i){
        if(!checkBoxListPsu1Part4Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu1Part4DataAll_clicked()
{
    if(checkBoxAllSelectPsu1Part4()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu1Part4Data.size(); ++i){
            checkBoxListPsu1Part4Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu1Part4Data.size(); ++i){
            checkBoxListPsu1Part4Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part4DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu1Part4Data.size();){
        if(checkBoxListPsu1Part4Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu1Part4Data.removeAt(i);
            dataLineEditListPsu1Part4Data.removeAt(i);
            addrLineEditListPsu1Part4Data.removeAt(i);
            QFrame *tempFrame = frameListPsu1Part4Data.at(i);
            frameListPsu1Part4Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu1Part4Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu1Part4Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu1Part4Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu1Part4Data = checkBoxListPsu1Part4Data.size();
    int column = nowIndexPsu1Part4Data/10 + (nowIndexPsu1Part4Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu1Part4->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part4Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu1Part4DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu1Part4())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu1Part4DataAll_clicked();
    on_pushBtnPsu1Part4DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu1Part4DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu1Part4Data; ++i){
        dataLineEditListPsu1Part4Data.at(i)->clear();
        addrLineEditListPsu1Part4Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part4DataBatchAdd_clicked()
{
    if(nowIndexPsu1Part4Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu1Part4DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu1Part4(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu1Part4(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu1Part4DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu1Part4Data.at(nowIndexPsu1Part4Data-1)->setText(strData);
        addrLineEditListPsu1Part4Data.at(nowIndexPsu1Part4Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part4DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu1Part4Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu1Part4Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu1Part4Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu1Part4Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu1Part4->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu1Part4->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu1Part4->clear();
    dataAndAddrListPsu1Part4 = tempList;
    if(!dataAndAddrListPsu1Part4->isEmpty()){
        if(psu1Part4 == NULL){
            psu1Part4 = new testItem(cmdListPsu1Part4Pre, dataAndAddrListPsu1Part4,
                                     setCmdPsu1Part4Verify, setPsu1Part4Multi,
                                     dmmCmdPsu1Part4Verify, dmmPsu1Part4Multi,
                                     meterCmdPsu1Part4Verify, meterPsu1Part4Multi,
                                     setCmdPsu1Part4Test, dmmCmdPsu1Part4Test, meterCmdPsu1Part4Test);
        }else
            psu1Part4->setDataList(dataAndAddrListPsu1Part4);
        if(itemPsu1 == NULL)
            itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
        else
            itemPsu1->setPart4(psu1Part4);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part4DataUndo_clicked()
{
    on_pushBtnPsu1Part4DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu1Part4->size(); ++i){
        on_pushBtnPsu1Part4DataAdd_clicked();
        checkBoxListPsu1Part4Data.at(i)->setChecked(dataAndAddrListPsu1Part4->at(i)->first);
        dataLineEditListPsu1Part4Data.at(i)->setText(dataAndAddrListPsu1Part4->at(i)->second->first);
        addrLineEditListPsu1Part4Data.at(i)->setText(dataAndAddrListPsu1Part4->at(i)->second->second);
    }
}
// PSU1  Part4 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu1Part4VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part4VerifySetCmd->text();
    setStart = ui->lineEditPsu1Part4VerifySetStart->text();
    setEnd = ui->lineEditPsu1Part4VerifySetEnd->text();
    setJudge = ui->lineEditPsu1Part4VerifySetJudge->text();
    setMulti = ui->lineEditPsu1Part4VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part4VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu1Part4VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu1Part4VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part4VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part4VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu1Part4VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part4VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu1Part4VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part4Verify->setName(setName);  // �������õ����������
    setCmdPsu1Part4Verify->setStart(setStart);
    setCmdPsu1Part4Verify->setEnd(setEnd);
    setCmdPsu1Part4Verify->setJudge(setJudge);
    setPsu1Part4Multi = setMulti.toInt();
    dmmCmdPsu1Part4Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu1Part4Verify->setParam(dmmParam);
    dmmCmdPsu1Part4Verify->setStart(dmmStart);
    dmmCmdPsu1Part4Verify->setEnd(dmmEnd);
    dmmCmdPsu1Part4Verify->setRatio(dmmJudge.toDouble());
    dmmPsu1Part4Multi = dmmMulti.toInt();
    meterCmdPsu1Part4Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu1Part4Multi = meterMulti.toInt();
    if(psu1Part4 == NULL){
        psu1Part4 = new testItem(cmdListPsu1Part4Pre, dataAndAddrListPsu1Part4,
                                 setCmdPsu1Part4Verify, setPsu1Part4Multi,
                                 dmmCmdPsu1Part4Verify, dmmPsu1Part4Multi,
                                 meterCmdPsu1Part4Verify, meterPsu1Part4Multi,
                                 setCmdPsu1Part4Test, dmmCmdPsu1Part4Test, meterCmdPsu1Part4Test);
    }else{
        psu1Part4->setSetCmdVerify(setCmdPsu1Part4Verify);
        psu1Part4->setDmmCmdVerify(dmmCmdPsu1Part4Verify);
        psu1Part4->setMeterCmdVerify(meterCmdPsu1Part4Verify);
        psu1Part4->setSetMulti(setPsu1Part4Multi);
        psu1Part4->setDmmMulti(dmmPsu1Part4Multi);
        psu1Part4->setMeterMulti(meterPsu1Part4Multi);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart4(psu1Part4);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part4VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part4VerifySetCmd->setText(setCmdPsu1Part4Verify->getName());
    ui->lineEditPsu1Part4VerifySetStart->setText(setCmdPsu1Part4Verify->getStart());
    ui->lineEditPsu1Part4VerifySetEnd->setText(setCmdPsu1Part4Verify->getEnd());
    ui->lineEditPsu1Part4VerifySetJudge->setText(setCmdPsu1Part4Verify->getJudge());
    ui->lineEditPsu1Part4VerifySetMulti->setText(QString("%1").arg(setPsu1Part4Multi));
    // ��д��ȡ��������
    ui->lineEditPsu1Part4VerifyDMMCmd->setText(dmmCmdPsu1Part4Verify->getName());
    ui->lineEditPsu1Part4VerifyDMMStart->setText(dmmCmdPsu1Part4Verify->getStart());
    ui->lineEditPsu1Part4VerifyDMMEnd->setText(dmmCmdPsu1Part4Verify->getEnd());
    ui->lineEditPsu1Part4VerifyDMMJudge->setText(dmmCmdPsu1Part4Verify->getRatio());
    ui->lineEditPsu1Part4VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part4Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part4VerifyMeterJudge->setText(meterCmdPsu1Part4Verify->getRatio());
    ui->lineEditPsu1Part4VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part4Multi));
}
// PSU1  Part4  ����
// ���水ť
void curdataconfig::on_pushBtnPsu1Part4TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part4TestSetCmd->text();
    setStart = ui->lineEditPsu1Part4TestSetStart->text();
    setEnd = ui->lineEditPsu1Part4TestSetEnd->text();
    setJudge = ui->lineEditPsu1Part4TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part4TestDMMCmd->text();
    dmmStart = ui->lineEditPsu1Part4TestDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part4TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part4TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part4TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part4Test->setName(setName);  // �������õ����������
    setCmdPsu1Part4Test->setStart(setStart);
    setCmdPsu1Part4Test->setEnd(setEnd);
    setCmdPsu1Part4Test->setJudge(setJudge);
    dmmCmdPsu1Part4Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu1Part4Test->setStart(dmmStart);
    dmmCmdPsu1Part4Test->setEnd(dmmEnd);
    dmmCmdPsu1Part4Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu1Part4Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu1Part4 == NULL){
        psu1Part4 = new testItem(cmdListPsu1Part4Pre, dataAndAddrListPsu1Part4,
                                 setCmdPsu1Part4Verify, setPsu1Part4Multi,
                                 dmmCmdPsu1Part4Verify, dmmPsu1Part4Multi,
                                 meterCmdPsu1Part4Verify, meterPsu1Part4Multi,
                                 setCmdPsu1Part4Test, dmmCmdPsu1Part4Test, meterCmdPsu1Part4Test);
    }else{
        psu1Part4->setSetCmdTest(setCmdPsu1Part4Test);
        psu1Part4->setDmmCmdTest(dmmCmdPsu1Part4Test);
        psu1Part4->setMeterCmdTest(meterCmdPsu1Part4Test);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart4(psu1Part4);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part4TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part4TestSetCmd->setText(setCmdPsu1Part4Test->getName());
    ui->lineEditPsu1Part4TestSetStart->setText(setCmdPsu1Part4Test->getStart());
    ui->lineEditPsu1Part4TestSetEnd->setText(setCmdPsu1Part4Test->getEnd());
    ui->lineEditPsu1Part4TestSetJudge->setText(setCmdPsu1Part4Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu1Part4TestDMMCmd->setText(dmmCmdPsu1Part4Test->getName());
    ui->lineEditPsu1Part4TestDMMStart->setText(dmmCmdPsu1Part4Test->getStart());
    ui->lineEditPsu1Part4TestDMMEnd->setText(dmmCmdPsu1Part4Test->getEnd());
    ui->lineEditPsu1Part4TestDMMJudge->setText(dmmCmdPsu1Part4Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part4TestMeterJudge->setText(meterCmdPsu1Part4Test->getRatio());
}
// PSU1  Part5  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part5PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu1Part5Pre->append(newCommand);
    if(psu1Part5 == NULL){
        psu1Part5 = new testItem(cmdListPsu1Part5Pre, dataAndAddrListPsu1Part5,
                                 setCmdPsu1Part5Verify, setPsu1Part5Multi,
                                 dmmCmdPsu1Part5Verify, dmmPsu1Part5Multi,
                                 meterCmdPsu1Part5Verify, meterPsu1Part5Multi,
                                 setCmdPsu1Part5Test, dmmCmdPsu1Part5Test, meterCmdPsu1Part5Test);
    }else
        psu1Part5->setCmdList(cmdListPsu1Part5Pre);
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart5(psu1Part5);
    showPsu1Part5PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu1Part5PreCmdList()
{
    ui->psu1Part5PreCmdList->clear();
    for(int i=0; i != cmdListPsu1Part5Pre->size(); ++i){
        command *curCommand = cmdListPsu1Part5Pre->at(i);
        ui->psu1Part5PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu1Part5PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu1Part5Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu1Part5 = cmdListPsu1Part5Pre->at(nowIndexPsu1Part5Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu1Part5PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part5PreDel_clicked()
{
    if(nowIndexPsu1Part5Pre == -1){
        return ;
    }
    cmdListPsu1Part5Pre->removeAt(nowIndexPsu1Part5Pre);
    nowIndexPsu1Part5Pre = cmdListPsu1Part5Pre->size()-1;
    if(nowIndexPsu1Part5Pre != -1)
        nowCommandPsu1Part5 = cmdListPsu1Part5Pre->at(nowIndexPsu1Part5Pre);
    showPsu1Part5PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu1Part5PreName->clear();
    ui->lineEditPsu1Part5PreParam->clear();
    ui->lineEditPsu1Part5PreStart->clear();
    ui->lineEditPsu1Part5PreEnd->clear();
    ui->lineEditPsu1Part5PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu1Part5PreUp_clicked()
{
    if(nowIndexPsu1Part5Pre < 1)
        return;
    cmdListPsu1Part5Pre->swap(nowIndexPsu1Part5Pre, nowIndexPsu1Part5Pre-1);
    nowIndexPsu1Part5Pre--;
    showPsu1Part5PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu1Part5PreDown_clicked()
{
    if((nowIndexPsu1Part5Pre == -1)||(nowIndexPsu1Part5Pre >= cmdListPsu1Part5Pre->size()-1))
        return;
    cmdListPsu1Part5Pre->swap(nowIndexPsu1Part5Pre, nowIndexPsu1Part5Pre+1);
    nowIndexPsu1Part5Pre++;
    showPsu1Part5PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part5PreSave_clicked()
{
    if(ui->lineEditPsu1Part5PreName->text().size() == 0 &&
            ui->lineEditPsu1Part5PreParam->text().size() == 0 &&
            ui->lineEditPsu1Part5PreStart->text().size() == 0 &&
            ui->lineEditPsu1Part5PreEnd->text().size() == 0 &&
            ui->lineEditPsu1Part5PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu1Part5 != NULL){
        nowCommandPsu1Part5->setName(ui->lineEditPsu1Part5PreName->text());
        nowCommandPsu1Part5->setParam(ui->lineEditPsu1Part5PreParam->text());
        nowCommandPsu1Part5->setStart(ui->lineEditPsu1Part5PreStart->text());
        nowCommandPsu1Part5->setEnd(ui->lineEditPsu1Part5PreEnd->text());
        nowCommandPsu1Part5->setJudge(ui->lineEditPsu1Part5PreJudge->text());
        showPsu1Part5PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part5PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu1Part5PreName->clear();
    ui->lineEditPsu1Part5PreParam->clear();
    ui->lineEditPsu1Part5PreStart->clear();
    ui->lineEditPsu1Part5PreEnd->clear();
    ui->lineEditPsu1Part5PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu1Part5PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu1Part5PreName->setText(nowCommandPsu1Part5->getName());
    // ��ʾ�������
    ui->lineEditPsu1Part5PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu1Part5PreParam->setText(nowCommandPsu1Part5->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu1Part5PreStart->setText(nowCommandPsu1Part5->getStart());
    ui->lineEditPsu1Part5PreEnd->setText(nowCommandPsu1Part5->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu1Part5PreJudge->setText(nowCommandPsu1Part5->getJudge());
}
// PSU1  Part5  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part5DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu1Part5Data / 10;
    y = nowIndexPsu1Part5Data % 10;
    ui->scrollAreaWidgetContentsPsu1Part5->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part5Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu1Part5);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu1Part5Data_%1").arg(nowIndexPsu1Part5Data+1));
    frameListPsu1Part5Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu1Part5Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu1Part5Data_%1").arg(nowIndexPsu1Part5Data+1));
    checkBoxListPsu1Part5Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu1Part5Data_%1").arg(nowIndexPsu1Part5Data+1));
    dataLineEditListPsu1Part5Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu1Part5Data_%1").arg(nowIndexPsu1Part5Data+1));
    addrLineEditListPsu1Part5Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu1Part5Data++;    
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu1Part5()
{
    for(int i=0; i != checkBoxListPsu1Part5Data.size(); ++i){
        if(!checkBoxListPsu1Part5Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu1Part5DataAll_clicked()
{
    if(checkBoxAllSelectPsu1Part5()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu1Part5Data.size(); ++i){
            checkBoxListPsu1Part5Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu1Part5Data.size(); ++i){
            checkBoxListPsu1Part5Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu1Part5DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu1Part5Data.size();){
        if(checkBoxListPsu1Part5Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu1Part5Data.removeAt(i);
            dataLineEditListPsu1Part5Data.removeAt(i);
            addrLineEditListPsu1Part5Data.removeAt(i);
            QFrame *tempFrame = frameListPsu1Part5Data.at(i);
            frameListPsu1Part5Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu1Part5Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu1Part5Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu1Part5Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu1Part5Data = checkBoxListPsu1Part5Data.size();
    int column = nowIndexPsu1Part5Data/10 + (nowIndexPsu1Part5Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu1Part5->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu1Part5Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu1Part5DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu1Part5())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu1Part5DataAll_clicked();
    on_pushBtnPsu1Part5DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu1Part5DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu1Part5Data; ++i){
        dataLineEditListPsu1Part5Data.at(i)->clear();
        addrLineEditListPsu1Part5Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu1Part5DataBatchAdd_clicked()
{
    if(nowIndexPsu1Part5Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu1Part5DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu1Part5(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu1Part5(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu1Part5DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu1Part5Data.at(nowIndexPsu1Part5Data-1)->setText(strData);
        addrLineEditListPsu1Part5Data.at(nowIndexPsu1Part5Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu1Part5DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu1Part5Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu1Part5Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu1Part5Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu1Part5Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu1Part5->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu1Part5->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu1Part5->clear();
    dataAndAddrListPsu1Part5 = tempList;
    if(!dataAndAddrListPsu1Part5->isEmpty()){
        if(psu1Part5 == NULL){
            psu1Part5 = new testItem(cmdListPsu1Part5Pre, dataAndAddrListPsu1Part5,
                                     setCmdPsu1Part5Verify, setPsu1Part5Multi,
                                     dmmCmdPsu1Part5Verify, dmmPsu1Part5Multi,
                                     meterCmdPsu1Part5Verify, meterPsu1Part5Multi,
                                     setCmdPsu1Part5Test, dmmCmdPsu1Part5Test, meterCmdPsu1Part5Test);
        }else
            psu1Part5->setDataList(dataAndAddrListPsu1Part5);
        if(itemPsu1 == NULL)
            itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
        else
            itemPsu1->setPart5(psu1Part5);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part5DataUndo_clicked()
{
    on_pushBtnPsu1Part5DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu1Part5->size(); ++i){
        on_pushBtnPsu1Part5DataAdd_clicked();
        checkBoxListPsu1Part5Data.at(i)->setChecked(dataAndAddrListPsu1Part5->at(i)->first);
        dataLineEditListPsu1Part5Data.at(i)->setText(dataAndAddrListPsu1Part5->at(i)->second->first);
        addrLineEditListPsu1Part5Data.at(i)->setText(dataAndAddrListPsu1Part5->at(i)->second->second);
    }
}
// PSU1  Part5 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu1Part5VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part5VerifySetCmd->text();
    setStart = ui->lineEditPsu1Part5VerifySetStart->text();
    setEnd = ui->lineEditPsu1Part5VerifySetEnd->text();
    setJudge = ui->lineEditPsu1Part5VerifySetJudge->text();
    setMulti = ui->lineEditPsu1Part5VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part5VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu1Part5VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu1Part5VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part5VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part5VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu1Part5VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part5VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu1Part5VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part5Verify->setName(setName);  // �������õ����������
    setCmdPsu1Part5Verify->setStart(setStart);
    setCmdPsu1Part5Verify->setEnd(setEnd);
    setCmdPsu1Part5Verify->setJudge(setJudge);
    setPsu1Part5Multi = setMulti.toInt();
    dmmCmdPsu1Part5Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu1Part5Verify->setParam(dmmParam);
    dmmCmdPsu1Part5Verify->setStart(dmmStart);
    dmmCmdPsu1Part5Verify->setEnd(dmmEnd);
    dmmCmdPsu1Part5Verify->setRatio(dmmJudge.toDouble());
    dmmPsu1Part5Multi = dmmMulti.toInt();
    meterCmdPsu1Part5Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu1Part5Multi = meterMulti.toInt();
    if(psu1Part5 == NULL){
        psu1Part5 = new testItem(cmdListPsu1Part5Pre, dataAndAddrListPsu1Part5,
                                 setCmdPsu1Part5Verify, setPsu1Part5Multi,
                                 dmmCmdPsu1Part5Verify, dmmPsu1Part5Multi,
                                 meterCmdPsu1Part5Verify, meterPsu1Part5Multi,
                                 setCmdPsu1Part5Test, dmmCmdPsu1Part5Test, meterCmdPsu1Part5Test);
    }else{
        psu1Part5->setSetCmdVerify(setCmdPsu1Part5Verify);
        psu1Part5->setDmmCmdVerify(dmmCmdPsu1Part5Verify);
        psu1Part5->setMeterCmdVerify(meterCmdPsu1Part5Verify);
        psu1Part5->setSetMulti(setPsu1Part5Multi);
        psu1Part5->setDmmMulti(dmmPsu1Part5Multi);
        psu1Part5->setMeterMulti(meterPsu1Part5Multi);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart5(psu1Part5);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part5VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part5VerifySetCmd->setText(setCmdPsu1Part5Verify->getName());
    ui->lineEditPsu1Part5VerifySetStart->setText(setCmdPsu1Part5Verify->getStart());
    ui->lineEditPsu1Part5VerifySetEnd->setText(setCmdPsu1Part5Verify->getEnd());
    ui->lineEditPsu1Part5VerifySetJudge->setText(setCmdPsu1Part5Verify->getJudge());
    ui->lineEditPsu1Part5VerifySetMulti->setText(QString("%1").arg(setPsu1Part5Multi));
    // ��д��ȡ��������
    ui->lineEditPsu1Part5VerifyDMMCmd->setText(dmmCmdPsu1Part5Verify->getName());
    ui->lineEditPsu1Part5VerifyDMMStart->setText(dmmCmdPsu1Part5Verify->getStart());
    ui->lineEditPsu1Part5VerifyDMMEnd->setText(dmmCmdPsu1Part5Verify->getEnd());
    ui->lineEditPsu1Part5VerifyDMMJudge->setText(dmmCmdPsu1Part5Verify->getRatio());
    ui->lineEditPsu1Part5VerifyDMMMulti->setText(QString("%1").arg(dmmPsu1Part5Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part5VerifyMeterJudge->setText(meterCmdPsu1Part5Verify->getRatio());
    ui->lineEditPsu1Part5VerifyMeterMulti->setText(QString("%1").arg(meterPsu1Part5Multi));
}
// PSU1  Part5  ����
// ���水ť
void curdataconfig::on_pushBtnPsu1Part5TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu1Part5TestSetCmd->text();
    setStart = ui->lineEditPsu1Part5TestSetStart->text();
    setEnd = ui->lineEditPsu1Part5TestSetEnd->text();
    setJudge = ui->lineEditPsu1Part5TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu1Part5TestDMMCmd->text();
    dmmStart = ui->lineEditPsu1Part5TestDMMStart->text();
    dmmEnd = ui->lineEditPsu1Part5TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu1Part5TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu1Part5TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu1Part5Test->setName(setName);  // �������õ����������
    setCmdPsu1Part5Test->setStart(setStart);
    setCmdPsu1Part5Test->setEnd(setEnd);
    setCmdPsu1Part5Test->setJudge(setJudge);
    dmmCmdPsu1Part5Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu1Part5Test->setStart(dmmStart);
    dmmCmdPsu1Part5Test->setEnd(dmmEnd);
    dmmCmdPsu1Part5Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu1Part5Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu1Part5 == NULL){
        psu1Part5 = new testItem(cmdListPsu1Part5Pre, dataAndAddrListPsu1Part5,
                                 setCmdPsu1Part5Verify, setPsu1Part5Multi,
                                 dmmCmdPsu1Part5Verify, dmmPsu1Part5Multi,
                                 meterCmdPsu1Part5Verify, meterPsu1Part5Multi,
                                 setCmdPsu1Part5Test, dmmCmdPsu1Part5Test, meterCmdPsu1Part5Test);
    }else{
        psu1Part5->setSetCmdTest(setCmdPsu1Part5Test);
        psu1Part5->setDmmCmdTest(dmmCmdPsu1Part5Test);
        psu1Part5->setMeterCmdTest(meterCmdPsu1Part5Test);
    }
    if(itemPsu1 == NULL)
        itemPsu1 = new currentItem(cmdListPsu1Pre, psu1Part1, psu1Part2, psu1Part3, psu1Part4, psu1Part5);
    else
        itemPsu1->setPart5(psu1Part5);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu1Part5TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu1Part5TestSetCmd->setText(setCmdPsu1Part5Test->getName());
    ui->lineEditPsu1Part5TestSetStart->setText(setCmdPsu1Part5Test->getStart());
    ui->lineEditPsu1Part5TestSetEnd->setText(setCmdPsu1Part5Test->getEnd());
    ui->lineEditPsu1Part5TestSetJudge->setText(setCmdPsu1Part5Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu1Part5TestDMMCmd->setText(dmmCmdPsu1Part5Test->getName());
    ui->lineEditPsu1Part5TestDMMStart->setText(dmmCmdPsu1Part5Test->getStart());
    ui->lineEditPsu1Part5TestDMMEnd->setText(dmmCmdPsu1Part5Test->getEnd());
    ui->lineEditPsu1Part5TestDMMJudge->setText(dmmCmdPsu1Part5Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu1Part5TestMeterJudge->setText(meterCmdPsu1Part5Test->getRatio());
}
// PSU2  ǰ������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu2Pre->append(newCommand);
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPreCmdList(cmdListPsu2Pre);
    showPsu2PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu2PreCmdList()
{
    ui->psu2PreCmdList->clear();
    for(int i=0; i != cmdListPsu2Pre->size(); ++i){
        command *curCommand = cmdListPsu2Pre->at(i);
        ui->psu2PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu2PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu2Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu2 = cmdListPsu2Pre->at(nowIndexPsu2Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu2PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2PreDel_clicked()
{
    if(nowIndexPsu2Pre == -1){
        return ;
    }
    cmdListPsu2Pre->removeAt(nowIndexPsu2Pre);
    nowIndexPsu2Pre = cmdListPsu2Pre->size()-1;
    if(nowIndexPsu2Pre != -1)
        nowCommandPsu2 = cmdListPsu2Pre->at(nowIndexPsu2Pre);
    showPsu2PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu2PreCmdName->clear();
    ui->lineEditPsu2PreParam->clear();
    ui->lineEditPsu2PreStart->clear();
    ui->lineEditPsu2PreEnd->clear();
    ui->lineEditPsu2PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu2PreUp_clicked()
{
    if(nowIndexPsu2Pre < 1)
        return;
    cmdListPsu2Pre->swap(nowIndexPsu2Pre, nowIndexPsu2Pre-1);
    nowIndexPsu2Pre--;
    showPsu2PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu2PreDown_clicked()
{
    if((nowIndexPsu2Pre == -1)||(nowIndexPsu2Pre >= cmdListPsu2Pre->size()-1))
        return;
    cmdListPsu2Pre->swap(nowIndexPsu2Pre, nowIndexPsu2Pre+1);
    nowIndexPsu2Pre++;
    showPsu2PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu2PreSave_clicked()
{
    if(ui->lineEditPsu2PreCmdName->text().size() == 0 &&
            ui->lineEditPsu2PreParam->text().size() == 0 &&
            ui->lineEditPsu2PreStart->text().size() == 0 &&
            ui->lineEditPsu2PreEnd->text().size() == 0 &&
            ui->lineEditPsu2PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu2 != NULL){
        nowCommandPsu2->setName(ui->lineEditPsu2PreCmdName->text());
        nowCommandPsu2->setParam(ui->lineEditPsu2PreParam->text());
        nowCommandPsu2->setStart(ui->lineEditPsu2PreStart->text());
        nowCommandPsu2->setEnd(ui->lineEditPsu2PreEnd->text());
        nowCommandPsu2->setJudge(ui->lineEditPsu2PreJudge->text());
        showPsu2PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu2PreCmdName->clear();
    ui->lineEditPsu2PreParam->clear();
    ui->lineEditPsu2PreStart->clear();
    ui->lineEditPsu2PreEnd->clear();
    ui->lineEditPsu2PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu2PreCmdName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu2PreCmdName->setText(nowCommandPsu2->getName());
    // ��ʾ�������
    ui->lineEditPsu2PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu2PreParam->setText(nowCommandPsu2->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu2PreStart->setText(nowCommandPsu2->getStart());
    ui->lineEditPsu2PreEnd->setText(nowCommandPsu2->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu2PreJudge->setText(nowCommandPsu2->getJudge());
}
// PSU2  Part1  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part1PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu2Part1Pre->append(newCommand);
    if(psu2Part1 == NULL){
        psu2Part1 = new testItem(cmdListPsu2Part1Pre, dataAndAddrListPsu2Part1,
                                 setCmdPsu2Part1Verify, setPsu2Part1Multi,
                                 dmmCmdPsu2Part1Verify, dmmPsu2Part1Multi,
                                 meterCmdPsu2Part1Verify, meterPsu2Part1Multi,
                                 setCmdPsu2Part1Test, dmmCmdPsu2Part1Test, meterCmdPsu2Part1Test);
    }else
        psu2Part1->setCmdList(cmdListPsu2Part1Pre);
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart1(psu2Part1);
    showPsu2Part1PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu2Part1PreCmdList()
{
    ui->psu2Part1PreCmdList->clear();
    for(int i=0; i != cmdListPsu2Part1Pre->size(); ++i){
        command *curCommand = cmdListPsu2Part1Pre->at(i);
        ui->psu2Part1PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu2Part1PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu2Part1Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu2Part1 = cmdListPsu2Part1Pre->at(nowIndexPsu2Part1Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu2Part1PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part1PreDel_clicked()
{
    if(nowIndexPsu2Part1Pre == -1){
        return ;
    }
    cmdListPsu2Part1Pre->removeAt(nowIndexPsu2Part1Pre);
    nowIndexPsu2Part1Pre = cmdListPsu2Part1Pre->size()-1;
    if(nowIndexPsu2Part1Pre != -1)
        nowCommandPsu2Part1 = cmdListPsu2Part1Pre->at(nowIndexPsu2Part1Pre);
    showPsu2Part1PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu2Part1PreName->clear();
    ui->lineEditPsu2Part1PreParam->clear();
    ui->lineEditPsu2Part1PreStart->clear();
    ui->lineEditPsu2Part1PreEnd->clear();
    ui->lineEditPsu2Part1PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu2Part1PreUp_clicked()
{
    if(nowIndexPsu2Part1Pre < 1)
        return;
    cmdListPsu2Part1Pre->swap(nowIndexPsu2Part1Pre, nowIndexPsu2Part1Pre-1);
    nowIndexPsu2Part1Pre--;
    showPsu2Part1PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu2Part1PreDown_clicked()
{
    if((nowIndexPsu2Part1Pre == -1)||(nowIndexPsu2Part1Pre >= cmdListPsu2Part1Pre->size()-1))
        return;
    cmdListPsu2Part1Pre->swap(nowIndexPsu2Part1Pre, nowIndexPsu2Part1Pre+1);
    nowIndexPsu2Part1Pre++;
    showPsu2Part1PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part1PreSave_clicked()
{
    if(ui->lineEditPsu2Part1PreName->text().size() == 0 &&
            ui->lineEditPsu2Part1PreParam->text().size() == 0 &&
            ui->lineEditPsu2Part1PreStart->text().size() == 0 &&
            ui->lineEditPsu2Part1PreEnd->text().size() == 0 &&
            ui->lineEditPsu2Part1PreJudge->text().size() == 0)
        return;
    if (nowCommandPsu2Part1 != NULL){
        nowCommandPsu2Part1->setName(ui->lineEditPsu2Part1PreName->text());
        nowCommandPsu2Part1->setParam(ui->lineEditPsu2Part1PreParam->text());
        nowCommandPsu2Part1->setStart(ui->lineEditPsu2Part1PreStart->text());
        nowCommandPsu2Part1->setEnd(ui->lineEditPsu2Part1PreEnd->text());
        nowCommandPsu2Part1->setJudge(ui->lineEditPsu2Part1PreJudge->text());
        showPsu2Part1PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part1PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu2Part1PreName->clear();
    ui->lineEditPsu2Part1PreParam->clear();
    ui->lineEditPsu2Part1PreStart->clear();
    ui->lineEditPsu2Part1PreEnd->clear();
    ui->lineEditPsu2Part1PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu2Part1PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu2Part1PreName->setText(nowCommandPsu2Part1->getName());
    // ��ʾ�������
    ui->lineEditPsu2Part1PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu2Part1PreParam->setText(nowCommandPsu2Part1->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu2Part1PreStart->setText(nowCommandPsu2Part1->getStart());
    ui->lineEditPsu2Part1PreEnd->setText(nowCommandPsu2Part1->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu2Part1PreJudge->setText(nowCommandPsu2Part1->getJudge());
}
// PSU2  Part1  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part1DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu2Part1Data / 10;
    y = nowIndexPsu2Part1Data % 10;
    ui->scrollAreaWidgetContentsPsu2Part1->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part1Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu2Part1);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu2Part1Data_%1").arg(nowIndexPsu2Part1Data+1));
    frameListPsu2Part1Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu2Part1Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu2Part1Data_%1").arg(nowIndexPsu2Part1Data+1));
    checkBoxListPsu2Part1Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu2Part1Data_%1").arg(nowIndexPsu2Part1Data+1));
    dataLineEditListPsu2Part1Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu2Part1Data_%1").arg(nowIndexPsu2Part1Data+1));
    addrLineEditListPsu2Part1Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu2Part1Data++;    
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu2Part1()
{
    for(int i=0; i != checkBoxListPsu2Part1Data.size(); ++i){
        if(!checkBoxListPsu2Part1Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu2Part1DataAll_clicked()
{
    if(checkBoxAllSelectPsu2Part1()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu2Part1Data.size(); ++i){
            checkBoxListPsu2Part1Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu2Part1Data.size(); ++i){
            checkBoxListPsu2Part1Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part1DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu2Part1Data.size();){
        if(checkBoxListPsu2Part1Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu2Part1Data.removeAt(i);
            dataLineEditListPsu2Part1Data.removeAt(i);
            addrLineEditListPsu2Part1Data.removeAt(i);
            QFrame *tempFrame = frameListPsu2Part1Data.at(i);
            frameListPsu2Part1Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu2Part1Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu2Part1Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu2Part1Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu2Part1Data = checkBoxListPsu2Part1Data.size();
    int column = nowIndexPsu2Part1Data/10 + (nowIndexPsu2Part1Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu2Part1->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part1Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu2Part1DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu2Part1())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu2Part1DataAll_clicked();
    on_pushBtnPsu2Part1DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu2Part1DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu2Part1Data; ++i){
        dataLineEditListPsu2Part1Data.at(i)->clear();
        addrLineEditListPsu2Part1Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part1DataBatchAdd_clicked()
{
    if(nowIndexPsu2Part1Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu2Part1DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu2Part1(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu2Part1(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu2Part1DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu2Part1Data.at(nowIndexPsu2Part1Data-1)->setText(strData);
        addrLineEditListPsu2Part1Data.at(nowIndexPsu2Part1Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part1DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu2Part1Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu2Part1Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu2Part1Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu2Part1Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu2Part1->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu2Part1->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu2Part1->clear();
    dataAndAddrListPsu2Part1 = tempList;
    if(!dataAndAddrListPsu2Part1->isEmpty()){
        if(psu2Part1 == NULL){
            psu2Part1 = new testItem(cmdListPsu2Part1Pre, dataAndAddrListPsu2Part1,
                                     setCmdPsu2Part1Verify, setPsu2Part1Multi,
                                     dmmCmdPsu2Part1Verify, dmmPsu2Part1Multi,
                                     meterCmdPsu2Part1Verify, meterPsu2Part1Multi,
                                     setCmdPsu2Part1Test, dmmCmdPsu2Part1Test, meterCmdPsu2Part1Test);
        }else
            psu2Part1->setDataList(dataAndAddrListPsu2Part1);
        if(itemPsu2 == NULL)
            itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
        else
            itemPsu2->setPart1(psu2Part1);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part1DataUndo_clicked()
{
    on_pushBtnPsu2Part1DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu2Part1->size(); ++i){
        on_pushBtnPsu2Part1DataAdd_clicked();
        checkBoxListPsu2Part1Data.at(i)->setChecked(dataAndAddrListPsu2Part1->at(i)->first);
        dataLineEditListPsu2Part1Data.at(i)->setText(dataAndAddrListPsu2Part1->at(i)->second->first);
        addrLineEditListPsu2Part1Data.at(i)->setText(dataAndAddrListPsu2Part1->at(i)->second->second);
    }
}
// PSU2  Part1 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu2Part1VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part1VerifySetCmd->text();
    setStart = ui->lineEditPsu2Part1VerifySetStart->text();
    setEnd = ui->lineEditPsu2Part1VerifySetEnd->text();
    setJudge = ui->lineEditPsu2Part1VerifySetJudge->text();
    setMulti = ui->lineEditPsu2Part1VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part1VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu2Part1VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu2Part1VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part1VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part1VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu2Part1VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part1VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu2Part1VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part1Verify->setName(setName);  // �������õ����������
    setCmdPsu2Part1Verify->setStart(setStart);
    setCmdPsu2Part1Verify->setEnd(setEnd);
    setCmdPsu2Part1Verify->setJudge(setJudge);
    setPsu2Part1Multi = setMulti.toInt();
    dmmCmdPsu2Part1Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu2Part1Verify->setParam(dmmParam);
    dmmCmdPsu2Part1Verify->setStart(dmmStart);
    dmmCmdPsu2Part1Verify->setEnd(dmmEnd);
    dmmCmdPsu2Part1Verify->setRatio(dmmJudge.toDouble());
    dmmPsu2Part1Multi = dmmMulti.toInt();
    meterCmdPsu2Part1Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu2Part1Multi = meterMulti.toInt();
    if(psu2Part1 == NULL){
        psu2Part1 = new testItem(cmdListPsu2Part1Pre, dataAndAddrListPsu2Part1,
                                 setCmdPsu2Part1Verify, setPsu2Part1Multi,
                                 dmmCmdPsu2Part1Verify, dmmPsu2Part1Multi,
                                 meterCmdPsu2Part1Verify, meterPsu2Part1Multi,
                                 setCmdPsu2Part1Test, dmmCmdPsu2Part1Test, meterCmdPsu2Part1Test);
    }else{
        psu2Part1->setSetCmdVerify(setCmdPsu2Part1Verify);
        psu2Part1->setDmmCmdVerify(dmmCmdPsu2Part1Verify);
        psu2Part1->setMeterCmdVerify(meterCmdPsu2Part1Verify);
        psu2Part1->setSetMulti(setPsu2Part1Multi);
        psu2Part1->setDmmMulti(dmmPsu2Part1Multi);
        psu2Part1->setMeterMulti(meterPsu2Part1Multi);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart1(psu2Part1);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part1VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part1VerifySetCmd->setText(setCmdPsu2Part1Verify->getName());
    ui->lineEditPsu2Part1VerifySetStart->setText(setCmdPsu2Part1Verify->getStart());
    ui->lineEditPsu2Part1VerifySetEnd->setText(setCmdPsu2Part1Verify->getEnd());
    ui->lineEditPsu2Part1VerifySetJudge->setText(setCmdPsu2Part1Verify->getJudge());
    ui->lineEditPsu2Part1VerifySetMulti->setText(QString("%1").arg(setPsu2Part1Multi));
    // ��д��ȡ��������
    ui->lineEditPsu2Part1VerifyDMMCmd->setText(dmmCmdPsu2Part1Verify->getName());
    ui->lineEditPsu2Part1VerifyDMMStart->setText(dmmCmdPsu2Part1Verify->getStart());
    ui->lineEditPsu2Part1VerifyDMMEnd->setText(dmmCmdPsu2Part1Verify->getEnd());
    ui->lineEditPsu2Part1VerifyDMMJudge->setText(dmmCmdPsu2Part1Verify->getRatio());
    ui->lineEditPsu2Part1VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part1Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part1VerifyMeterJudge->setText(meterCmdPsu2Part1Verify->getRatio());
    ui->lineEditPsu2Part1VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part1Multi));
}
// PSU2  Part1  ����
// ���水ť
void curdataconfig::on_pushBtnPsu2Part1TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part1TestSetCmd->text();
    setStart = ui->lineEditPsu2Part1TestSetStart->text();
    setEnd = ui->lineEditPsu2Part1TestSetEnd->text();
    setJudge = ui->lineEditPsu2Part1TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part1TestDMMCmd->text();
    dmmStart = ui->lineEditPsu2Part1TestDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part1TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part1TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part1TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part1Test->setName(setName);  // �������õ����������
    setCmdPsu2Part1Test->setStart(setStart);
    setCmdPsu2Part1Test->setEnd(setEnd);
    setCmdPsu2Part1Test->setJudge(setJudge);
    dmmCmdPsu2Part1Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu2Part1Test->setStart(dmmStart);
    dmmCmdPsu2Part1Test->setEnd(dmmEnd);
    dmmCmdPsu2Part1Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu2Part1Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu2Part1 == NULL){
        psu2Part1 = new testItem(cmdListPsu2Part1Pre, dataAndAddrListPsu2Part1,
                                 setCmdPsu2Part1Verify, setPsu2Part1Multi,
                                 dmmCmdPsu2Part1Verify, dmmPsu2Part1Multi,
                                 meterCmdPsu2Part1Verify, meterPsu2Part1Multi,
                                 setCmdPsu2Part1Test, dmmCmdPsu2Part1Test, meterCmdPsu2Part1Test);
    }else{
        psu2Part1->setSetCmdTest(setCmdPsu2Part1Test);
        psu2Part1->setDmmCmdTest(dmmCmdPsu2Part1Test);
        psu2Part1->setMeterCmdTest(meterCmdPsu2Part1Test);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart1(psu2Part1);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part1TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part1TestSetCmd->setText(setCmdPsu2Part1Test->getName());
    ui->lineEditPsu2Part1TestSetStart->setText(setCmdPsu2Part1Test->getStart());
    ui->lineEditPsu2Part1TestSetEnd->setText(setCmdPsu2Part1Test->getEnd());
    ui->lineEditPsu2Part1TestSetJudge->setText(setCmdPsu2Part1Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu2Part1TestDMMCmd->setText(dmmCmdPsu2Part1Test->getName());
    ui->lineEditPsu2Part1TestDMMStart->setText(dmmCmdPsu2Part1Test->getStart());
    ui->lineEditPsu2Part1TestDMMEnd->setText(dmmCmdPsu2Part1Test->getEnd());
    ui->lineEditPsu2Part1TestDMMJudge->setText(dmmCmdPsu2Part1Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part1TestMeterJudge->setText(meterCmdPsu2Part1Test->getRatio());
}
// PSU2  Part2  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part2PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu2Part2Pre->append(newCommand);
    if(psu2Part2 == NULL){
        psu2Part2 = new testItem(cmdListPsu2Part2Pre, dataAndAddrListPsu2Part2,
                                 setCmdPsu2Part2Verify, setPsu2Part2Multi,
                                 dmmCmdPsu2Part2Verify, dmmPsu2Part2Multi,
                                 meterCmdPsu2Part2Verify, meterPsu2Part2Multi,
                                 setCmdPsu2Part2Test, dmmCmdPsu2Part2Test, meterCmdPsu2Part2Test);
    }else
        psu2Part2->setCmdList(cmdListPsu2Part2Pre);
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart2(psu2Part2);
    showPsu2Part2PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu2Part2PreCmdList()
{
    ui->psu2Part2PreCmdList->clear();
    for(int i=0; i != cmdListPsu2Part2Pre->size(); ++i){
        command *curCommand = cmdListPsu2Part2Pre->at(i);
        ui->psu2Part2PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu2Part2PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu2Part2Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu2Part2 = cmdListPsu2Part2Pre->at(nowIndexPsu2Part2Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu2Part2PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part2PreDel_clicked()
{
    if(nowIndexPsu2Part2Pre == -1){
        return ;
    }
    cmdListPsu2Part2Pre->removeAt(nowIndexPsu2Part2Pre);
    nowIndexPsu2Part2Pre = cmdListPsu2Part2Pre->size()-1;
    if(nowIndexPsu2Part2Pre != -1)
        nowCommandPsu2Part2 = cmdListPsu2Part2Pre->at(nowIndexPsu2Part2Pre);
    showPsu2Part2PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu2Part2PreName->clear();
    ui->lineEditPsu2Part2PreParam->clear();
    ui->lineEditPsu2Part2PreStart->clear();
    ui->lineEditPsu2Part2PreEnd->clear();
    ui->lineEditPsu2Part2PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu2Part2PreUp_clicked()
{
    if(nowIndexPsu2Part2Pre < 1)
        return;
    cmdListPsu2Part2Pre->swap(nowIndexPsu2Part2Pre, nowIndexPsu2Part2Pre-1);
    nowIndexPsu2Part2Pre--;
    showPsu2Part2PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu2Part2PreDown_clicked()
{
    if((nowIndexPsu2Part2Pre == -1)||(nowIndexPsu2Part2Pre >= cmdListPsu2Part2Pre->size()-1))
        return;
    cmdListPsu2Part2Pre->swap(nowIndexPsu2Part2Pre, nowIndexPsu2Part2Pre+1);
    nowIndexPsu2Part2Pre++;
    showPsu2Part2PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part2PreSave_clicked()
{
    if(ui->lineEditPsu2Part2PreName->text().size() == 0 &&
            ui->lineEditPsu2Part2PreParam->text().size() == 0 &&
            ui->lineEditPsu2Part2PreStart->text().size() == 0 &&
            ui->lineEditPsu2Part2PreEnd->text().size() == 0 &&
            ui->lineEditPsu2Part2PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu2Part2 != NULL){
        nowCommandPsu2Part2->setName(ui->lineEditPsu2Part2PreName->text());
        nowCommandPsu2Part2->setParam(ui->lineEditPsu2Part2PreParam->text());
        nowCommandPsu2Part2->setStart(ui->lineEditPsu2Part2PreStart->text());
        nowCommandPsu2Part2->setEnd(ui->lineEditPsu2Part2PreEnd->text());
        nowCommandPsu2Part2->setJudge(ui->lineEditPsu2Part2PreJudge->text());
        showPsu2Part2PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part2PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu2Part2PreName->clear();
    ui->lineEditPsu2Part2PreParam->clear();
    ui->lineEditPsu2Part2PreStart->clear();
    ui->lineEditPsu2Part2PreEnd->clear();
    ui->lineEditPsu2Part2PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu2Part2PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu2Part2PreName->setText(nowCommandPsu2Part2->getName());
    // ��ʾ�������
    ui->lineEditPsu2Part2PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu2Part2PreParam->setText(nowCommandPsu2Part2->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu2Part2PreStart->setText(nowCommandPsu2Part2->getStart());
    ui->lineEditPsu2Part2PreEnd->setText(nowCommandPsu2Part2->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu2Part2PreJudge->setText(nowCommandPsu2Part2->getJudge());
}
// PSU2  Part2  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part2DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu2Part2Data / 10;
    y = nowIndexPsu2Part2Data % 10;
    ui->scrollAreaWidgetContentsPsu2Part2->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part2Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu2Part2);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu2Part2Data_%1").arg(nowIndexPsu2Part2Data+1));
    frameListPsu2Part2Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu2Part2Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu2Part2Data_%1").arg(nowIndexPsu2Part2Data+1));
    checkBoxListPsu2Part2Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu2Part2Data_%1").arg(nowIndexPsu2Part2Data+1));
    dataLineEditListPsu2Part2Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu2Part2Data_%1").arg(nowIndexPsu2Part2Data+1));
    addrLineEditListPsu2Part2Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu2Part2Data++;    
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu2Part2()
{
    for(int i=0; i != checkBoxListPsu2Part2Data.size(); ++i){
        if(!checkBoxListPsu2Part2Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu2Part2DataAll_clicked()
{
    if(checkBoxAllSelectPsu2Part2()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu2Part2Data.size(); ++i){
            checkBoxListPsu2Part2Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu2Part2Data.size(); ++i){
            checkBoxListPsu2Part2Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part2DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu2Part2Data.size();){
        if(checkBoxListPsu2Part2Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu2Part2Data.removeAt(i);
            dataLineEditListPsu2Part2Data.removeAt(i);
            addrLineEditListPsu2Part2Data.removeAt(i);
            QFrame *tempFrame = frameListPsu2Part2Data.at(i);
            frameListPsu2Part2Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu2Part2Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu2Part2Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu2Part2Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu2Part2Data = checkBoxListPsu2Part2Data.size();
    int column = nowIndexPsu2Part2Data/10 + (nowIndexPsu2Part2Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu2Part2->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part2Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu2Part2DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu2Part2())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu2Part2DataAll_clicked();
    on_pushBtnPsu2Part2DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu2Part2DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu2Part2Data; ++i){
        dataLineEditListPsu2Part2Data.at(i)->clear();
        addrLineEditListPsu2Part2Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part2DataBatchAdd_clicked()
{
    if(nowIndexPsu2Part2Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu2Part2DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu2Part2(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu2Part2(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu2Part2DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu2Part2Data.at(nowIndexPsu2Part2Data-1)->setText(strData);
        addrLineEditListPsu2Part2Data.at(nowIndexPsu2Part2Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part2DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu2Part2Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu2Part2Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu2Part2Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu2Part2Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu2Part2->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu2Part2->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu2Part2->clear();
    dataAndAddrListPsu2Part2 = tempList;
    if(!dataAndAddrListPsu2Part2->isEmpty()){
        if(psu2Part2 == NULL){
            psu2Part2 = new testItem(cmdListPsu2Part2Pre, dataAndAddrListPsu2Part2,
                                     setCmdPsu2Part2Verify, setPsu2Part2Multi,
                                     dmmCmdPsu2Part2Verify, dmmPsu2Part2Multi,
                                     meterCmdPsu2Part2Verify, meterPsu2Part2Multi,
                                     setCmdPsu2Part2Test, dmmCmdPsu2Part2Test, meterCmdPsu2Part2Test);
        }else
            psu2Part2->setDataList(dataAndAddrListPsu2Part2);
        if(itemPsu2 == NULL)
            itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
        else
            itemPsu2->setPart2(psu2Part2);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part2DataUndo_clicked()
{
    on_pushBtnPsu2Part2DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu2Part2->size(); ++i){
        on_pushBtnPsu2Part2DataAdd_clicked();
        checkBoxListPsu2Part2Data.at(i)->setChecked(dataAndAddrListPsu2Part2->at(i)->first);
        dataLineEditListPsu2Part2Data.at(i)->setText(dataAndAddrListPsu2Part2->at(i)->second->first);
        addrLineEditListPsu2Part2Data.at(i)->setText(dataAndAddrListPsu2Part2->at(i)->second->second);
    }
}
// PSU2  Part2 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu2Part2VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part2VerifySetCmd->text();
    setStart = ui->lineEditPsu2Part2VerifySetStart->text();
    setEnd = ui->lineEditPsu2Part2VerifySetEnd->text();
    setJudge = ui->lineEditPsu2Part2VerifySetJudge->text();
    setMulti = ui->lineEditPsu2Part2VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part2VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu2Part2VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu2Part2VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part2VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part2VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu2Part2VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part2VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu2Part2VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part2Verify->setName(setName);  // �������õ����������
    setCmdPsu2Part2Verify->setStart(setStart);
    setCmdPsu2Part2Verify->setEnd(setEnd);
    setCmdPsu2Part2Verify->setJudge(setJudge);
    setPsu2Part2Multi = setMulti.toInt();
    dmmCmdPsu2Part2Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu2Part2Verify->setParam(dmmParam);
    dmmCmdPsu2Part2Verify->setStart(dmmStart);
    dmmCmdPsu2Part2Verify->setEnd(dmmEnd);
    dmmCmdPsu2Part2Verify->setRatio(dmmJudge.toDouble());
    dmmPsu2Part2Multi = dmmMulti.toInt();
    meterCmdPsu2Part2Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu2Part2Multi = meterMulti.toInt();
    if(psu2Part2 == NULL){
        psu2Part2 = new testItem(cmdListPsu2Part2Pre, dataAndAddrListPsu2Part2,
                                 setCmdPsu2Part2Verify, setPsu2Part2Multi,
                                 dmmCmdPsu2Part2Verify, dmmPsu2Part2Multi,
                                 meterCmdPsu2Part2Verify, meterPsu2Part2Multi,
                                 setCmdPsu2Part2Test, dmmCmdPsu2Part2Test, meterCmdPsu2Part2Test);
    }else{
        psu2Part2->setSetCmdVerify(setCmdPsu2Part2Verify);
        psu2Part2->setDmmCmdVerify(dmmCmdPsu2Part2Verify);
        psu2Part2->setMeterCmdVerify(meterCmdPsu2Part2Verify);
        psu2Part2->setSetMulti(setPsu2Part2Multi);
        psu2Part2->setDmmMulti(dmmPsu2Part2Multi);
        psu2Part2->setMeterMulti(meterPsu2Part2Multi);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart2(psu2Part2);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part2VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part2VerifySetCmd->setText(setCmdPsu2Part2Verify->getName());
    ui->lineEditPsu2Part2VerifySetStart->setText(setCmdPsu2Part2Verify->getStart());
    ui->lineEditPsu2Part2VerifySetEnd->setText(setCmdPsu2Part2Verify->getEnd());
    ui->lineEditPsu2Part2VerifySetJudge->setText(setCmdPsu2Part2Verify->getJudge());
    ui->lineEditPsu2Part2VerifySetMulti->setText(QString("%1").arg(setPsu2Part2Multi));
    // ��д��ȡ��������
    ui->lineEditPsu2Part2VerifyDMMCmd->setText(dmmCmdPsu2Part2Verify->getName());
    ui->lineEditPsu2Part2VerifyDMMStart->setText(dmmCmdPsu2Part2Verify->getStart());
    ui->lineEditPsu2Part2VerifyDMMEnd->setText(dmmCmdPsu2Part2Verify->getEnd());
    ui->lineEditPsu2Part2VerifyDMMJudge->setText(dmmCmdPsu2Part2Verify->getRatio());
    ui->lineEditPsu2Part2VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part2Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part2VerifyMeterJudge->setText(meterCmdPsu2Part2Verify->getRatio());
    ui->lineEditPsu2Part2VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part2Multi));
}
// PSU2  Part2  ����
// ���水ť
void curdataconfig::on_pushBtnPsu2Part2TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part2TestSetCmd->text();
    setStart = ui->lineEditPsu2Part2TestSetStart->text();
    setEnd = ui->lineEditPsu2Part2TestSetEnd->text();
    setJudge = ui->lineEditPsu2Part2TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part2TestDMMCmd->text();
    dmmStart = ui->lineEditPsu2Part2TestDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part2TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part2TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part2TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part2Test->setName(setName);  // �������õ����������
    setCmdPsu2Part2Test->setStart(setStart);
    setCmdPsu2Part2Test->setEnd(setEnd);
    setCmdPsu2Part2Test->setJudge(setJudge);
    dmmCmdPsu2Part2Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu2Part2Test->setStart(dmmStart);
    dmmCmdPsu2Part2Test->setEnd(dmmEnd);
    dmmCmdPsu2Part2Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu2Part2Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu2Part2 == NULL){
        psu2Part2 = new testItem(cmdListPsu2Part2Pre, dataAndAddrListPsu2Part2,
                                 setCmdPsu2Part2Verify, setPsu2Part2Multi,
                                 dmmCmdPsu2Part2Verify, dmmPsu2Part2Multi,
                                 meterCmdPsu2Part2Verify, meterPsu2Part2Multi,
                                 setCmdPsu2Part2Test, dmmCmdPsu2Part2Test, meterCmdPsu2Part2Test);
    }else{
        psu2Part2->setSetCmdTest(setCmdPsu2Part2Test);
        psu2Part2->setDmmCmdTest(dmmCmdPsu2Part2Test);
        psu2Part2->setMeterCmdTest(meterCmdPsu2Part2Test);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart2(psu2Part2);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part2TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part2TestSetCmd->setText(setCmdPsu2Part2Test->getName());
    ui->lineEditPsu2Part2TestSetStart->setText(setCmdPsu2Part2Test->getStart());
    ui->lineEditPsu2Part2TestSetEnd->setText(setCmdPsu2Part2Test->getEnd());
    ui->lineEditPsu2Part2TestSetJudge->setText(setCmdPsu2Part2Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu2Part2TestDMMCmd->setText(dmmCmdPsu2Part2Test->getName());
    ui->lineEditPsu2Part2TestDMMStart->setText(dmmCmdPsu2Part2Test->getStart());
    ui->lineEditPsu2Part2TestDMMEnd->setText(dmmCmdPsu2Part2Test->getEnd());
    ui->lineEditPsu2Part2TestDMMJudge->setText(dmmCmdPsu2Part2Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part2TestMeterJudge->setText(meterCmdPsu2Part2Test->getRatio());
}
// PSU2  Part3  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part3PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu2Part3Pre->append(newCommand);
    if(psu2Part3 == NULL){
        psu2Part3 = new testItem(cmdListPsu2Part3Pre, dataAndAddrListPsu2Part3,
                                 setCmdPsu2Part3Verify, setPsu2Part3Multi,
                                 dmmCmdPsu2Part3Verify, dmmPsu2Part3Multi,
                                 meterCmdPsu2Part3Verify, meterPsu2Part3Multi,
                                 setCmdPsu2Part3Test, dmmCmdPsu2Part3Test, meterCmdPsu2Part3Test);
    }else
        psu2Part3->setCmdList(cmdListPsu2Part3Pre);
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart3(psu2Part3);
    showPsu2Part3PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu2Part3PreCmdList()
{
    ui->psu2Part3PreCmdList->clear();
    for(int i=0; i != cmdListPsu2Part3Pre->size(); ++i){
        command *curCommand = cmdListPsu2Part3Pre->at(i);
        ui->psu2Part3PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu2Part3PreCmdList_clicked(const QModelIndex &index)
{
    nowIndexPsu2Part3Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu2Part3 = cmdListPsu2Part3Pre->at(nowIndexPsu2Part3Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu2Part3PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part3PreDel_clicked()
{
    if(nowIndexPsu2Part3Pre == -1){
        return ;
    }
    cmdListPsu2Part3Pre->removeAt(nowIndexPsu2Part3Pre);
    nowIndexPsu2Part3Pre = cmdListPsu2Part3Pre->size()-1;
    if(nowIndexPsu2Part3Pre != -1)
        nowCommandPsu2Part3 = cmdListPsu2Part3Pre->at(nowIndexPsu2Part3Pre);
    showPsu2Part3PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu2Part3PreName->clear();
    ui->lineEditPsu2Part3PreParam->clear();
    ui->lineEditPsu2Part3PreStart->clear();
    ui->lineEditPsu2Part3PreEnd->clear();
    ui->lineEditPsu2Part3PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu2Part3PreUp_clicked()
{
    if(nowIndexPsu2Part3Pre < 1)
        return;
    cmdListPsu2Part3Pre->swap(nowIndexPsu2Part3Pre, nowIndexPsu2Part3Pre-1);
    nowIndexPsu2Part3Pre--;
    showPsu2Part3PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu2Part3PreDown_clicked()
{
    if((nowIndexPsu2Part3Pre == -1)||(nowIndexPsu2Part3Pre >= cmdListPsu2Part3Pre->size()-1))
        return;
    cmdListPsu2Part3Pre->swap(nowIndexPsu2Part3Pre, nowIndexPsu2Part3Pre+1);
    nowIndexPsu2Part3Pre++;
    showPsu2Part3PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part3PreSave_clicked()
{
    if(ui->lineEditPsu2Part3PreName->text().size() == 0 &&
            ui->lineEditPsu2Part3PreParam->text().size() == 0 &&
            ui->lineEditPsu2Part3PreStart->text().size() == 0 &&
            ui->lineEditPsu2Part3PreEnd->text().size() == 0 &&
            ui->lineEditPsu2Part3PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu2Part3 != NULL){
        nowCommandPsu2Part3->setName(ui->lineEditPsu2Part3PreName->text());
        nowCommandPsu2Part3->setParam(ui->lineEditPsu2Part3PreParam->text());
        nowCommandPsu2Part3->setStart(ui->lineEditPsu2Part3PreStart->text());
        nowCommandPsu2Part3->setEnd(ui->lineEditPsu2Part3PreEnd->text());
        nowCommandPsu2Part3->setJudge(ui->lineEditPsu2Part3PreJudge->text());
        showPsu2Part3PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part3PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu2Part3PreName->clear();
    ui->lineEditPsu2Part3PreParam->clear();
    ui->lineEditPsu2Part3PreStart->clear();
    ui->lineEditPsu2Part3PreEnd->clear();
    ui->lineEditPsu2Part3PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu2Part3PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu2Part3PreName->setText(nowCommandPsu2Part3->getName());
    // ��ʾ�������
    ui->lineEditPsu2Part3PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu2Part3PreParam->setText(nowCommandPsu2Part3->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu2Part3PreStart->setText(nowCommandPsu2Part3->getStart());
    ui->lineEditPsu2Part3PreEnd->setText(nowCommandPsu2Part3->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu2Part3PreJudge->setText(nowCommandPsu2Part3->getJudge());
}
// PSU2  Part3  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part3DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu2Part3Data / 10;
    y = nowIndexPsu2Part3Data % 10;
    ui->scrollAreaWidgetContentsPsu2Part3->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part3Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu2Part3);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu2Part3Data_%1").arg(nowIndexPsu2Part3Data+1));
    frameListPsu2Part3Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu2Part3Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu2Part3Data_%1").arg(nowIndexPsu2Part3Data+1));
    checkBoxListPsu2Part3Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu2Part3Data_%1").arg(nowIndexPsu2Part3Data+1));
    dataLineEditListPsu2Part3Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu2Part3Data_%1").arg(nowIndexPsu2Part3Data+1));
    addrLineEditListPsu2Part3Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu2Part3Data++;
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu2Part3()
{
    for(int i=0; i != checkBoxListPsu2Part3Data.size(); ++i){
        if(!checkBoxListPsu2Part3Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu2Part3DataAll_clicked()
{
    if(checkBoxAllSelectPsu2Part3()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu2Part3Data.size(); ++i){
            checkBoxListPsu2Part3Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu2Part3Data.size(); ++i){
            checkBoxListPsu2Part3Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part3DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu2Part3Data.size();){
        if(checkBoxListPsu2Part3Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu2Part3Data.removeAt(i);
            dataLineEditListPsu2Part3Data.removeAt(i);
            addrLineEditListPsu2Part3Data.removeAt(i);
            QFrame *tempFrame = frameListPsu2Part3Data.at(i);
            frameListPsu2Part3Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu2Part3Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu2Part3Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu2Part3Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu2Part3Data = checkBoxListPsu2Part3Data.size();
    int column = nowIndexPsu2Part3Data/10 + (nowIndexPsu2Part3Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu2Part3->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part3Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu2Part3DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu2Part3())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu2Part3DataAll_clicked();
    on_pushBtnPsu2Part3DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu2Part3DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu2Part3Data; ++i){
        dataLineEditListPsu2Part3Data.at(i)->clear();
        addrLineEditListPsu2Part3Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part3DataBatchAdd_clicked()
{
    if(nowIndexPsu2Part3Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu2Part3DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu2Part3(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu2Part3(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu2Part3DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu2Part3Data.at(nowIndexPsu2Part3Data-1)->setText(strData);
        addrLineEditListPsu2Part3Data.at(nowIndexPsu2Part3Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part3DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu2Part3Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu2Part3Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu2Part3Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu2Part3Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu2Part3->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu2Part3->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu2Part3->clear();
    dataAndAddrListPsu2Part3 = tempList;
    if(!dataAndAddrListPsu2Part3->isEmpty()){
        if(psu2Part3 == NULL){
            psu2Part3 = new testItem(cmdListPsu2Part3Pre, dataAndAddrListPsu2Part3,
                                     setCmdPsu2Part3Verify, setPsu2Part3Multi,
                                     dmmCmdPsu2Part3Verify, dmmPsu2Part3Multi,
                                     meterCmdPsu2Part3Verify, meterPsu2Part3Multi,
                                     setCmdPsu2Part3Test, dmmCmdPsu2Part3Test, meterCmdPsu2Part3Test);
        }else
            psu2Part3->setDataList(dataAndAddrListPsu2Part3);
        if(itemPsu2 == NULL)
            itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
        else
            itemPsu2->setPart3(psu2Part3);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part3DataUndo_clicked()
{
    on_pushBtnPsu2Part3DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu2Part3->size(); ++i){
        on_pushBtnPsu2Part3DataAdd_clicked();
        checkBoxListPsu2Part3Data.at(i)->setChecked(dataAndAddrListPsu2Part3->at(i)->first);
        dataLineEditListPsu2Part3Data.at(i)->setText(dataAndAddrListPsu2Part3->at(i)->second->first);
        addrLineEditListPsu2Part3Data.at(i)->setText(dataAndAddrListPsu2Part3->at(i)->second->second);
    }
}
// PSU2  Part3 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu2Part3VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part3VerifySetCmd->text();
    setStart = ui->lineEditPsu2Part3VerifySetStart->text();
    setEnd = ui->lineEditPsu2Part3VerifySetEnd->text();
    setJudge = ui->lineEditPsu2Part3VerifySetJudge->text();
    setMulti = ui->lineEditPsu2Part3VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part3VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu2Part3VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu2Part3VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part3VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part3VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu2Part3VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part3VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu2Part3VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part3Verify->setName(setName);  // �������õ����������
    setCmdPsu2Part3Verify->setStart(setStart);
    setCmdPsu2Part3Verify->setEnd(setEnd);
    setCmdPsu2Part3Verify->setJudge(setJudge);
    setPsu2Part3Multi = setMulti.toInt();
    dmmCmdPsu2Part3Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu2Part3Verify->setParam(dmmParam);
    dmmCmdPsu2Part3Verify->setStart(dmmStart);
    dmmCmdPsu2Part3Verify->setEnd(dmmEnd);
    dmmCmdPsu2Part3Verify->setRatio(dmmJudge.toDouble());
    dmmPsu2Part3Multi = dmmMulti.toInt();
    meterCmdPsu2Part3Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu2Part3Multi = meterMulti.toInt();
    if(psu2Part3 == NULL){
        psu2Part3 = new testItem(cmdListPsu2Part3Pre, dataAndAddrListPsu2Part3,
                                 setCmdPsu2Part3Verify, setPsu2Part3Multi,
                                 dmmCmdPsu2Part3Verify, dmmPsu2Part3Multi,
                                 meterCmdPsu2Part3Verify, meterPsu2Part3Multi,
                                 setCmdPsu2Part3Test, dmmCmdPsu2Part3Test, meterCmdPsu2Part3Test);
    }else{
        psu2Part3->setSetCmdVerify(setCmdPsu2Part3Verify);
        psu2Part3->setDmmCmdVerify(dmmCmdPsu2Part3Verify);
        psu2Part3->setMeterCmdVerify(meterCmdPsu2Part3Verify);
        psu2Part3->setSetMulti(setPsu2Part3Multi);
        psu2Part3->setDmmMulti(dmmPsu2Part3Multi);
        psu2Part3->setMeterMulti(meterPsu2Part3Multi);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart3(psu2Part3);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part3VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part3VerifySetCmd->setText(setCmdPsu2Part3Verify->getName());
    ui->lineEditPsu2Part3VerifySetStart->setText(setCmdPsu2Part3Verify->getStart());
    ui->lineEditPsu2Part3VerifySetEnd->setText(setCmdPsu2Part3Verify->getEnd());
    ui->lineEditPsu2Part3VerifySetJudge->setText(setCmdPsu2Part3Verify->getJudge());
    ui->lineEditPsu2Part3VerifySetMulti->setText(QString("%1").arg(setPsu2Part3Multi));
    // ��д��ȡ��������
    ui->lineEditPsu2Part3VerifyDMMCmd->setText(dmmCmdPsu2Part3Verify->getName());
    ui->lineEditPsu2Part3VerifyDMMStart->setText(dmmCmdPsu2Part3Verify->getStart());
    ui->lineEditPsu2Part3VerifyDMMEnd->setText(dmmCmdPsu2Part3Verify->getEnd());
    ui->lineEditPsu2Part3VerifyDMMJudge->setText(dmmCmdPsu2Part3Verify->getRatio());
    ui->lineEditPsu2Part3VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part3Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part3VerifyMeterJudge->setText(meterCmdPsu2Part3Verify->getRatio());
    ui->lineEditPsu2Part3VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part3Multi));
}
// PSU2  Part3  ����
// ���水ť
void curdataconfig::on_pushBtnPsu2Part3TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part3TestSetCmd->text();
    setStart = ui->lineEditPsu2Part3TestSetStart->text();
    setEnd = ui->lineEditPsu2Part3TestSetEnd->text();
    setJudge = ui->lineEditPsu2Part3TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part3TestDMMCmd->text();
    dmmStart = ui->lineEditPsu2Part3TestDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part3TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part3TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part3TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part3Test->setName(setName);  // �������õ����������
    setCmdPsu2Part3Test->setStart(setStart);
    setCmdPsu2Part3Test->setEnd(setEnd);
    setCmdPsu2Part3Test->setJudge(setJudge);
    dmmCmdPsu2Part3Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu2Part3Test->setStart(dmmStart);
    dmmCmdPsu2Part3Test->setEnd(dmmEnd);
    dmmCmdPsu2Part3Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu2Part3Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu2Part3 == NULL){
        psu2Part3 = new testItem(cmdListPsu2Part3Pre, dataAndAddrListPsu2Part3,
                                 setCmdPsu2Part3Verify, setPsu2Part3Multi,
                                 dmmCmdPsu2Part3Verify, dmmPsu2Part3Multi,
                                 meterCmdPsu2Part3Verify, meterPsu2Part3Multi,
                                 setCmdPsu2Part3Test, dmmCmdPsu2Part3Test, meterCmdPsu2Part3Test);
    }else{
        psu2Part3->setSetCmdTest(setCmdPsu2Part3Test);
        psu2Part3->setDmmCmdTest(dmmCmdPsu2Part3Test);
        psu2Part3->setMeterCmdTest(meterCmdPsu2Part3Test);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart3(psu2Part3);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part3TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part3TestSetCmd->setText(setCmdPsu2Part3Test->getName());
    ui->lineEditPsu2Part3TestSetStart->setText(setCmdPsu2Part3Test->getStart());
    ui->lineEditPsu2Part3TestSetEnd->setText(setCmdPsu2Part3Test->getEnd());
    ui->lineEditPsu2Part3TestSetJudge->setText(setCmdPsu2Part3Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu2Part3TestDMMCmd->setText(dmmCmdPsu2Part3Test->getName());
    ui->lineEditPsu2Part3TestDMMStart->setText(dmmCmdPsu2Part3Test->getStart());
    ui->lineEditPsu2Part3TestDMMEnd->setText(dmmCmdPsu2Part3Test->getEnd());
    ui->lineEditPsu2Part3TestDMMJudge->setText(dmmCmdPsu2Part3Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part3TestMeterJudge->setText(meterCmdPsu2Part3Test->getRatio());
}
// PSU2  Part4  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part4PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu2Part4Pre->append(newCommand);
    if(psu2Part4 == NULL){
        psu2Part4 = new testItem(cmdListPsu2Part4Pre, dataAndAddrListPsu2Part4,
                                 setCmdPsu2Part4Verify, setPsu2Part4Multi,
                                 dmmCmdPsu2Part4Verify, dmmPsu2Part4Multi,
                                 meterCmdPsu2Part4Verify, meterPsu2Part4Multi,
                                 setCmdPsu2Part4Test, dmmCmdPsu2Part4Test, meterCmdPsu2Part4Test);
    }else
        psu2Part4->setCmdList(cmdListPsu2Part4Pre);
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart4(psu2Part4);
    showPsu2Part4PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu2Part4PreCmdList()
{
    ui->psu2Part4PreCmdList->clear();
    for(int i=0; i != cmdListPsu2Part4Pre->size(); ++i){
        command *curCommand = cmdListPsu2Part4Pre->at(i);
        ui->psu2Part4PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu2Part4PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu2Part4Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu2Part4 = cmdListPsu2Part4Pre->at(nowIndexPsu2Part4Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu2Part4PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part4PreDel_clicked()
{
    if(nowIndexPsu2Part4Pre == -1){
        return ;
    }
    cmdListPsu2Part4Pre->removeAt(nowIndexPsu2Part4Pre);
    nowIndexPsu2Part4Pre = cmdListPsu2Part4Pre->size()-1;
    if(nowIndexPsu2Part4Pre != -1)
        nowCommandPsu2Part4 = cmdListPsu2Part4Pre->at(nowIndexPsu2Part4Pre);
    showPsu2Part4PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu2Part4PreName->clear();
    ui->lineEditPsu2Part4PreParam->clear();
    ui->lineEditPsu2Part4PreStart->clear();
    ui->lineEditPsu2Part4PreEnd->clear();
    ui->lineEditPsu2Part4PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu2Part4PreUp_clicked()
{
    if(nowIndexPsu2Part4Pre < 1)
        return;
    cmdListPsu2Part4Pre->swap(nowIndexPsu2Part4Pre, nowIndexPsu2Part4Pre-1);
    nowIndexPsu2Part4Pre--;
    showPsu2Part4PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu2Part4PreDown_clicked()
{
    if((nowIndexPsu2Part4Pre == -1)||(nowIndexPsu2Part4Pre >= cmdListPsu2Part4Pre->size()-1))
        return;
    cmdListPsu2Part4Pre->swap(nowIndexPsu2Part4Pre, nowIndexPsu2Part4Pre+1);
    nowIndexPsu2Part4Pre++;
    showPsu2Part4PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part4PreSave_clicked()
{
    if(ui->lineEditPsu2Part4PreName->text().size() == 0 &&
            ui->lineEditPsu2Part4PreParam->text().size() == 0 &&
            ui->lineEditPsu2Part4PreStart->text().size() == 0 &&
            ui->lineEditPsu2Part4PreEnd->text().size() == 0 &&
            ui->lineEditPsu2Part4PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu2Part4 != NULL){
        nowCommandPsu2Part4->setName(ui->lineEditPsu2Part4PreName->text());
        nowCommandPsu2Part4->setParam(ui->lineEditPsu2Part4PreParam->text());
        nowCommandPsu2Part4->setStart(ui->lineEditPsu2Part4PreStart->text());
        nowCommandPsu2Part4->setEnd(ui->lineEditPsu2Part4PreEnd->text());
        nowCommandPsu2Part4->setJudge(ui->lineEditPsu2Part4PreJudge->text());
        showPsu2Part4PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part4PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu2Part4PreName->clear();
    ui->lineEditPsu2Part4PreParam->clear();
    ui->lineEditPsu2Part4PreStart->clear();
    ui->lineEditPsu2Part4PreEnd->clear();
    ui->lineEditPsu2Part4PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu2Part4PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu2Part4PreName->setText(nowCommandPsu2Part4->getName());
    // ��ʾ�������
    ui->lineEditPsu2Part4PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu2Part4PreParam->setText(nowCommandPsu2Part4->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu2Part4PreStart->setText(nowCommandPsu2Part4->getStart());
    ui->lineEditPsu2Part4PreEnd->setText(nowCommandPsu2Part4->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu2Part4PreJudge->setText(nowCommandPsu2Part4->getJudge());
}
// PSU2  Part4  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part4DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu2Part4Data / 10;
    y = nowIndexPsu2Part4Data % 10;
    ui->scrollAreaWidgetContentsPsu2Part4->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part4Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu2Part4);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu2Part4Data_%1").arg(nowIndexPsu2Part4Data+1));
    frameListPsu2Part4Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu2Part4Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu2Part4Data_%1").arg(nowIndexPsu2Part4Data+1));
    checkBoxListPsu2Part4Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu2Part4Data_%1").arg(nowIndexPsu2Part4Data+1));
    dataLineEditListPsu2Part4Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu2Part4Data_%1").arg(nowIndexPsu2Part4Data+1));
    addrLineEditListPsu2Part4Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu2Part4Data++;
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu2Part4()
{
    for(int i=0; i != checkBoxListPsu2Part4Data.size(); ++i){
        if(!checkBoxListPsu2Part4Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu2Part4DataAll_clicked()
{
    if(checkBoxAllSelectPsu2Part4()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu2Part4Data.size(); ++i){
            checkBoxListPsu2Part4Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu2Part4Data.size(); ++i){
            checkBoxListPsu2Part4Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part4DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu2Part4Data.size();){
        if(checkBoxListPsu2Part4Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu2Part4Data.removeAt(i);
            dataLineEditListPsu2Part4Data.removeAt(i);
            addrLineEditListPsu2Part4Data.removeAt(i);
            QFrame *tempFrame = frameListPsu2Part4Data.at(i);
            frameListPsu2Part4Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu2Part4Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu2Part4Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu2Part4Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu2Part4Data = checkBoxListPsu2Part4Data.size();
    int column = nowIndexPsu2Part4Data/10 + (nowIndexPsu2Part4Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu2Part4->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part4Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu2Part4DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu2Part4())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu2Part4DataAll_clicked();
    on_pushBtnPsu2Part4DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu2Part4DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu2Part4Data; ++i){
        dataLineEditListPsu2Part4Data.at(i)->clear();
        addrLineEditListPsu2Part4Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part4DataBatchAdd_clicked()
{
    if(nowIndexPsu2Part4Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu2Part4DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu2Part4(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu2Part4(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu2Part4DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu2Part4Data.at(nowIndexPsu2Part4Data-1)->setText(strData);
        addrLineEditListPsu2Part4Data.at(nowIndexPsu2Part4Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part4DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu2Part4Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu2Part4Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu2Part4Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu2Part4Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu2Part4->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu2Part4->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu2Part4->clear();
    dataAndAddrListPsu2Part4 = tempList;
    if(!dataAndAddrListPsu2Part4->isEmpty()){
        if(psu2Part4 == NULL){
            psu2Part4 = new testItem(cmdListPsu2Part4Pre, dataAndAddrListPsu2Part4,
                                     setCmdPsu2Part4Verify, setPsu2Part4Multi,
                                     dmmCmdPsu2Part4Verify, dmmPsu2Part4Multi,
                                     meterCmdPsu2Part4Verify, meterPsu2Part4Multi,
                                     setCmdPsu2Part4Test, dmmCmdPsu2Part4Test, meterCmdPsu2Part4Test);
        }else
            psu2Part4->setDataList(dataAndAddrListPsu2Part4);
        if(itemPsu2 == NULL)
            itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
        else
            itemPsu2->setPart4(psu2Part4);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part4DataUndo_clicked()
{
    on_pushBtnPsu2Part4DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu2Part4->size(); ++i){
        on_pushBtnPsu2Part4DataAdd_clicked();
        checkBoxListPsu2Part4Data.at(i)->setChecked(dataAndAddrListPsu2Part4->at(i)->first);
        dataLineEditListPsu2Part4Data.at(i)->setText(dataAndAddrListPsu2Part4->at(i)->second->first);
        addrLineEditListPsu2Part4Data.at(i)->setText(dataAndAddrListPsu2Part4->at(i)->second->second);
    }
}
// PSU2  Part4 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu2Part4VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part4VerifySetCmd->text();
    setStart = ui->lineEditPsu2Part4VerifySetStart->text();
    setEnd = ui->lineEditPsu2Part4VerifySetEnd->text();
    setJudge = ui->lineEditPsu2Part4VerifySetJudge->text();
    setMulti = ui->lineEditPsu2Part4VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part4VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu2Part4VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu2Part4VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part4VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part4VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu2Part4VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part4VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu2Part4VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part4Verify->setName(setName);  // �������õ����������
    setCmdPsu2Part4Verify->setStart(setStart);
    setCmdPsu2Part4Verify->setEnd(setEnd);
    setCmdPsu2Part4Verify->setJudge(setJudge);
    setPsu2Part4Multi = setMulti.toInt();
    dmmCmdPsu2Part4Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu2Part4Verify->setParam(dmmParam);
    dmmCmdPsu2Part4Verify->setStart(dmmStart);
    dmmCmdPsu2Part4Verify->setEnd(dmmEnd);
    dmmCmdPsu2Part4Verify->setRatio(dmmJudge.toDouble());
    dmmPsu2Part4Multi = dmmMulti.toInt();
    meterCmdPsu2Part4Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu2Part4Multi = meterMulti.toInt();
    if(psu2Part4 == NULL){
        psu2Part4 = new testItem(cmdListPsu2Part4Pre, dataAndAddrListPsu2Part4,
                                 setCmdPsu2Part4Verify, setPsu2Part4Multi,
                                 dmmCmdPsu2Part4Verify, dmmPsu2Part4Multi,
                                 meterCmdPsu2Part4Verify, meterPsu2Part4Multi,
                                 setCmdPsu2Part4Test, dmmCmdPsu2Part4Test, meterCmdPsu2Part4Test);
    }else{
        psu2Part4->setSetCmdVerify(setCmdPsu2Part4Verify);
        psu2Part4->setDmmCmdVerify(dmmCmdPsu2Part4Verify);
        psu2Part4->setMeterCmdVerify(meterCmdPsu2Part4Verify);
        psu2Part4->setSetMulti(setPsu2Part4Multi);
        psu2Part4->setDmmMulti(dmmPsu2Part4Multi);
        psu2Part4->setMeterMulti(meterPsu2Part4Multi);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart4(psu2Part4);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part4VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part4VerifySetCmd->setText(setCmdPsu2Part4Verify->getName());
    ui->lineEditPsu2Part4VerifySetStart->setText(setCmdPsu2Part4Verify->getStart());
    ui->lineEditPsu2Part4VerifySetEnd->setText(setCmdPsu2Part4Verify->getEnd());
    ui->lineEditPsu2Part4VerifySetJudge->setText(setCmdPsu2Part4Verify->getJudge());
    ui->lineEditPsu2Part4VerifySetMulti->setText(QString("%1").arg(setPsu2Part4Multi));
    // ��д��ȡ��������
    ui->lineEditPsu2Part4VerifyDMMCmd->setText(dmmCmdPsu2Part4Verify->getName());
    ui->lineEditPsu2Part4VerifyDMMStart->setText(dmmCmdPsu2Part4Verify->getStart());
    ui->lineEditPsu2Part4VerifyDMMEnd->setText(dmmCmdPsu2Part4Verify->getEnd());
    ui->lineEditPsu2Part4VerifyDMMJudge->setText(dmmCmdPsu2Part4Verify->getRatio());
    ui->lineEditPsu2Part4VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part4Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part4VerifyMeterJudge->setText(meterCmdPsu2Part4Verify->getRatio());
    ui->lineEditPsu2Part4VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part4Multi));
}
// PSU2  Part4  ����
// ���水ť
void curdataconfig::on_pushBtnPsu2Part4TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part4TestSetCmd->text();
    setStart = ui->lineEditPsu2Part4TestSetStart->text();
    setEnd = ui->lineEditPsu2Part4TestSetEnd->text();
    setJudge = ui->lineEditPsu2Part4TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part4TestDMMCmd->text();
    dmmStart = ui->lineEditPsu2Part4TestDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part4TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part4TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part4TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part4Test->setName(setName);  // �������õ����������
    setCmdPsu2Part4Test->setStart(setStart);
    setCmdPsu2Part4Test->setEnd(setEnd);
    setCmdPsu2Part4Test->setJudge(setJudge);
    dmmCmdPsu2Part4Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu2Part4Test->setStart(dmmStart);
    dmmCmdPsu2Part4Test->setEnd(dmmEnd);
    dmmCmdPsu2Part4Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu2Part4Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu2Part4 == NULL){
        psu2Part4 = new testItem(cmdListPsu2Part4Pre, dataAndAddrListPsu2Part4,
                                 setCmdPsu2Part4Verify, setPsu2Part4Multi,
                                 dmmCmdPsu2Part4Verify, dmmPsu2Part4Multi,
                                 meterCmdPsu2Part4Verify, meterPsu2Part4Multi,
                                 setCmdPsu2Part4Test, dmmCmdPsu2Part4Test, meterCmdPsu2Part4Test);
    }else{
        psu2Part4->setSetCmdTest(setCmdPsu2Part4Test);
        psu2Part4->setDmmCmdTest(dmmCmdPsu2Part4Test);
        psu2Part4->setMeterCmdTest(meterCmdPsu2Part4Test);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart4(psu2Part4);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part4TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part4TestSetCmd->setText(setCmdPsu2Part4Test->getName());
    ui->lineEditPsu2Part4TestSetStart->setText(setCmdPsu2Part4Test->getStart());
    ui->lineEditPsu2Part4TestSetEnd->setText(setCmdPsu2Part4Test->getEnd());
    ui->lineEditPsu2Part4TestSetJudge->setText(setCmdPsu2Part4Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu2Part4TestDMMCmd->setText(dmmCmdPsu2Part4Test->getName());
    ui->lineEditPsu2Part4TestDMMStart->setText(dmmCmdPsu2Part4Test->getStart());
    ui->lineEditPsu2Part4TestDMMEnd->setText(dmmCmdPsu2Part4Test->getEnd());
    ui->lineEditPsu2Part4TestDMMJudge->setText(dmmCmdPsu2Part4Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part4TestMeterJudge->setText(meterCmdPsu2Part4Test->getRatio());
}
// PSU2  Part5  ��������
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part5PreAdd_clicked()
{
    command *newCommand = new command(QString("untitled"));
    cmdListPsu2Part5Pre->append(newCommand);
    if(psu2Part5 == NULL){
        psu2Part5 = new testItem(cmdListPsu2Part5Pre, dataAndAddrListPsu2Part5,
                                 setCmdPsu2Part5Verify, setPsu2Part5Multi,
                                 dmmCmdPsu2Part5Verify, dmmPsu2Part5Multi,
                                 meterCmdPsu2Part5Verify, meterPsu2Part5Multi,
                                 setCmdPsu2Part5Test, dmmCmdPsu2Part5Test, meterCmdPsu2Part5Test);
    }else
        psu2Part5->setCmdList(cmdListPsu2Part5Pre);
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart5(psu2Part5);
    showPsu2Part5PreCmdList();
}
// ˢ�������
void curdataconfig::showPsu2Part5PreCmdList()
{
    ui->psu2Part5PreCmdList->clear();
    for(int i=0; i != cmdListPsu2Part5Pre->size(); ++i){
        command *curCommand = cmdListPsu2Part5Pre->at(i);
        ui->psu2Part5PreCmdList->addItem(curCommand->getFullName());
    }
}
// ��������¼�
void curdataconfig::on_psu2Part5PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index.data().toString();
    nowIndexPsu2Part5Pre = index.row();  // ����ch1ǰ������ҳ��ǰ�������б��е�����
    nowCommandPsu2Part5 = cmdListPsu2Part5Pre->at(nowIndexPsu2Part5Pre);  // ����ch1ǰ������ҳ��ǰ�����ָ��
    on_pushBtnPsu2Part5PreUndo_clicked();
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part5PreDel_clicked()
{
    if(nowIndexPsu2Part5Pre == -1){
        return ;
    }
    cmdListPsu2Part5Pre->removeAt(nowIndexPsu2Part5Pre);
    nowIndexPsu2Part5Pre = cmdListPsu2Part5Pre->size()-1;
    if(nowIndexPsu2Part5Pre != -1)
        nowCommandPsu2Part5 = cmdListPsu2Part5Pre->at(nowIndexPsu2Part5Pre);
    showPsu2Part5PreCmdList();
    // ��յ�ǰ��������
    ui->lineEditPsu2Part5PreName->clear();
    ui->lineEditPsu2Part5PreParam->clear();
    ui->lineEditPsu2Part5PreStart->clear();
    ui->lineEditPsu2Part5PreEnd->clear();
    ui->lineEditPsu2Part5PreJudge->clear();
}
// ���ϰ�ť
void curdataconfig::on_pushBtnPsu2Part5PreUp_clicked()
{
    if(nowIndexPsu2Part5Pre < 1)
        return;
    cmdListPsu2Part5Pre->swap(nowIndexPsu2Part5Pre, nowIndexPsu2Part5Pre-1);
    nowIndexPsu2Part5Pre--;
    showPsu2Part5PreCmdList();
}
// ���°�ť
void curdataconfig::on_pushBtnPsu2Part5PreDown_clicked()
{
    if((nowIndexPsu2Part5Pre == -1)||(nowIndexPsu2Part5Pre >= cmdListPsu2Part5Pre->size()-1))
        return;
    cmdListPsu2Part5Pre->swap(nowIndexPsu2Part5Pre, nowIndexPsu2Part5Pre+1);
    nowIndexPsu2Part5Pre++;
    showPsu2Part5PreCmdList();
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part5PreSave_clicked()
{
    if(ui->lineEditPsu2Part5PreName->text().size() == 0 &&
            ui->lineEditPsu2Part5PreParam->text().size() == 0 &&
            ui->lineEditPsu2Part5PreStart->text().size() == 0 &&
            ui->lineEditPsu2Part5PreEnd->text().size() == 0 &&
            ui->lineEditPsu2Part5PreJudge->text().size() == 0)
        return;
    if(nowCommandPsu2Part5 != NULL){
        nowCommandPsu2Part5->setName(ui->lineEditPsu2Part5PreName->text());
        nowCommandPsu2Part5->setParam(ui->lineEditPsu2Part5PreParam->text());
        nowCommandPsu2Part5->setStart(ui->lineEditPsu2Part5PreStart->text());
        nowCommandPsu2Part5->setEnd(ui->lineEditPsu2Part5PreEnd->text());
        nowCommandPsu2Part5->setJudge(ui->lineEditPsu2Part5PreJudge->text());
        showPsu2Part5PreCmdList();
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part5PreUndo_clicked()
{
    // ��յ�ǰ��������
    ui->lineEditPsu2Part5PreName->clear();
    ui->lineEditPsu2Part5PreParam->clear();
    ui->lineEditPsu2Part5PreStart->clear();
    ui->lineEditPsu2Part5PreEnd->clear();
    ui->lineEditPsu2Part5PreJudge->clear();
    // ��ʾ��������
    ui->lineEditPsu2Part5PreName->setPlaceholderText(tr("��������"));
    ui->lineEditPsu2Part5PreName->setText(nowCommandPsu2Part5->getName());
    // ��ʾ�������
    ui->lineEditPsu2Part5PreParam->setPlaceholderText(tr("����"));
    ui->lineEditPsu2Part5PreParam->setText(nowCommandPsu2Part5->getParam());

    // ��ʾ��ȡ����
    ui->lineEditPsu2Part5PreStart->setText(nowCommandPsu2Part5->getStart());
    ui->lineEditPsu2Part5PreEnd->setText(nowCommandPsu2Part5->getEnd());
    // ��ʾ�еȲ���
    ui->lineEditPsu2Part5PreJudge->setText(nowCommandPsu2Part5->getJudge());
}
// PSU2  Part5  ����
// ��Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part5DataAdd_clicked()
{
    int x, y;
    x = nowIndexPsu2Part5Data / 10;
    y = nowIndexPsu2Part5Data % 10;
    ui->scrollAreaWidgetContentsPsu2Part5->setFixedWidth((x+1)*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part5Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
    // ��
    QFrame* newframe = new QFrame(ui->scrollAreaWidgetContentsPsu2Part5);
    newframe->setGeometry(QRect(x*180, y*30, 170, 21));
    newframe->setFrameShape(QFrame::Box);
    newframe->setFrameShadow(QFrame::Raised);
    newframe->setObjectName(QString("framePsu2Part5Data_%1").arg(nowIndexPsu2Part5Data+1));
    frameListPsu2Part5Data.append(newframe);
    newframe->show();
    // ��ѡ��
    QCheckBox * newcheckbox = new QCheckBox(newframe);
    newcheckbox->setGeometry(QRect(5, 3, 50, 16));
    newcheckbox->setChecked(true);
    newcheckbox->setText(QString("%1").arg(nowIndexPsu2Part5Data+1));
    newcheckbox->setObjectName(QString("checkBoxPsu2Part5Data_%1").arg(nowIndexPsu2Part5Data+1));
    checkBoxListPsu2Part5Data.append(newcheckbox);
    newcheckbox->show();
    // ���ݿ�
    QLineEdit * datalineedit = new QLineEdit(newframe);
    datalineedit->setGeometry(QRect(60, 2, 51, 17));
    datalineedit->setPlaceholderText(tr("����"));
    datalineedit->setValidator(new QDoubleValidator(0.0, 65535.0, 2, this));
    datalineedit->setObjectName(QString("lineEditPsu2Part5Data_%1").arg(nowIndexPsu2Part5Data+1));
    dataLineEditListPsu2Part5Data.append(datalineedit);
    datalineedit->show();
    // ��ַ��
    QLineEdit * addrlineedit = new QLineEdit(newframe);
    addrlineedit->setGeometry(QRect(117, 2, 51, 17));
    addrlineedit->setPlaceholderText(tr("��ַ"));
    addrlineedit->setObjectName(QString("lineEditPsu2Part5Data_%1").arg(nowIndexPsu2Part5Data+1));
    addrLineEditListPsu2Part5Data.append(addrlineedit);
    addrlineedit->show();
    nowIndexPsu2Part5Data++;
}
// �ж�ȫѡ״̬
bool curdataconfig::checkBoxAllSelectPsu2Part5()
{
    for(int i=0; i != checkBoxListPsu2Part5Data.size(); ++i){
        if(!checkBoxListPsu2Part5Data.at(i)->isChecked())
            return false;
    }
    return true;
}
// ȫѡ��ť
void curdataconfig::on_pushBtnPsu2Part5DataAll_clicked()
{
    if(checkBoxAllSelectPsu2Part5()){  // �Ѿ�ȫѡ, ����Ϊȫ��ѡ
        for(int i=0; i != checkBoxListPsu2Part5Data.size(); ++i){
            checkBoxListPsu2Part5Data.at(i)->setChecked(false);
        }
    }else { // δȫѡ,����Ϊȫѡ
        for(int i=0; i != checkBoxListPsu2Part5Data.size(); ++i){
            checkBoxListPsu2Part5Data.at(i)->setChecked(true);
        }
    }
}
// ɾ����ť
void curdataconfig::on_pushBtnPsu2Part5DataDel_clicked()
{
    for(int i=0; i != checkBoxListPsu2Part5Data.size();){
        if(checkBoxListPsu2Part5Data.at(i)->isChecked()){  // �����ѡ��,��ɾ����������
            checkBoxListPsu2Part5Data.removeAt(i);
            dataLineEditListPsu2Part5Data.removeAt(i);
            addrLineEditListPsu2Part5Data.removeAt(i);
            QFrame *tempFrame = frameListPsu2Part5Data.at(i);
            frameListPsu2Part5Data.removeAt(i);
            delete tempFrame;
        }else{
            ++i;
        }
    }
    // �������п�
    int x, y;
    for(int i=0; i != frameListPsu2Part5Data.size(); ++i){
        x = i / 10;
        y = i % 10;
        frameListPsu2Part5Data.at(i)->setGeometry(QRect(x*180, y*30+2, 170, 21));
        checkBoxListPsu2Part5Data.at(i)->setText(QString("%1").arg(i+1));
    }
    nowIndexPsu2Part5Data = checkBoxListPsu2Part5Data.size();
    int column = nowIndexPsu2Part5Data/10 + (nowIndexPsu2Part5Data % 10 ? 1 : 0);
    ui->scrollAreaWidgetContentsPsu2Part5->setFixedWidth(column*180);  // ���ù��������С
    QScrollBar *pScrollBar = ui->scrollAreaPsu2Part5Data->horizontalScrollBar();
    if (pScrollBar != NULL)
    {
        int nMax = pScrollBar->maximum();
        pScrollBar->setValue(nMax);  // �Զ����������ұ�
    }
}
// ȫ��ɾ����ť
void curdataconfig::on_pushBtnPsu2Part5DataBatchDel_clicked()
{
    if(!checkBoxAllSelectPsu2Part5())  // û��ȫѡ,��ȫѡ
        on_pushBtnPsu2Part5DataAll_clicked();
    on_pushBtnPsu2Part5DataDel_clicked(); // ɾ��
}
// ������ݰ�ť
void curdataconfig::on_pushBtnPsu2Part5DataClear_clicked()
{
    for(int i=0; i != nowIndexPsu2Part5Data; ++i){
        dataLineEditListPsu2Part5Data.at(i)->clear();
        addrLineEditListPsu2Part5Data.at(i)->clear();
    }
}
// ������Ӱ�ť
void curdataconfig::on_pushBtnPsu2Part5DataBatchAdd_clicked()
{
    if(nowIndexPsu2Part5Data)
        if(QMessageBox::question(this, tr("ѯ��"), tr("�Ƿ�����ǰ�������ݣ�"), QMessageBox::Ok|QMessageBox::No) == QMessageBox::No)
            on_pushBtnPsu2Part5DataBatchDel_clicked();
    BatchAdd * batchdialog = new BatchAdd();
    connect(batchdialog, SIGNAL(returnParams(int, double, double, QString, int, bool)),
            this, SLOT(handleBatchParamsPsu2Part5(int, double, double, QString, int, bool)));
    batchdialog->show();
}
// ������յ���������Ӳ���
void curdataconfig::handleBatchParamsPsu2Part5(int num, double dataStart,
                                      double dataStep, QString strAddrStart,
                                      int addrStep, bool isRise)
{
    qDebug() << num << dataStart << dataStep << strAddrStart << addrStep;
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
        on_pushBtnPsu2Part5DataAdd_clicked();
        QString strData, strAddr;
        if(isRise)
            strData = QString("%1").arg(dataStart+dataStep*i);
        else
            strData = QString("%1").arg(dataStart-dataStep*i);
        if(isHex){
            strAddr = QString("0x%1").arg(addrStart+addrStep*i, 4, 16, QLatin1Char('0'));
        }else{
            strAddr = QString("%1").arg(addrStart+addrStep*i);
        }
        dataLineEditListPsu2Part5Data.at(nowIndexPsu2Part5Data-1)->setText(strData);
        addrLineEditListPsu2Part5Data.at(nowIndexPsu2Part5Data-1)->setText(strAddr);
    }
}
// ���水ť
void curdataconfig::on_pushBtnPsu2Part5DataSave_clicked()
{
    QList<QPair<bool, QPair<QString, QString> *> *>  *tempList = new QList<QPair<bool, QPair<QString, QString> *> *>;
    // ��������е�����, �ж�����Ĳ����Ƿ�Ϸ�
    for(int i=0; i != nowIndexPsu2Part5Data; ++i){
        QString data, addr;
        data = dataLineEditListPsu2Part5Data.at(i)->text();
        bool ok;
        data.toDouble(&ok);
        if(!ok){
            QMessageBox::information(this, tr("����"), tr("��%1�����ݲ�����Ч�����ݣ�����ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        addr = addrLineEditListPsu2Part5Data.at(i)->text();
        if(!QStringIsInt(addr)){
            QMessageBox::information(this, tr("����"), tr("��%1���ַ������Ч�ĵ�ַ������ʧ�ܣ�").arg(i+1), QMessageBox::Ok);
            return;
        }
        QPair<QString, QString> * tempPair = new QPair<QString, QString>(data, addr);
        tempList->append(new QPair<bool, QPair<QString, QString> * >(checkBoxListPsu2Part5Data.at(i)->checkState(), tempPair));
    }
    // ��յ�ǰ�����б�
    for(int i=0; i != dataAndAddrListPsu2Part5->size(); ++i){
        QPair<bool, QPair<QString, QString> *> * tempWithCheck;
        tempWithCheck = dataAndAddrListPsu2Part5->at(i);
        QPair<QString, QString> * temp;
        temp = tempWithCheck->second;
        delete tempWithCheck;
        delete temp;
    }
    dataAndAddrListPsu2Part5->clear();
    dataAndAddrListPsu2Part5 = tempList;
    if(!dataAndAddrListPsu2Part5->isEmpty()){
        if(psu2Part5 == NULL){
            psu2Part5 = new testItem(cmdListPsu2Part5Pre, dataAndAddrListPsu2Part5,
                                     setCmdPsu2Part5Verify, setPsu2Part5Multi,
                                     dmmCmdPsu2Part5Verify, dmmPsu2Part5Multi,
                                     meterCmdPsu2Part5Verify, meterPsu2Part5Multi,
                                     setCmdPsu2Part5Test, dmmCmdPsu2Part5Test, meterCmdPsu2Part5Test);

        }else
            psu2Part5->setDataList(dataAndAddrListPsu2Part5);
        if(itemPsu2 == NULL)
            itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
        else
            itemPsu2->setPart5(psu2Part5);
        QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
    }
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part5DataUndo_clicked()
{
    on_pushBtnPsu2Part5DataBatchDel_clicked();  // ��������б��
    for(int i=0; i != dataAndAddrListPsu2Part5->size(); ++i){
        on_pushBtnPsu2Part5DataAdd_clicked();
        checkBoxListPsu2Part5Data.at(i)->setChecked(dataAndAddrListPsu2Part5->at(i)->first);
        dataLineEditListPsu2Part5Data.at(i)->setText(dataAndAddrListPsu2Part5->at(i)->second->first);
        addrLineEditListPsu2Part5Data.at(i)->setText(dataAndAddrListPsu2Part5->at(i)->second->second);
    }
}
// PSU2  Part5 У׼
// ���水ť
void curdataconfig::on_pushBtnPsu2Part5VerifySave_clicked()
{
    QString setName, setStart, setEnd, setJudge, setMulti;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part5VerifySetCmd->text();
    setStart = ui->lineEditPsu2Part5VerifySetStart->text();
    setEnd = ui->lineEditPsu2Part5VerifySetEnd->text();
    setJudge = ui->lineEditPsu2Part5VerifySetJudge->text();
    setMulti = ui->lineEditPsu2Part5VerifySetMulti->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0 || setMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmParam, dmmStart, dmmEnd, dmmJudge, dmmMulti;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part5VerifyDMMCmd->text();
    dmmParam = ui->lineEditPsu2Part5VerifyDMMParam->text();
    dmmStart = ui->lineEditPsu2Part5VerifyDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part5VerifyDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part5VerifyDMMJudge->text();
    dmmMulti = ui->lineEditPsu2Part5VerifyDMMMulti->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0 || dmmMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge, meterMulti;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part5VerifyMeterJudge->text();
    meterMulti = ui->lineEditPsu2Part5VerifyMeterMulti->text();
    if(meterJudge.size()==0 || meterMulti.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ���ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part5Verify->setName(setName);  // �������õ����������
    setCmdPsu2Part5Verify->setStart(setStart);
    setCmdPsu2Part5Verify->setEnd(setEnd);
    setCmdPsu2Part5Verify->setJudge(setJudge);
    setPsu2Part5Multi = setMulti.toInt();
    dmmCmdPsu2Part5Verify->setName(dmmName); // �����ȡ��ѹ�������
    dmmCmdPsu2Part5Verify->setParam(dmmParam);
    dmmCmdPsu2Part5Verify->setStart(dmmStart);
    dmmCmdPsu2Part5Verify->setEnd(dmmEnd);
    dmmCmdPsu2Part5Verify->setRatio(dmmJudge.toDouble());
    dmmPsu2Part5Multi = dmmMulti.toInt();
    meterCmdPsu2Part5Verify->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    meterPsu2Part5Multi = meterMulti.toInt();
    if(psu2Part5 == NULL){
        psu2Part5 = new testItem(cmdListPsu2Part5Pre, dataAndAddrListPsu2Part5,
                                 setCmdPsu2Part5Verify, setPsu2Part5Multi,
                                 dmmCmdPsu2Part5Verify, dmmPsu2Part5Multi,
                                 meterCmdPsu2Part5Verify, meterPsu2Part5Multi,
                                 setCmdPsu2Part5Test, dmmCmdPsu2Part5Test, meterCmdPsu2Part5Test);
    }else{
        psu2Part5->setSetCmdVerify(setCmdPsu2Part5Verify);
        psu2Part5->setDmmCmdVerify(dmmCmdPsu2Part5Verify);
        psu2Part5->setMeterCmdVerify(meterCmdPsu2Part5Verify);
        psu2Part5->setSetMulti(setPsu2Part5Multi);
        psu2Part5->setDmmMulti(dmmPsu2Part5Multi);
        psu2Part5->setMeterMulti(meterPsu2Part5Multi);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart5(psu2Part5);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part5VerifyUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part5VerifySetCmd->setText(setCmdPsu2Part5Verify->getName());
    ui->lineEditPsu2Part5VerifySetStart->setText(setCmdPsu2Part5Verify->getStart());
    ui->lineEditPsu2Part5VerifySetEnd->setText(setCmdPsu2Part5Verify->getEnd());
    ui->lineEditPsu2Part5VerifySetJudge->setText(setCmdPsu2Part5Verify->getJudge());
    ui->lineEditPsu2Part5VerifySetMulti->setText(QString("%1").arg(setPsu2Part5Multi));
    // ��д��ȡ��������
    ui->lineEditPsu2Part5VerifyDMMCmd->setText(dmmCmdPsu2Part5Verify->getName());
    ui->lineEditPsu2Part5VerifyDMMStart->setText(dmmCmdPsu2Part5Verify->getStart());
    ui->lineEditPsu2Part5VerifyDMMEnd->setText(dmmCmdPsu2Part5Verify->getEnd());
    ui->lineEditPsu2Part5VerifyDMMJudge->setText(dmmCmdPsu2Part5Verify->getRatio());
    ui->lineEditPsu2Part5VerifyDMMMulti->setText(QString("%1").arg(dmmPsu2Part5Multi));
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part5VerifyMeterJudge->setText(meterCmdPsu2Part5Verify->getRatio());
    ui->lineEditPsu2Part5VerifyMeterMulti->setText(QString("%1").arg(meterPsu2Part5Multi));
}
// PSU2  Part5  ����
// ���水ť
void curdataconfig::on_pushBtnPsu2Part5TestSave_clicked()
{
    QString setName, setStart, setEnd, setJudge;  // ��ȡ���жϵ�����������Ĳ���
    setName = ui->lineEditPsu2Part5TestSetCmd->text();
    setStart = ui->lineEditPsu2Part5TestSetStart->text();
    setEnd = ui->lineEditPsu2Part5TestSetEnd->text();
    setJudge = ui->lineEditPsu2Part5TestSetJudge->text();
    if(setName.size()==0 || setStart.size()==0 || setEnd.size()==0 || setJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("���õ�������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString dmmName, dmmStart, dmmEnd, dmmJudge;  // ��ȡ���жϵ�����ȡ����Ĳ���
    dmmName = ui->lineEditPsu2Part5TestDMMCmd->text();
    dmmStart = ui->lineEditPsu2Part5TestDMMStart->text();
    dmmEnd = ui->lineEditPsu2Part5TestDMMEnd->text();
    dmmJudge = ui->lineEditPsu2Part5TestDMMJudge->text();
    if(dmmName.size()==0 || dmmStart.size()==0 || dmmEnd.size()==0 || dmmJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡDMM��������Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    QString meterJudge;  // ��ȡ���ж����ñ�Ķ�ȡ�Ӳ���
    meterJudge = ui->lineEditPsu2Part5TestMeterJudge->text();
    if(meterJudge.size()==0){
        QMessageBox::information(this, tr("����ʧ��"), tr("��ȡ�����ñ�����Ĳ�����д����������������д"), QMessageBox::Ok);
        return;
    }
    setCmdPsu2Part5Test->setName(setName);  // �������õ����������
    setCmdPsu2Part5Test->setStart(setStart);
    setCmdPsu2Part5Test->setEnd(setEnd);
    setCmdPsu2Part5Test->setJudge(setJudge);
    dmmCmdPsu2Part5Test->setName(dmmName);  // �����ȡ�����������
    dmmCmdPsu2Part5Test->setStart(dmmStart);
    dmmCmdPsu2Part5Test->setEnd(dmmEnd);
    dmmCmdPsu2Part5Test->setRatio(dmmJudge.toDouble());
    meterCmdPsu2Part5Test->setRatio(meterJudge.toDouble());  // �����ȡ���ñ��������
    if(psu2Part5 == NULL){
        psu2Part5 = new testItem(cmdListPsu2Part5Pre, dataAndAddrListPsu2Part5,
                                 setCmdPsu2Part5Verify, setPsu2Part5Multi,
                                 dmmCmdPsu2Part5Verify, dmmPsu2Part5Multi,
                                 meterCmdPsu2Part5Verify, meterPsu2Part5Multi,
                                 setCmdPsu2Part5Test, dmmCmdPsu2Part5Test, meterCmdPsu2Part5Test);
    }else{
        psu2Part5->setSetCmdTest(setCmdPsu2Part5Test);
        psu2Part5->setDmmCmdTest(dmmCmdPsu2Part5Test);
        psu2Part5->setMeterCmdTest(meterCmdPsu2Part5Test);
    }
    if(itemPsu2 == NULL)
        itemPsu2 = new currentItem(cmdListPsu2Pre, psu2Part1, psu2Part2, psu2Part3, psu2Part4, psu2Part5);
    else
        itemPsu2->setPart5(psu2Part5);
    QMessageBox::information(this, tr("����ɹ�"), tr("����ɹ�"), QMessageBox::Ok);
}
// ������ť
void curdataconfig::on_pushBtnPsu2Part5TestUndo_clicked()
{
    // ��д���õ�������
    ui->lineEditPsu2Part5TestSetCmd->setText(setCmdPsu2Part5Test->getName());
    ui->lineEditPsu2Part5TestSetStart->setText(setCmdPsu2Part5Test->getStart());
    ui->lineEditPsu2Part5TestSetEnd->setText(setCmdPsu2Part5Test->getEnd());
    ui->lineEditPsu2Part5TestSetJudge->setText(setCmdPsu2Part5Test->getJudge());
    // ��д��ȡ��������
    ui->lineEditPsu2Part5TestDMMCmd->setText(dmmCmdPsu2Part5Test->getName());
    ui->lineEditPsu2Part5TestDMMStart->setText(dmmCmdPsu2Part5Test->getStart());
    ui->lineEditPsu2Part5TestDMMEnd->setText(dmmCmdPsu2Part5Test->getEnd());
    ui->lineEditPsu2Part5TestDMMJudge->setText(dmmCmdPsu2Part5Test->getRatio());
    // ��д��ȡ���ñ����
    ui->lineEditPsu2Part5TestMeterJudge->setText(meterCmdPsu2Part5Test->getRatio());
}
