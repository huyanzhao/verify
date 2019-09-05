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
    bool checkBoxAllSelect();
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
    void handleBatchParams(int, int, int, QString, QString);

private:
    Ui::volDataConfig *ui;
    // ch1ǰ������ҳ
    QList<command *>  cmdListCh1Pre;  // �����б�
    int nowIndexCh1Pre;  // ��ǰ��������
    command * nowCommand;  // ��ǰ����ָ��
    // ch1����ҳ
    int nowIndexCh1Data;  // ��ǰ��������
    QList<QFrame *> frameListCh1Data;  // ���б�
    QList<QCheckBox *> checkBoxListCh1Data; // ��ѡ���б�
    QList<QLineEdit *> dataLineEditListCh1Data; // ���ݿ��б�
    QList<QLineEdit *> addrLineEditListCh1Data; // ��ַ���б�

};

#endif // VOLDATACONFIG_H
