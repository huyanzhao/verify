#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QRadioButton>
#include <QVariantMap>
#include "command.h"
#include "testitem.h"
#include "currentitem.h"

namespace Ui {
class MainWindow;
}

enum verifyOrTest{noneVot, verify, test};  // У׼�����
enum voltageOrCurrent{noneVoc, voltage, current};  // ��ѹ�����
enum ch{noneCh, ch1, ch2};  // ��ѹͨ��
enum psu{nonePsu, psu1, psu2};  // ����ͨ��

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getParameters();
    
private slots:
    void on_actionNew_triggered();            // �½�
    void on_actionOpen_triggered();           // ��
    void on_actionSave_triggered();           // ����
    void on_actionSaveAs_triggered();         // ���Ϊ
    void on_actionRename_triggered();         // ������
    void on_actionRecent_triggered();         // �����
    void on_actionExit_triggered();           // �˳�
    void on_actionMeter_triggered();          // �������ñ�
    void on_actionSlot_triggered();           // ����ͨ��
    void on_actionVoltageData_triggered();    // ���õ�ѹ����
    void on_actionCurrentData_triggered();    // ���õ�������
    void on_actionDataFile_triggered();       // �������ļ�
    void on_actionDataDir_triggered();        // �������ļ���
    void on_actionLogFile_triggered();        // ��log�ļ�
    void on_actionLogDir_triggered();         // ��log�ļ���
    void on_actionHardwareBuild_triggered();  // Ӳ��ƽ̨��İ�����Ϣ
    void on_actionDataConfig_triggered();     // �������õİ�����Ϣ
    void on_actionAbout_triggered();          // ����
    void resizeEvent(QResizeEvent *);         // ��С�ı�

    void recviceMeter(QString host, int port);  // �����û��趨�����ñ�IP�˿�
    void recviceSlots(QMap<QString, QPair<QString, int> >*);  // �����ü��趨��ͨ������
    void recviceVolParam(testItem *,testItem *);  // ���յ�ѹ���ò���

    void on_radioBtnCur_clicked();            // ������ѡ��
    void on_radioBtnVol_clicked();            // ��ѹ��ѡ��
    void on_checkBoxAll_clicked();            // ȫѡ
    void on_checkBoxPart1_clicked();          // Part1
    void on_checkBoxPart2_clicked();          // Part2
    void on_checkBoxPart3_clicked();          // Part3
    void on_checkBoxPart4_clicked();          // Part4
    void on_checkBoxPart5_clicked();          // Part5


    void on_pushBtnStart_clicked();

private:
    Ui::MainWindow *ui;
    testItem * itemCh1;
    testItem * itemCh2;

    QVariantMap config;

    QList<QRadioButton *> * slotList;
    int currentSlot;  // ��ǰͨ��
    verifyOrTest vot;  //  У׼�����
    voltageOrCurrent voc;  //  ��ѹ�����
    ch currentCh;  //  ch1��ch2
    psu currentPsu;  //  psu1��psu2
    QList<int> * partList;  // ��λ�б�
};

#endif // MAINWINDOW_H
