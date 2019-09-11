#ifndef CURDATACONFIG_H
#define CURDATACONFIG_H

#include <QDialog>
#include <QModelIndex>
#include <QFrame>
#include <QList>
#include <QCheckBox>
#include <QLineEdit>
#include "command.h"

namespace Ui {
class curdataconfig;
}

class curdataconfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit curdataconfig(QWidget *parent = 0);
    ~curdataconfig();
    
private slots:
    void on_pushBtnExit_clicked(); // �˳�
    // PSU1  ǰ������
    // ��Ӱ�ť
    void on_pushBtnPsu1PreAdd_clicked();
    // ˢ�������б��
    void showPsu1PreCmdList();
    // �������
    void on_psu1PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu1PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu1PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu1PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu1PreSave_clicked();
    // ������ť
    void on_pushBtnPsu1PreUndo_clicked();
    // PSU1  Part1  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu1Part1PreAdd_clicked();
    // ˢ�������б��
    void showPsu1Part1PreCmdList();
    // �������
    void on_psu1Part1PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu1Part1PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu1Part1PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu1Part1PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu1Part1PreSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part1PreUndo_clicked();
    // PSU1 Part1 ����
    // ��Ӱ�ť
    void on_pushBtnPsu1Part1DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu1Part1();
    // ȫѡ��ť
    void on_pushBtnPsu1Part1DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu1Part1DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu1Part1DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu1Part1DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu1Part1DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu1Part1(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu1Part1DataSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part1DataUndo_clicked();
    // PSU1 Part1 У׼
    // ���水ť
    void on_pushBtnPsu1Part1VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu1Part1VerifyUndo_clicked();
    // PSU1  Part1  ����ҳ
    // ���水ť
    void on_pushBtnPsu1Part1TestSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part1TestUndo_clicked();
    // PSU1  Part2  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu1Part2PreAdd_clicked();
    // ˢ�������б��
    void showPsu1Part2PreCmdList();
    // �������
    void on_psu1Part2PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu1Part2PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu1Part2PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu1Part2PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu1Part2PreSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part2PreUndo_clicked();
    // PSU1 Part2 ����
    // ��Ӱ�ť
    void on_pushBtnPsu1Part2DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu1Part2();
    // ȫѡ��ť
    void on_pushBtnPsu1Part2DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu1Part2DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu1Part2DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu1Part2DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu1Part2DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu1Part2(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu1Part2DataSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part2DataUndo_clicked();
    // PSU1 Part2 У׼
    // ���水ť
    void on_pushBtnPsu1Part2VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu1Part2VerifyUndo_clicked();
    // PSU1  Part2  ����ҳ
    // ���水ť
    void on_pushBtnPsu1Part2TestSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part2TestUndo_clicked();
    // PSU1  Part3  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu1Part3PreAdd_clicked();
    // ˢ�������б��
    void showPsu1Part3PreCmdList();
    // �������
    void on_psu1Part3PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu1Part3PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu1Part3PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu1Part3PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu1Part3PreSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part3PreUndo_clicked();
    // PSU1 Part3 ����
    // ��Ӱ�ť
    void on_pushBtnPsu1Part3DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu1Part3();
    // ȫѡ��ť
    void on_pushBtnPsu1Part3DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu1Part3DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu1Part3DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu1Part3DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu1Part3DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu1Part3(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu1Part3DataSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part3DataUndo_clicked();
    // PSU1 Part3 У׼
    // ���水ť
    void on_pushBtnPsu1Part3VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu1Part3VerifyUndo_clicked();
    // PSU1  Part3  ����ҳ
    // ���水ť
    void on_pushBtnPsu1Part3TestSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part3TestUndo_clicked();
    // PSU1  Part4  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu1Part4PreAdd_clicked();
    // ˢ�������б��
    void showPsu1Part4PreCmdList();
    // �������
    void on_psu1Part4PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu1Part4PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu1Part4PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu1Part4PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu1Part4PreSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part4PreUndo_clicked();
    // PSU1 Part4 ����
    // ��Ӱ�ť
    void on_pushBtnPsu1Part4DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu1Part4();
    // ȫѡ��ť
    void on_pushBtnPsu1Part4DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu1Part4DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu1Part4DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu1Part4DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu1Part4DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu1Part4(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu1Part4DataSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part4DataUndo_clicked();
    // PSU1 Part4 У׼
    // ���水ť
    void on_pushBtnPsu1Part4VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu1Part4VerifyUndo_clicked();
    // PSU1  Part4  ����ҳ
    // ���水ť
    void on_pushBtnPsu1Part4TestSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part4TestUndo_clicked();
    // PSU1  Part1  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu1Part5PreAdd_clicked();
    // ˢ�������б��
    void showPsu1Part5PreCmdList();
    // �������
    void on_psu1Part5PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu1Part5PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu1Part5PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu1Part5PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu1Part5PreSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part5PreUndo_clicked();
    // PSU1 Part5 ����
    // ��Ӱ�ť
    void on_pushBtnPsu1Part5DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu1Part5();
    // ȫѡ��ť
    void on_pushBtnPsu1Part5DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu1Part5DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu1Part5DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu1Part5DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu1Part5DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu1Part5(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu1Part5DataSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part5DataUndo_clicked();
    // PSU1 Part5 У׼
    // ���水ť
    void on_pushBtnPsu1Part5VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu1Part5VerifyUndo_clicked();
    // PSU1  Part5  ����ҳ
    // ���水ť
    void on_pushBtnPsu1Part5TestSave_clicked();
    // ������ť
    void on_pushBtnPsu1Part5TestUndo_clicked();
    // PSU2  ǰ������
    // ��Ӱ�ť
    void on_pushBtnPsu2PreAdd_clicked();
    // ˢ�������б��
    void showPsu2PreCmdList();
    // �������
    void on_psu2PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu2PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu2PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu2PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu2PreSave_clicked();
    // ������ť
    void on_pushBtnPsu2PreUndo_clicked();
    // PSU2  Part1  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu2Part1PreAdd_clicked();
    // ˢ�������б��
    void showPsu2Part1PreCmdList();
    // �������
    void on_psu2Part1PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu2Part1PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu2Part1PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu2Part1PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu2Part1PreSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part1PreUndo_clicked();
    // PSU2 Part1 ����
    // ��Ӱ�ť
    void on_pushBtnPsu2Part1DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu2Part1();
    // ȫѡ��ť
    void on_pushBtnPsu2Part1DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu2Part1DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu2Part1DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu2Part1DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu2Part1DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu2Part1(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu2Part1DataSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part1DataUndo_clicked();
    // PSU2 Part1 У׼
    // ���水ť
    void on_pushBtnPsu2Part1VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu2Part1VerifyUndo_clicked();
    // PSU2  Part1  ����ҳ
    // ���水ť
    void on_pushBtnPsu2Part1TestSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part1TestUndo_clicked();
    // PSU2  Part2  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu2Part2PreAdd_clicked();
    // ˢ�������б��
    void showPsu2Part2PreCmdList();
    // �������
    void on_psu2Part2PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu2Part2PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu2Part2PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu2Part2PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu2Part2PreSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part2PreUndo_clicked();
    // PSU2 Part2 ����
    // ��Ӱ�ť
    void on_pushBtnPsu2Part2DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu2Part2();
    // ȫѡ��ť
    void on_pushBtnPsu2Part2DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu2Part2DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu2Part2DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu2Part2DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu2Part2DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu2Part2(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu2Part2DataSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part2DataUndo_clicked();
    // PSU2 Part2 У׼
    // ���水ť
    void on_pushBtnPsu2Part2VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu2Part2VerifyUndo_clicked();
    // PSU2  Part2  ����ҳ
    // ���水ť
    void on_pushBtnPsu2Part2TestSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part2TestUndo_clicked();
    // PSU2  Part3  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu2Part3PreAdd_clicked();
    // ˢ�������б��
    void showPsu2Part3PreCmdList();
    // �������
    void on_psu2Part3PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu2Part3PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu2Part3PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu2Part3PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu2Part3PreSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part3PreUndo_clicked();
    // PSU2 Part3 ����
    // ��Ӱ�ť
    void on_pushBtnPsu2Part3DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu2Part3();
    // ȫѡ��ť
    void on_pushBtnPsu2Part3DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu2Part3DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu2Part3DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu2Part3DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu2Part3DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu2Part3(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu2Part3DataSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part3DataUndo_clicked();
    // PSU2 Part3 У׼
    // ���水ť
    void on_pushBtnPsu2Part3VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu2Part3VerifyUndo_clicked();
    // PSU2  Part3  ����ҳ
    // ���水ť
    void on_pushBtnPsu2Part3TestSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part3TestUndo_clicked();
    // PSU2  Part4  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu2Part4PreAdd_clicked();
    // ˢ�������б��
    void showPsu2Part4PreCmdList();
    // �������
    void on_psu2Part4PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu2Part4PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu2Part4PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu2Part4PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu2Part4PreSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part4PreUndo_clicked();
    // PSU2 Part4 ����
    // ��Ӱ�ť
    void on_pushBtnPsu2Part4DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu2Part4();
    // ȫѡ��ť
    void on_pushBtnPsu2Part4DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu2Part4DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu2Part4DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu2Part4DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu2Part4DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu2Part4(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu2Part4DataSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part4DataUndo_clicked();
    // PSU2 Part4 У׼
    // ���水ť
    void on_pushBtnPsu2Part4VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu2Part4VerifyUndo_clicked();
    // PSU2  Part4  ����ҳ
    // ���水ť
    void on_pushBtnPsu2Part4TestSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part4TestUndo_clicked();
    // PSU2  Part1  ��������
    // ��Ӱ�ť
    void on_pushBtnPsu2Part5PreAdd_clicked();
    // ˢ�������б��
    void showPsu2Part5PreCmdList();
    // �������
    void on_psu2Part5PreCmdList_clicked(const QModelIndex &index);
    // ɾ����ť
    void on_pushBtnPsu2Part5PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnPsu2Part5PreUp_clicked();
    // ���°�ť
    void on_pushBtnPsu2Part5PreDown_clicked();
    // ���水ť
    void on_pushBtnPsu2Part5PreSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part5PreUndo_clicked();
    // PSU2 Part5 ����
    // ��Ӱ�ť
    void on_pushBtnPsu2Part5DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectPsu2Part5();
    // ȫѡ��ť
    void on_pushBtnPsu2Part5DataAll_clicked();
    // ɾ����ť
    void on_pushBtnPsu2Part5DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnPsu2Part5DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnPsu2Part5DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnPsu2Part5DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsPsu2Part5(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnPsu2Part5DataSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part5DataUndo_clicked();
    // PSU2 Part5 У׼
    // ���水ť
    void on_pushBtnPsu2Part5VerifySave_clicked();
    // ������ť
    void on_pushBtnPsu2Part5VerifyUndo_clicked();
    // PSU2  Part5  ����ҳ
    // ���水ť
    void on_pushBtnPsu2Part5TestSave_clicked();
    // ������ť
    void on_pushBtnPsu2Part5TestUndo_clicked();

private:
    Ui::curdataconfig *ui;
    // PSU1  ǰ��
    QList<command *>  cmdListPsu1Pre;  // �����б�
    int nowIndexPsu1Pre;  // ��ǰ��������
    command * nowCommandPsu1;  // ��ǰ����ָ��
    // PSU1  Part1  ����
    QList<command *>  cmdListPsu1Part1Pre;  // �����б�
    int nowIndexPsu1Part1Pre;  // ��ǰ��������
    command * nowCommandPsu1Part1;  // ��ǰ����ָ��
    // PSU1  Part1  ����
    int nowIndexPsu1Part1Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu1Part1Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part1Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part1Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part1Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part1; // �����б�
    // PSU1  Part1  У׼
    command * setCmdPsu1Part1Verify;  // У׼���õ�ѹ����
    int setPsu1Part1Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part1Verify;  // У׼��ȡ��ѹ����
    int dmmPsu1Part1Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part1Verify;  // У׼�����ñ�����
    int meterPsu1Part1Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part1  ����
    command * setCmdPsu1Part1Test;  // �������õ�������
    command * dmmCmdPsu1Part1Test;  // ���Զ�ȡ��������
    command * meterCmdPsu1Part1Test;  // ���Զ����ñ�����
    // PSU1  Part2  ����
    QList<command *>  cmdListPsu1Part2Pre;  // �����б�
    int nowIndexPsu1Part2Pre;  // ��ǰ��������
    command * nowCommandPsu1Part2;  // ��ǰ����ָ��
    // PSU1  Part2  ����
    int nowIndexPsu1Part2Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu1Part2Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part2Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part2Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part2Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part2; // �����б�
    // PSU1   Part2  У׼
    command * setCmdPsu1Part2Verify;  // У׼���õ�ѹ����
    int setPsu1Part2Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part2Verify;  // У׼��ȡ��ѹ����
    int dmmPsu1Part2Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part2Verify;  // У׼�����ñ�����
    int meterPsu1Part2Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part2  ����
    command * setCmdPsu1Part2Test;  // �������õ�������
    command * dmmCmdPsu1Part2Test;  // ���Զ�ȡ��������
    command * meterCmdPsu1Part2Test;  // ���Զ����ñ�����
    // PSU1  Part3  ����
    QList<command *>  cmdListPsu1Part3Pre;  // �����б�
    int nowIndexPsu1Part3Pre;  // ��ǰ��������
    command * nowCommandPsu1Part3;  // ��ǰ����ָ��
    // PSU1  Part3  ����
    int nowIndexPsu1Part3Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu1Part3Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part3Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part3Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part3Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part3; // �����б�
    // PSU1  Part3  У׼
    command * setCmdPsu1Part3Verify;  // У׼���õ�ѹ����
    int setPsu1Part3Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part3Verify;  // У׼��ȡ��ѹ����
    int dmmPsu1Part3Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part3Verify;  // У׼�����ñ�����
    int meterPsu1Part3Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part3  ����
    command * setCmdPsu1Part3Test;  // �������õ�������
    command * dmmCmdPsu1Part3Test;  // ���Զ�ȡ��������
    command * meterCmdPsu1Part3Test;  // ���Զ����ñ�����
    // PSU1  Part4  ����
    QList<command *>  cmdListPsu1Part4Pre;  // �����б�
    int nowIndexPsu1Part4Pre;  // ��ǰ��������
    command * nowCommandPsu1Part4;  // ��ǰ����ָ��
    // PSU1  Part4  ����
    int nowIndexPsu1Part4Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu1Part4Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part4Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part4Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part4Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part4; // �����б�
    // PSU1  Part4  У׼
    command * setCmdPsu1Part4Verify;  // У׼���õ�ѹ����
    int setPsu1Part4Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part4Verify;  // У׼��ȡ��ѹ����
    int dmmPsu1Part4Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part4Verify;  // У׼�����ñ�����
    int meterPsu1Part4Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part4  ����
    command * setCmdPsu1Part4Test;  // �������õ�������
    command * dmmCmdPsu1Part4Test;  // ���Զ�ȡ��������
    command * meterCmdPsu1Part4Test;  // ���Զ����ñ�����
    // PSU1  Part1  ����
    QList<command *>  cmdListPsu1Part5Pre;  // �����б�
    int nowIndexPsu1Part5Pre;  // ��ǰ��������
    command * nowCommandPsu1Part5;  // ��ǰ����ָ��
    // PSU1  Part5  ����
    int nowIndexPsu1Part5Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu1Part5Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part5Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part5Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part5Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part5; // �����б�
    // PSU1  Part5  У׼
    command * setCmdPsu1Part5Verify;  // У׼���õ�ѹ����
    int setPsu1Part5Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part5Verify;  // У׼��ȡ��ѹ����
    int dmmPsu1Part5Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part5Verify;  // У׼�����ñ�����
    int meterPsu1Part5Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part5  ����
    command * setCmdPsu1Part5Test;  // �������õ�������
    command * dmmCmdPsu1Part5Test;  // ���Զ�ȡ��������
    command * meterCmdPsu1Part5Test;  // ���Զ����ñ�����
    // PSU2  ǰ��
    QList<command *>  cmdListPsu2Pre;  // �����б�
    int nowIndexPsu2Pre;  // ��ǰ��������
    command * nowCommandPsu2;  // ��ǰ����ָ��
    // PSU2  Part1  ����
    QList<command *>  cmdListPsu2Part1Pre;  // �����б�
    int nowIndexPsu2Part1Pre;  // ��ǰ��������
    command * nowCommandPsu2Part1;  // ��ǰ����ָ��
    // PSU2  Part1  ����
    int nowIndexPsu2Part1Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu2Part1Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part1Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part1Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part1Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part1; // �����б�
    // PSU2  Part1  У׼
    command * setCmdPsu2Part1Verify;  // У׼���õ�ѹ����
    int setPsu2Part1Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part1Verify;  // У׼��ȡ��ѹ����
    int dmmPsu2Part1Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part1Verify;  // У׼�����ñ�����
    int meterPsu2Part1Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part1  ����
    command * setCmdPsu2Part1Test;  // �������õ�������
    command * dmmCmdPsu2Part1Test;  // ���Զ�ȡ��������
    command * meterCmdPsu2Part1Test;  // ���Զ����ñ�����
    // PSU2  Part2  ����
    QList<command *>  cmdListPsu2Part2Pre;  // �����б�
    int nowIndexPsu2Part2Pre;  // ��ǰ��������
    command * nowCommandPsu2Part2;  // ��ǰ����ָ��
    // PSU2  Part2  ����
    int nowIndexPsu2Part2Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu2Part2Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part2Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part2Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part2Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part2; // �����б�
    // PSU2   Part2  У׼
    command * setCmdPsu2Part2Verify;  // У׼���õ�ѹ����
    int setPsu2Part2Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part2Verify;  // У׼��ȡ��ѹ����
    int dmmPsu2Part2Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part2Verify;  // У׼�����ñ�����
    int meterPsu2Part2Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part2  ����
    command * setCmdPsu2Part2Test;  // �������õ�������
    command * dmmCmdPsu2Part2Test;  // ���Զ�ȡ��������
    command * meterCmdPsu2Part2Test;  // ���Զ����ñ�����
    // PSU2  Part3  ����
    QList<command *>  cmdListPsu2Part3Pre;  // �����б�
    int nowIndexPsu2Part3Pre;  // ��ǰ��������
    command * nowCommandPsu2Part3;  // ��ǰ����ָ��
    // PSU2  Part3  ����
    int nowIndexPsu2Part3Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu2Part3Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part3Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part3Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part3Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part3; // �����б�
    // PSU2  Part3  У׼
    command * setCmdPsu2Part3Verify;  // У׼���õ�ѹ����
    int setPsu2Part3Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part3Verify;  // У׼��ȡ��ѹ����
    int dmmPsu2Part3Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part3Verify;  // У׼�����ñ�����
    int meterPsu2Part3Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part3  ����
    command * setCmdPsu2Part3Test;  // �������õ�������
    command * dmmCmdPsu2Part3Test;  // ���Զ�ȡ��������
    command * meterCmdPsu2Part3Test;  // ���Զ����ñ�����
    // PSU2  Part4  ����
    QList<command *>  cmdListPsu2Part4Pre;  // �����б�
    int nowIndexPsu2Part4Pre;  // ��ǰ��������
    command * nowCommandPsu2Part4;  // ��ǰ����ָ��
    // PSU2  Part4  ����
    int nowIndexPsu2Part4Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu2Part4Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part4Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part4Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part4Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part4; // �����б�
    // PSU2  Part4  У׼
    command * setCmdPsu2Part4Verify;  // У׼���õ�ѹ����
    int setPsu2Part4Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part4Verify;  // У׼��ȡ��ѹ����
    int dmmPsu2Part4Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part4Verify;  // У׼�����ñ�����
    int meterPsu2Part4Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part4  ����
    command * setCmdPsu2Part4Test;  // �������õ�������
    command * dmmCmdPsu2Part4Test;  // ���Զ�ȡ��������
    command * meterCmdPsu2Part4Test;  // ���Զ����ñ�����
    // PSU2  Part1  ����
    QList<command *>  cmdListPsu2Part5Pre;  // �����б�
    int nowIndexPsu2Part5Pre;  // ��ǰ��������
    command * nowCommandPsu2Part5;  // ��ǰ����ָ��
    // PSU2  Part5  ����
    int nowIndexPsu2Part5Data;  // ��ǰ��������
    QList<QFrame *> frameListPsu2Part5Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part5Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part5Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part5Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part5; // �����б�
    // PSU2  Part5  У׼
    command * setCmdPsu2Part5Verify;  // У׼���õ�ѹ����
    int setPsu2Part5Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part5Verify;  // У׼��ȡ��ѹ����
    int dmmPsu2Part5Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part5Verify;  // У׼�����ñ�����
    int meterPsu2Part5Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part5  ����
    command * setCmdPsu2Part5Test;  // �������õ�������
    command * dmmCmdPsu2Part5Test;  // ���Զ�ȡ��������
    command * meterCmdPsu2Part5Test;  // ���Զ����ñ�����
};

#endif // CURDATACONFIG_H
