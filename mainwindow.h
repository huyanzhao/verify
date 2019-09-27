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

enum verifyOrTest{noneVot, verify, test};  // 校准或测试
enum voltageOrCurrent{noneVoc, voltage, current};  // 电压或电流
enum ch{noneCh, ch1, ch2};  // 电压通道
enum psu{nonePsu, psu1, psu2};  // 电流通道

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getParameters();
    
private slots:
    void on_actionNew_triggered();            // 新建
    void on_actionOpen_triggered();           // 打开
    void on_actionSave_triggered();           // 保存
    void on_actionSaveAs_triggered();         // 另存为
    void on_actionRename_triggered();         // 重命名
    void on_actionRecent_triggered();         // 最近打开
    void on_actionExit_triggered();           // 退出
    void on_actionMeter_triggered();          // 设置万用表
    void on_actionSlot_triggered();           // 设置通道
    void on_actionVoltageData_triggered();    // 配置电压数据
    void on_actionCurrentData_triggered();    // 配置电流数据
    void on_actionDataFile_triggered();       // 打开数据文件
    void on_actionDataDir_triggered();        // 打开数据文件夹
    void on_actionLogFile_triggered();        // 打开log文件
    void on_actionLogDir_triggered();         // 打开log文件夹
    void on_actionHardwareBuild_triggered();  // 硬件平台搭建的帮助信息
    void on_actionDataConfig_triggered();     // 数据配置的帮助信息
    void on_actionAbout_triggered();          // 关于
    void resizeEvent(QResizeEvent *);         // 大小改变
    bool eventFilter(QObject *, QEvent *);    // 点击事件
    void repaintTable();                      // 重画表头
    void updateConsume();                     // 更新耗时

    void newMeterConnect();                   // 新建万用表连接
    void meterConnected();                    // 万用表连接成功
    void displayMeterError(QAbstractSocket::SocketError);  // 显示万用表连接错误

    void newZynqConnect();                    // 新建ZYNQ连接
    void zynqConnected();                     // ZYNQ连接成功
    void displayZynqError(QAbstractSocket::SocketError);  // 显示zynq连接错误

    void recviceMeter(QString host, int port);  // 接收用户设定的万用表IP端口
    void recviceSlots(QMap<QString, QPair<QString, int> >*);  // 接收用记设定的通道数据
    void recviceVolParam(testItem *,testItem *);  // 接收电压设置参数

    void readConfFile(QString name="default.json");  // 读取配置文件
    void initConfig(QJsonObject);             // 初始化配置
    void parseSlots(QJsonObject);  // 将json字典解析为通道列表
    testItem * parseItem(QJsonObject);        // 将json字典解析为测试项
    command * parseCmd(QJsonObject);          // 将json字典解析为命令


    QVariantMap saveSlots();                  // 保存通道列表
    QVariantMap saveTestItem(testItem *);     // 保存测试项
    QVariantList saveCommandList(QList<command *> *);  // 保存命令列表
    QVariantList saveDataList(QList<QPair<bool, QPair<QString, QString> *> *> *);  // 保存数据列表
    QVariantMap saveCommand(command *);       // 保存命令

    void on_radioSlot_clicked(int);              // 通道单选框
    void on_radioBtnCur_clicked();            // 电流单选框
    void on_radioBtnVol_clicked();            // 电压单选框
    void on_checkBoxAll_clicked();            // 全选
    void on_checkBoxPart1_clicked();          // Part1
    void on_checkBoxPart2_clicked();          // Part2
    void on_checkBoxPart3_clicked();          // Part3
    void on_checkBoxPart4_clicked();          // Part4
    void on_checkBoxPart5_clicked();          // Part5
    void on_pushBtnStart_clicked();           // 开始按钮
    void on_pushBtnStop_clicked();            // 停止按钮
    void runCompleted();                      // 线程运行结束

    bool createFolder(QString);               // 创建目录
    void statusBarShow(QString);              // 在状态栏显示信息, 供子线程调用
    void setProGressMax(int);                 // 设置滚动条总大小
    void setProGress(int);                    // 设置滚动条
    void showTable(QStringList);              // 显示表格

private:
    Ui::MainWindow *ui;
    QStandardItemModel * model;  // 表格
    QTimer * myTimer;  // 定时器
    float consume;  // 耗时
    QString meterHost;  // 万用表地址
    int meterPort;  // 万用表端口
    QTcpSocket *meterSocket;  //万用表socket
    QLabel * meterStatus;  // 状态栏万用表状态指示

    QString zynqHost;  // ZYNQ地址
    int zynqPort;  // ZYNQ端口
    QTcpSocket * zynqSocket;  // ZYNQsocket
    QLabel * zynqStatus;  // 状态栏ZYNQ状态指示

    testItem * itemCh1;
    testItem * itemCh2;
    currentItem * itemPsu1;
    currentItem * itemPsu2;

    QVariantMap config;

    QButtonGroup * slotGroup;
    QList<QRadioButton *> * slotList;
    int currentSlot;  // 当前通道
    verifyOrTest vot;  //  校准或测试
    voltageOrCurrent voc;  //  电压或电流
    ch currentCh;  //  ch1或ch2
    psu currentPsu;  //  psu1或psu2
    QList<int> * partList;  // 档位列表

    QThread * thread;  // 校准测试进程
    QString logPath;
    QString csvPath;
    QString logFile;
    QString csvFile;
    int curTableLine;  // 当前表格行
    bool endResult;  // 最终结果
    int bit;  // 序列位数
};

#endif // MAINWINDOW_H
