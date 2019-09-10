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
    // �˳���ť
    void on_pushBtnExit_clicked();

    // ch1ǰ������ҳ
    // ��Ӱ�ť
    void on_pushBtnCh1PreAdd_clicked();
    // �����б�
    void on_ch1PreCmdList_clicked(const QModelIndex &index);
    // ˢ�������б��
    void showCh1PreCmdList();
    // ɾ����ť
    void on_pushBtnCh1PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnCh1PreUp_clicked();
    // ���°�ť
    void on_pushBtnCh1PreDown_clicked();
    // ���水ť
    void on_pushBtnCh1PreSave_clicked();
    // ������ť
    void on_pushBtnCh1PreUndo_clicked();

    // ch1����ҳ
    // ��Ӱ�ť
    void on_pushBtnCh1DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectCh1();
    // ȫѡ��ť
    void on_pushBtnCh1DataAll_clicked();
    // ɾ����ť
    void on_pushBtnCh1DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnCh1DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnCh1DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnCh1DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsCh1(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnCh1DataSave_clicked();
    // ������ť
    void on_pushBtnCh1DataUndo_clicked();

    // ch1У׼ҳ
    // ���水ť
    void on_pushBtnCh1VerifySave_clicked();
    // ������ť
    void on_pushBtnCh1VerifyUndo_clicked();

    // ch1����ҳ
    // ���水ť
    void on_pushBtnCh1TestSave_clicked();
    // ������ť
    void on_pushBtnCh1TestUndo_clicked();

    // ch2ǰ������ҳ
    // ��Ӱ�ť
    void on_pushBtnCh2PreAdd_clicked();
    // �����б�
    void on_ch2PreCmdList_clicked(const QModelIndex &index);
    // ˢ�������б��
    void showCh2PreCmdList();
    // ɾ����ť
    void on_pushBtnCh2PreDel_clicked();
    // ���ϰ�ť
    void on_pushBtnCh2PreUp_clicked();
    // ���°�ť
    void on_pushBtnCh2PreDown_clicked();
    // ���水ť
    void on_pushBtnCh2PreSave_clicked();
    // ������ť
    void on_pushBtnCh2PreUndo_clicked();

    // ch2����ҳ
    // ��Ӱ�ť
    void on_pushBtnCh2DataAdd_clicked();
    // �ж�ȫѡ״̬
    bool checkBoxAllSelectCh2();
    // ȫѡ��ť
    void on_pushBtnCh2DataAll_clicked();
    // ɾ����ť
    void on_pushBtnCh2DataDel_clicked();
    // ȫ��ɾ����ť
    void on_pushBtnCh2DataBatchDel_clicked();
    // ������ݰ�ť
    void on_pushBtnCh2DataClear_clicked();
    // ������Ӱ�ť
    void on_pushBtnCh2DataBatchAdd_clicked();
    // ������յ���������Ӳ���
    void handleBatchParamsCh2(int, double, double, QString, int, bool);
    // ���水ť
    void on_pushBtnCh2DataSave_clicked();
    // ������ť
    void on_pushBtnCh2DataUndo_clicked();

    // ch2У׼ҳ
    // ���水ť
    void on_pushBtnCh2VerifySave_clicked();
    // ������ť
    void on_pushBtnCh2VerifyUndo_clicked();

    // Ch2����ҳ
    // ���水ť
    void on_pushBtnCh2TestSave_clicked();
    // ������ť
    void on_pushBtnCh2TestUndo_clicked();

private:
    Ui::volDataConfig *ui;
    // ch1ǰ������ҳ
    QList<command *>  cmdListCh1Pre;  // �����б�
    int nowIndexCh1Pre;  // ��ǰ��������
    command * nowCommandCh1;  // ��ǰ����ָ��
    // ch1����ҳ
    int nowIndexCh1Data;  // ��ǰ��������
    QList<QFrame *> frameListCh1Data;  // ���б�
    QList<QCheckBox *> checkBoxListCh1Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListCh1Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListCh1Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListCh1; // �����б�
    // ch1У׼ҳ
    command * setCmdCh1Verify;  // У׼���õ�ѹ����
    int setCh1Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdCh1Verify;  // У׼��ȡ��ѹ����
    int dmmCh1Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdCh1Verify;  // У׼�����ñ�����
    int meterCh1Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // ch1����ҳ
    command * setCmdCh1Test;  // �������õ�ѹ����
    command * dmmCmdCh1Test;  // ���Զ�ȡ��ѹ����
    command * meterCmdCh1Test;  // ���Զ����ñ�����

    // ch2ǰ������ҳ
    QList<command *>  cmdListCh2Pre;  // �����б�
    int nowIndexCh2Pre;  // ��ǰ��������
    command * nowCommandCh2;  // ��ǰ����ָ��
    // ch2����ҳ
    int nowIndexCh2Data;  // ��ǰ��������
    QList<QFrame *> frameListCh2Data;  // ���б�
    QList<QCheckBox *> checkBoxListCh2Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListCh2Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListCh2Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListCh2; // �����б�
    // ch2У׼ҳ
    command * setCmdCh2Verify;  // У׼���õ�ѹ����
    int setCh2Multi;  // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdCh2Verify;  // У׼��ȡ��ѹ����
    int dmmCh2Multi;  // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdCh2Verify;  // У׼�����ñ�����
    int meterCh2Multi;  // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // Ch2����ҳ
    command * setCmdCh2Test;  // �������õ�ѹ����
    command * dmmCmdCh2Test;  // ���Զ�ȡ��ѹ����
    command * meterCmdCh2Test;  // ���Զ����ñ�����
};

#endif // VOLDATACONFIG_H
