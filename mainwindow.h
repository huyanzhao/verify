#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include "command.h"
#include "testitem.h"
#include "currentitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
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

    void recviceMeter(QString host, int port);  // 接收用户设定的万用表IP端口
    void recviceSlots(QMap<QString, QPair<QString, int> >*);  // 接收用记设定的通道数据
    void recviceVolParam(testItem *,testItem *);  // 接收电压设置参数

    void on_radioBtnCur_clicked();            // 电流单选框
    void on_radioBtnVol_clicked();            // 电压单选框
    void on_checkBoxAll_clicked();            // 全选
    void on_checkBoxPart1_clicked();          // Part1
    void on_checkBoxPart2_clicked();          // Part2
    void on_checkBoxPart3_clicked();          // Part3
    void on_checkBoxPart4_clicked();          // Part4
    void on_checkBoxPart5_clicked();          // Part5


private:
    Ui::MainWindow *ui;
    testItem * ch1;
    testItem * ch2;
};

#endif // MAINWINDOW_H
