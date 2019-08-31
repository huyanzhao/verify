#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    // 新建
    void on_actionNew_triggered();
    // 打开
    void on_actionOpen_triggered();
    // 重命名
    void on_actionRename_triggered();
    // 最近打开
    void on_actionRecent_triggered();
    // 导入
    void on_actionImport_triggered();
    // 导出
    void on_actionExport_triggered();
    // 退出
    void on_actionExit_triggered();
    // 设置万用表
    void on_actionMeter_triggered();
    // 设置通道
    void on_actionSlot_triggered();
    // 配置电压数据
    void on_actionVoltageData_triggered();
    // 配置电流数据
    void on_actionCurrentData_triggered();
    // 打开数据文件
    void on_actionDataFile_triggered();
    // 打开数据文件夹
    void on_actionDataDir_triggered();
    // 打开log文件
    void on_actionLogFile_triggered();
    // 打开log文件夹
    void on_actionLogDir_triggered();
    // 硬件平台搭建的帮助信息
    void on_actionHardwareBuild_triggered();
    // 数据配置的帮助信息
    void on_actionDataConfig_triggered();
    // 关于
    void on_actionAbout_triggered();

    // 接收用户设定的万用表IP端口
    void recviceMeter(QString host, int port);
    // 接收用记设定的通道数据
    void recviveSlots(QMap<QString, QPair<QString, int> >*);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
