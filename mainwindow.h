#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QRadioButton>
#include <QVariantMap>
#include <QAbstractSocket>
#include <QLabel>
#include <QButtonGroup>
#include <QStandardItemModel>
#include <QTimer>
#include "command.h"
#include "testitem.h"
#include "currentitem.h"
#include "qjson4/QJsonObject.h"

class QTcpSocket;

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
    bool eventFilter(QObject *, QEvent *);    // ����¼�
    void repaintTable();                      // �ػ���ͷ
    void updateConsume();                     // ���º�ʱ

    void newMeterConnect();                   // �½����ñ�����
    void meterConnected();                    // ���ñ����ӳɹ�
    void displayMeterError(QAbstractSocket::SocketError);  // ��ʾ���ñ����Ӵ���

    void newZynqConnect();                    // �½�ZYNQ����
    void zynqConnected();                     // ZYNQ���ӳɹ�
    void displayZynqError(QAbstractSocket::SocketError);  // ��ʾzynq���Ӵ���

    void recviceMeter(QString host, int port);  // �����û��趨�����ñ�IP�˿�
    void recviceSlots(QMap<QString, QPair<QString, int> >*);  // �����ü��趨��ͨ������
    void recviceVolParam(testItem *,testItem *);  // ���յ�ѹ���ò���

    void readConfFile(QString name="default.json");  // ��ȡ�����ļ�
    void initConfig(QJsonObject);             // ��ʼ������
    void parseSlots(QJsonObject);  // ��json�ֵ����Ϊͨ���б�
    testItem * parseItem(QJsonObject);        // ��json�ֵ����Ϊ������
    command * parseCmd(QJsonObject);          // ��json�ֵ����Ϊ����


    QVariantMap saveSlots();                  // ����ͨ���б�
    QVariantMap saveTestItem(testItem *);     // ���������
    QVariantList saveCommandList(QList<command *> *);  // ���������б�
    QVariantList saveDataList(QList<QPair<bool, QPair<QString, QString> *> *> *);  // ���������б�
    QVariantMap saveCommand(command *);       // ��������

    void on_radioSlot_clicked(int);              // ͨ����ѡ��
    void on_radioBtnCur_clicked();            // ������ѡ��
    void on_radioBtnVol_clicked();            // ��ѹ��ѡ��
    void on_checkBoxAll_clicked();            // ȫѡ
    void on_checkBoxPart1_clicked();          // Part1
    void on_checkBoxPart2_clicked();          // Part2
    void on_checkBoxPart3_clicked();          // Part3
    void on_checkBoxPart4_clicked();          // Part4
    void on_checkBoxPart5_clicked();          // Part5
    void on_pushBtnStart_clicked();           // ��ʼ��ť
    void on_pushBtnStop_clicked();            // ֹͣ��ť
    void runCompleted();                      // �߳����н���

    bool createFolder(QString);               // ����Ŀ¼
    void statusBarShow(QString);              // ��״̬����ʾ��Ϣ, �����̵߳���
    void setProGressMax(int);                 // ���ù������ܴ�С
    void setProGress(int);                    // ���ù�����
    void showTable(QStringList);              // ��ʾ���

private:
    Ui::MainWindow *ui;
    QStandardItemModel * model;  // ���
    QTimer * myTimer;  // ��ʱ��
    float consume;  // ��ʱ
    QString meterHost;  // ���ñ��ַ
    int meterPort;  // ���ñ�˿�
    QTcpSocket *meterSocket;  //���ñ�socket
    QLabel * meterStatus;  // ״̬�����ñ�״ָ̬ʾ

    QString zynqHost;  // ZYNQ��ַ
    int zynqPort;  // ZYNQ�˿�
    QTcpSocket * zynqSocket;  // ZYNQsocket
    QLabel * zynqStatus;  // ״̬��ZYNQ״ָ̬ʾ

    testItem * itemCh1;
    testItem * itemCh2;
    currentItem * itemPsu1;
    currentItem * itemPsu2;

    QVariantMap config;

    QButtonGroup * slotGroup;
    QList<QRadioButton *> * slotList;
    int currentSlot;  // ��ǰͨ��
    verifyOrTest vot;  //  У׼�����
    voltageOrCurrent voc;  //  ��ѹ�����
    ch currentCh;  //  ch1��ch2
    psu currentPsu;  //  psu1��psu2
    QList<int> * partList;  // ��λ�б�

    QThread * thread;  // У׼���Խ���
    QString logPath;
    QString csvPath;
    QString logFile;
    QString csvFile;
    int curTableLine;  // ��ǰ�����
    bool endResult;  // ���ս��
    int bit;  // ����λ��
};

#endif // MAINWINDOW_H
