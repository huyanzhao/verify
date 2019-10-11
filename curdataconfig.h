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
    void returnTestItem(currentItem *, currentItem *);  // �����趨�Ĳ�����������
    
private slots:
    void on_pushBtnExit_clicked(); // �˳�
    // PSU1  ǰ������
    void on_pushBtnPsu1PreAdd_clicked();             // ��Ӱ�ť
    void showPsu1PreCmdList();                       // ˢ�������б��
    void on_psu1PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu1PreDel_clicked();             // ɾ����ť
    void on_pushBtnPsu1PreUp_clicked();              // ���ϰ�ť
    void on_pushBtnPsu1PreDown_clicked();            // ���°�ť
    void on_pushBtnPsu1PreSave_clicked();            // ���水ť
    void on_pushBtnPsu1PreUndo_clicked();            // ������ť
    // PSU1  Part1  ��������
    void on_pushBtnPsu1Part1PreAdd_clicked();        // ��Ӱ�ť
    void showPsu1Part1PreCmdList();                  // ˢ�������б��
    void on_psu1Part1PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu1Part1PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu1Part1PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu1Part1PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu1Part1PreSave_clicked();       // ���水ť
    void on_pushBtnPsu1Part1PreUndo_clicked();       // ������ť
    // PSU1 Part1 ����
    void on_pushBtnPsu1Part1DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu1Part1();               // �ж�ȫѡ״̬
    void on_pushBtnPsu1Part1DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu1Part1DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu1Part1DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu1Part1DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu1Part1DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu1Part1(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu1Part1_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu1Part1DataSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part1DataUndo_clicked();      // ������ť
    // PSU1 Part1 У׼
    void on_pushBtnPsu1Part1VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu1Part1VerifyUndo_clicked();    // ������ť
    // PSU1  Part1  ����ҳ
    void on_pushBtnPsu1Part1TestSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part1TestUndo_clicked();      // ������ť
    // PSU1  Part2  ��������
    void on_pushBtnPsu1Part2PreAdd_clicked();        // ��Ӱ�ť
    void showPsu1Part2PreCmdList();                  // ˢ�������б��
    void on_psu1Part2PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu1Part2PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu1Part2PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu1Part2PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu1Part2PreSave_clicked();       // ���水ť
    void on_pushBtnPsu1Part2PreUndo_clicked();       // ������ť
    // PSU1 Part2 ����
    void on_pushBtnPsu1Part2DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu1Part2();               // �ж�ȫѡ״̬
    void on_pushBtnPsu1Part2DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu1Part2DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu1Part2DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu1Part2DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu1Part2DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu1Part2(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu1Part2_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu1Part2DataSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part2DataUndo_clicked();      // ������ť
    // PSU1 Part2 У׼
    void on_pushBtnPsu1Part2VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu1Part2VerifyUndo_clicked();    // ������ť
    // PSU1  Part2  ����ҳ
    void on_pushBtnPsu1Part2TestSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part2TestUndo_clicked();      // ������ť
    // PSU1  Part3  ��������
    void on_pushBtnPsu1Part3PreAdd_clicked();        // ��Ӱ�ť
    void showPsu1Part3PreCmdList();                  // ˢ�������б��
    void on_psu1Part3PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu1Part3PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu1Part3PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu1Part3PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu1Part3PreSave_clicked();       // ���水ť
    void on_pushBtnPsu1Part3PreUndo_clicked();       // ������ť
    // PSU1 Part3 ����
    void on_pushBtnPsu1Part3DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu1Part3();               // �ж�ȫѡ״̬
    void on_pushBtnPsu1Part3DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu1Part3DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu1Part3DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu1Part3DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu1Part3DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu1Part3(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu1Part3_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu1Part3DataSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part3DataUndo_clicked();      // ������ť
    // PSU1 Part3 У׼
    void on_pushBtnPsu1Part3VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu1Part3VerifyUndo_clicked();    // ������ť
    // PSU1  Part3  ����ҳ
    void on_pushBtnPsu1Part3TestSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part3TestUndo_clicked();      // ������ť
    // PSU1  Part4  ��������
    void on_pushBtnPsu1Part4PreAdd_clicked();        // ��Ӱ�ť
    void showPsu1Part4PreCmdList();                  // ˢ�������б��
    void on_psu1Part4PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu1Part4PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu1Part4PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu1Part4PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu1Part4PreSave_clicked();       // ���水ť
    void on_pushBtnPsu1Part4PreUndo_clicked();       // ������ť
    // PSU1 Part4 ����
    void on_pushBtnPsu1Part4DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu1Part4();               // �ж�ȫѡ״̬
    void on_pushBtnPsu1Part4DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu1Part4DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu1Part4DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu1Part4DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu1Part4DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu1Part4(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu1Part4_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu1Part4DataSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part4DataUndo_clicked();      // ������ť
    // PSU1 Part4 У׼
    void on_pushBtnPsu1Part4VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu1Part4VerifyUndo_clicked();    // ������ť
    // PSU1  Part4  ����ҳ
    void on_pushBtnPsu1Part4TestSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part4TestUndo_clicked();      // ������ť
    // PSU1  Part5  ��������
    void on_pushBtnPsu1Part5PreAdd_clicked();        // ��Ӱ�ť
    void showPsu1Part5PreCmdList();                  // ˢ�������б��
    void on_psu1Part5PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu1Part5PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu1Part5PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu1Part5PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu1Part5PreSave_clicked();       // ���水ť
    void on_pushBtnPsu1Part5PreUndo_clicked();       // ������ť
    // PSU1 Part5 ����
    void on_pushBtnPsu1Part5DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu1Part5();               // �ж�ȫѡ״̬
    void on_pushBtnPsu1Part5DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu1Part5DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu1Part5DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu1Part5DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu1Part5DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu1Part5(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu1Part5_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu1Part5DataSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part5DataUndo_clicked();      // ������ť
    // PSU1 Part5 У׼
    void on_pushBtnPsu1Part5VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu1Part5VerifyUndo_clicked();    // ������ť
    // PSU1  Part5  ����ҳ
    void on_pushBtnPsu1Part5TestSave_clicked();      // ���水ť
    void on_pushBtnPsu1Part5TestUndo_clicked();      // ������ť
    // PSU2  ǰ������
    void on_pushBtnPsu2PreAdd_clicked();             // ��Ӱ�ť
    void showPsu2PreCmdList();                       // ˢ�������б��
    void on_psu2PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu2PreDel_clicked();             // ɾ����ť
    void on_pushBtnPsu2PreUp_clicked();              // ���ϰ�ť
    void on_pushBtnPsu2PreDown_clicked();            // ���°�ť
    void on_pushBtnPsu2PreSave_clicked();            // ���水ť
    void on_pushBtnPsu2PreUndo_clicked();            // ������ť
    // PSU2  Part1  ��������
    void on_pushBtnPsu2Part1PreAdd_clicked();        // ��Ӱ�ť
    void showPsu2Part1PreCmdList();                  // ˢ�������б��
    void on_psu2Part1PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu2Part1PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu2Part1PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu2Part1PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu2Part1PreSave_clicked();       // ���水ť
    void on_pushBtnPsu2Part1PreUndo_clicked();       // ������ť
    // PSU2 Part1 ����
    void on_pushBtnPsu2Part1DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu2Part1();               // �ж�ȫѡ״̬
    void on_pushBtnPsu2Part1DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu2Part1DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu2Part1DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu2Part1DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu2Part1DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu2Part1(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu2Part1_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu2Part1DataSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part1DataUndo_clicked();      // ������ť
    // PSU2 Part1 У׼
    void on_pushBtnPsu2Part1VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu2Part1VerifyUndo_clicked();    // ������ť
    // PSU2  Part1  ����ҳ
    void on_pushBtnPsu2Part1TestSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part1TestUndo_clicked();      // ������ť
    // PSU2  Part2  ��������
    void on_pushBtnPsu2Part2PreAdd_clicked();        // ��Ӱ�ť
    void showPsu2Part2PreCmdList();                  // ˢ�������б��
    void on_psu2Part2PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu2Part2PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu2Part2PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu2Part2PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu2Part2PreSave_clicked();       // ���水ť
    void on_pushBtnPsu2Part2PreUndo_clicked();       // ������ť
    // PSU2 Part2 ����
    void on_pushBtnPsu2Part2DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu2Part2();               // �ж�ȫѡ״̬
    void on_pushBtnPsu2Part2DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu2Part2DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu2Part2DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu2Part2DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu2Part2DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu2Part2(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu2Part2_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu2Part2DataSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part2DataUndo_clicked();      // ������ť
    // PSU2 Part2 У׼
    void on_pushBtnPsu2Part2VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu2Part2VerifyUndo_clicked();    // ������ť
    // PSU2  Part2  ����ҳ
    void on_pushBtnPsu2Part2TestSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part2TestUndo_clicked();      // ������ť
    // PSU2  Part3  ��������
    void on_pushBtnPsu2Part3PreAdd_clicked();        // ��Ӱ�ť
    void showPsu2Part3PreCmdList();                  // ˢ�������б��
    void on_psu2Part3PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu2Part3PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu2Part3PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu2Part3PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu2Part3PreSave_clicked();       // ���水ť
    void on_pushBtnPsu2Part3PreUndo_clicked();       // ������ť
    // PSU2 Part3 ����
    void on_pushBtnPsu2Part3DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu2Part3();               // �ж�ȫѡ״̬
    void on_pushBtnPsu2Part3DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu2Part3DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu2Part3DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu2Part3DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu2Part3DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu2Part3(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu2Part3_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu2Part3DataSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part3DataUndo_clicked();      // ������ť
    // PSU2 Part3 У׼
    void on_pushBtnPsu2Part3VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu2Part3VerifyUndo_clicked();    // ������ť
    // PSU2  Part3  ����ҳ
    void on_pushBtnPsu2Part3TestSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part3TestUndo_clicked();      // ������ť
    // PSU2  Part4  ��������
    void on_pushBtnPsu2Part4PreAdd_clicked();        // ��Ӱ�ť
    void showPsu2Part4PreCmdList();                  // ˢ�������б��
    void on_psu2Part4PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu2Part4PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu2Part4PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu2Part4PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu2Part4PreSave_clicked();       // ���水ť
    void on_pushBtnPsu2Part4PreUndo_clicked();       // ������ť
    // PSU2 Part4 ����
    void on_pushBtnPsu2Part4DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu2Part4();               // �ж�ȫѡ״̬
    void on_pushBtnPsu2Part4DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu2Part4DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu2Part4DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu2Part4DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu2Part4DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu2Part4(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu2Part4_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu2Part4DataSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part4DataUndo_clicked();      // ������ť
    // PSU2 Part4 У׼
    void on_pushBtnPsu2Part4VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu2Part4VerifyUndo_clicked();    // ������ť
    // PSU2  Part4  ����ҳ
    void on_pushBtnPsu2Part4TestSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part4TestUndo_clicked();      // ������ť
    // PSU2  Part5  ��������
    void on_pushBtnPsu2Part5PreAdd_clicked();        // ��Ӱ�ť
    void showPsu2Part5PreCmdList();                  // ˢ�������б��
    void on_psu2Part5PreCmdList_clicked(const QModelIndex &index);  // �������
    void on_pushBtnPsu2Part5PreDel_clicked();        // ɾ����ť
    void on_pushBtnPsu2Part5PreUp_clicked();         // ���ϰ�ť
    void on_pushBtnPsu2Part5PreDown_clicked();       // ���°�ť
    void on_pushBtnPsu2Part5PreSave_clicked();       // ���水ť
    void on_pushBtnPsu2Part5PreUndo_clicked();       // ������ť
    // PSU2 Part5 ����
    void on_pushBtnPsu2Part5DataAdd_clicked();       // ��Ӱ�ť
    bool checkBoxAllSelectPsu2Part5();               // �ж�ȫѡ״̬
    void on_pushBtnPsu2Part5DataAll_clicked();       // ȫѡ��ť
    void on_pushBtnPsu2Part5DataDel_clicked();       // ɾ����ť
    void on_pushBtnPsu2Part5DataBatchDel_clicked();  // ȫ��ɾ����ť
    void on_pushBtnPsu2Part5DataClear_clicked();     // ������ݰ�ť
    void on_pushBtnPsu2Part5DataBatchAdd_clicked();  // ������Ӱ�ť
    void handleBatchParamsPsu2Part5(int, double, double, QString, bool);  // ������յ���������Ӳ���
    void on_lineEditDataLengthPsu2Part5_textEdited();  // ���ݳ��ȱ༭
    void on_pushBtnPsu2Part5DataSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part5DataUndo_clicked();      // ������ť
    // PSU2 Part5 У׼
    void on_pushBtnPsu2Part5VerifySave_clicked();    // ���水ť
    void on_pushBtnPsu2Part5VerifyUndo_clicked();    // ������ť
    // PSU2  Part5  ����ҳ
    void on_pushBtnPsu2Part5TestSave_clicked();      // ���水ť
    void on_pushBtnPsu2Part5TestUndo_clicked();      // ������ť

    void closeEvent(QCloseEvent *);                  // �˳��¼�

    void on_pushBtnPsu1ToPsu2_clicked();             // ��psu1����psu2
    void on_pushBtnPsu2ToPsu1_clicked();             // ��psu2����psu1

    void repaintPsu1();                              // �ػ�psu1����
    void repaintPsu1Part1();                         // �ػ�psu1 part1����
    void repaintPsu1Part2();                         // �ػ�psu1 part2����
    void repaintPsu1Part3();                         // �ػ�psu1 part3����
    void repaintPsu1Part4();                         // �ػ�psu1 part4����
    void repaintPsu1Part5();                         // �ػ�psu1 part5����
    void repaintPsu2();                              // �ػ�psu2����
    void repaintPsu2Part1();                         // �ػ�psu2 part1����
    void repaintPsu2Part2();                         // �ػ�psu2 part2����
    void repaintPsu2Part3();                         // �ػ�psu2 part3����
    void repaintPsu2Part4();                         // �ػ�psu2 part4����
    void repaintPsu2Part5();                         // �ػ�psu2 part5����

private:
    Ui::curdataconfig *ui;
    currentItem * itemPsu1;
    currentItem * itemPsu2;
    // PSU1  ǰ��
    QList<command *> * cmdListPsu1Pre;   // �����б�
    int nowIndexPsu1Pre;                // ��ǰ��������
    command * nowCommandPsu1;           // ��ǰ����ָ��    
    testItem * psu1Part1;
    testItem * psu1Part2;
    testItem * psu1Part3;
    testItem * psu1Part4;
    testItem * psu1Part5;

    // PSU2  ǰ��
    QList<command *> * cmdListPsu2Pre;   // �����б�
    int nowIndexPsu2Pre;                // ��ǰ��������
    command * nowCommandPsu2;           // ��ǰ����ָ��
    testItem * psu2Part1;
    testItem * psu2Part2;
    testItem * psu2Part3;
    testItem * psu2Part4;
    testItem * psu2Part5;

    // PSU1  Part1  ����
    QList<command *> * cmdListPsu1Part1Pre;  // �����б�
    int nowIndexPsu1Part1Pre;           // ��ǰ��������
    command * nowCommandPsu1Part1;      // ��ǰ����ָ��
    // PSU1  Part1  ����
    int nowIndexPsu1Part1Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu1Part1Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part1Data;  // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part1Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part1Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part1; // �����б�
    int dataLengthPsu1Part1;  // ���ݳ���
    // PSU1  Part1  У׼
    command * setCmdPsu1Part1Verify;    // У׼���õ�ѹ����
    int setPsu1Part1Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part1Verify;    // У׼��ȡ��ѹ����
    int dmmPsu1Part1Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part1Verify;  // У׼�����ñ�����
    int meterPsu1Part1Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part1  ����
    command * setCmdPsu1Part1Test;      // �������õ�������
    command * dmmCmdPsu1Part1Test;      // ���Զ�ȡ��������
    command * meterCmdPsu1Part1Test;    // ���Զ����ñ�����
    // PSU1  Part2  ����
    QList<command *> * cmdListPsu1Part2Pre;  // �����б�
    int nowIndexPsu1Part2Pre;           // ��ǰ��������
    command * nowCommandPsu1Part2;      // ��ǰ����ָ��
    // PSU1  Part2  ����
    int nowIndexPsu1Part2Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu1Part2Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part2Data;  // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part2Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part2Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part2; // �����б�
    int dataLengthPsu1Part2;  // ���ݳ���
    // PSU1   Part2  У׼
    command * setCmdPsu1Part2Verify;    // У׼���õ�ѹ����
    int setPsu1Part2Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part2Verify;    // У׼��ȡ��ѹ����
    int dmmPsu1Part2Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part2Verify;  // У׼�����ñ�����
    int meterPsu1Part2Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part2  ����
    command * setCmdPsu1Part2Test;      // �������õ�������
    command * dmmCmdPsu1Part2Test;      // ���Զ�ȡ��������
    command * meterCmdPsu1Part2Test;    // ���Զ����ñ�����
    // PSU1  Part3  ����
    QList<command *> * cmdListPsu1Part3Pre;  // �����б�
    int nowIndexPsu1Part3Pre;           // ��ǰ��������
    command * nowCommandPsu1Part3;      // ��ǰ����ָ��
    // PSU1  Part3  ����
    int nowIndexPsu1Part3Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu1Part3Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part3Data;     // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part3Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part3Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part3; // �����б�
    int dataLengthPsu1Part3;  // ���ݳ���
    // PSU1  Part3  У׼
    command * setCmdPsu1Part3Verify;    // У׼���õ�ѹ����
    int setPsu1Part3Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part3Verify;    // У׼��ȡ��ѹ����
    int dmmPsu1Part3Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part3Verify;  // У׼�����ñ�����
    int meterPsu1Part3Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part3  ����
    command * setCmdPsu1Part3Test;      // �������õ�������
    command * dmmCmdPsu1Part3Test;      // ���Զ�ȡ��������
    command * meterCmdPsu1Part3Test;    // ���Զ����ñ�����
    // PSU1  Part4  ����
    QList<command *> * cmdListPsu1Part4Pre;  // �����б�
    int nowIndexPsu1Part4Pre;           // ��ǰ��������
    command * nowCommandPsu1Part4;      // ��ǰ����ָ��
    // PSU1  Part4  ����
    int nowIndexPsu1Part4Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu1Part4Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part4Data;  // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part4Data;  // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part4Data;  // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part4; // �����б�
    int dataLengthPsu1Part4;  // ���ݳ���
    // PSU1  Part4  У׼
    command * setCmdPsu1Part4Verify;    // У׼���õ�ѹ����
    int setPsu1Part4Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part4Verify;    // У׼��ȡ��ѹ����
    int dmmPsu1Part4Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part4Verify;  // У׼�����ñ�����
    int meterPsu1Part4Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part4  ����
    command * setCmdPsu1Part4Test;      // �������õ�������
    command * dmmCmdPsu1Part4Test;      // ���Զ�ȡ��������
    command * meterCmdPsu1Part4Test;    // ���Զ����ñ�����
    // PSU1  Part5  ����
    QList<command *> * cmdListPsu1Part5Pre;  // �����б�
    int nowIndexPsu1Part5Pre;           // ��ǰ��������
    command * nowCommandPsu1Part5;      // ��ǰ����ָ��
    // PSU1  Part5  ����
    int nowIndexPsu1Part5Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu1Part5Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu1Part5Data;  // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu1Part5Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu1Part5Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu1Part5; // �����б�
    int dataLengthPsu1Part5;  // ���ݳ���
    // PSU1  Part5  У׼
    command * setCmdPsu1Part5Verify;    // У׼���õ�ѹ����
    int setPsu1Part5Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu1Part5Verify;    // У׼��ȡ��ѹ����
    int dmmPsu1Part5Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu1Part5Verify;  // У׼�����ñ�����
    int meterPsu1Part5Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU1  Part5  ����
    command * setCmdPsu1Part5Test;      // �������õ�������
    command * dmmCmdPsu1Part5Test;      // ���Զ�ȡ��������
    command * meterCmdPsu1Part5Test;    // ���Զ����ñ�����

    // PSU2  Part1  ����
    QList<command *> * cmdListPsu2Part1Pre;  // �����б�
    int nowIndexPsu2Part1Pre;           // ��ǰ��������
    command * nowCommandPsu2Part1;      // ��ǰ����ָ��
    // PSU2  Part1  ����
    int nowIndexPsu2Part1Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu2Part1Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part1Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part1Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part1Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part1; // �����б�
    int dataLengthPsu2Part1;            // ���ݳ���
    // PSU2  Part1  У׼
    command * setCmdPsu2Part1Verify;    // У׼���õ�ѹ����
    int setPsu2Part1Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part1Verify;    // У׼��ȡ��ѹ����
    int dmmPsu2Part1Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part1Verify;  // У׼�����ñ�����
    int meterPsu2Part1Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part1  ����
    command * setCmdPsu2Part1Test;      // �������õ�������
    command * dmmCmdPsu2Part1Test;      // ���Զ�ȡ��������
    command * meterCmdPsu2Part1Test;    // ���Զ����ñ�����
    // PSU2  Part2  ����
    QList<command *> * cmdListPsu2Part2Pre;  // �����б�
    int nowIndexPsu2Part2Pre;           // ��ǰ��������
    command * nowCommandPsu2Part2;      // ��ǰ����ָ��
    // PSU2  Part2  ����
    int nowIndexPsu2Part2Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu2Part2Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part2Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part2Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part2Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part2; // �����б�
    int dataLengthPsu2Part2;            // ���ݳ���
    // PSU2   Part2  У׼
    command * setCmdPsu2Part2Verify;    // У׼���õ�ѹ����
    int setPsu2Part2Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part2Verify;    // У׼��ȡ��ѹ����
    int dmmPsu2Part2Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part2Verify;  // У׼�����ñ�����
    int meterPsu2Part2Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part2  ����
    command * setCmdPsu2Part2Test;      // �������õ�������
    command * dmmCmdPsu2Part2Test;      // ���Զ�ȡ��������
    command * meterCmdPsu2Part2Test;    // ���Զ����ñ�����
    // PSU2  Part3  ����
    QList<command *> * cmdListPsu2Part3Pre;  // �����б�
    int nowIndexPsu2Part3Pre;           // ��ǰ��������
    command * nowCommandPsu2Part3;      // ��ǰ����ָ��
    // PSU2  Part3  ����
    int nowIndexPsu2Part3Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu2Part3Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part3Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part3Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part3Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part3; // �����б�
    int dataLengthPsu2Part3;            // ���ݳ���
    // PSU2  Part3  У׼
    command * setCmdPsu2Part3Verify;    // У׼���õ�ѹ����
    int setPsu2Part3Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part3Verify;    // У׼��ȡ��ѹ����
    int dmmPsu2Part3Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part3Verify;  // У׼�����ñ�����
    int meterPsu2Part3Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part3  ����
    command * setCmdPsu2Part3Test;      // �������õ�������
    command * dmmCmdPsu2Part3Test;      // ���Զ�ȡ��������
    command * meterCmdPsu2Part3Test;    // ���Զ����ñ�����
    // PSU2  Part4  ����
    QList<command *> * cmdListPsu2Part4Pre;  // �����б�
    int nowIndexPsu2Part4Pre;           // ��ǰ��������
    command * nowCommandPsu2Part4;      // ��ǰ����ָ��
    // PSU2  Part4  ����
    int nowIndexPsu2Part4Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu2Part4Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part4Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part4Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part4Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part4; // �����б�
    int dataLengthPsu2Part4;            // ���ݳ���
    // PSU2  Part4  У׼
    command * setCmdPsu2Part4Verify;    // У׼���õ�ѹ����
    int setPsu2Part4Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part4Verify;    // У׼��ȡ��ѹ����
    int dmmPsu2Part4Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part4Verify;  // У׼�����ñ�����
    int meterPsu2Part4Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part4  ����
    command * setCmdPsu2Part4Test;      // �������õ�������
    command * dmmCmdPsu2Part4Test;      // ���Զ�ȡ��������
    command * meterCmdPsu2Part4Test;    // ���Զ����ñ�����
    // PSU2  Part5  ����
    QList<command *> * cmdListPsu2Part5Pre;  // �����б�
    int nowIndexPsu2Part5Pre;           // ��ǰ��������
    command * nowCommandPsu2Part5;      // ��ǰ����ָ��
    // PSU2  Part5  ����
    int nowIndexPsu2Part5Data;          // ��ǰ��������
    QList<QFrame *> frameListPsu2Part5Data;  // ���б�
    QList<QCheckBox *> checkBoxListPsu2Part5Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListPsu2Part5Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListPsu2Part5Data; // ��ַ���б�
    QList<QPair<bool, QPair<QString, QString> *> *> * dataAndAddrListPsu2Part5; // �����б�
    int dataLengthPsu2Part5;            // ���ݳ���
    // PSU2  Part5  У׼
    command * setCmdPsu2Part5Verify;    // У׼���õ�ѹ����
    int setPsu2Part5Multi;              // д��eepromʱ�����õ�ѹҪ�Ŵ�ı���
    command * dmmCmdPsu2Part5Verify;    // У׼��ȡ��ѹ����
    int dmmPsu2Part5Multi;              // д��eepromʱ����ȡ��ѹҪ�Ŵ�ı���
    command * meterCmdPsu2Part5Verify;  // У׼�����ñ�����
    int meterPsu2Part5Multi;            // д��eepromʱ�����ñ��ؽ��Ҫ�Ŵ�ı���
    // PSU2  Part5  ����
    command * setCmdPsu2Part5Test;      // �������õ�������
    command * dmmCmdPsu2Part5Test;      // ���Զ�ȡ��������
    command * meterCmdPsu2Part5Test;    // ���Զ����ñ�����
};

#endif // CURDATACONFIG_H
