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
    // �½�
    void on_actionNew_triggered();
    // ��
    void on_actionOpen_triggered();
    // ������
    void on_actionRename_triggered();
    // �����
    void on_actionRecent_triggered();
    // ����
    void on_actionImport_triggered();
    // ����
    void on_actionExport_triggered();
    // �˳�
    void on_actionExit_triggered();
    // �������ñ�
    void on_actionMeter_triggered();
    // ����ͨ��
    void on_actionSlot_triggered();
    // ���õ�ѹ����
    void on_actionVoltageData_triggered();
    // ���õ�������
    void on_actionCurrentData_triggered();
    // �������ļ�
    void on_actionDataFile_triggered();
    // �������ļ���
    void on_actionDataDir_triggered();
    // ��log�ļ�
    void on_actionLogFile_triggered();
    // ��log�ļ���
    void on_actionLogDir_triggered();
    // Ӳ��ƽ̨��İ�����Ϣ
    void on_actionHardwareBuild_triggered();
    // �������õİ�����Ϣ
    void on_actionDataConfig_triggered();
    // ����
    void on_actionAbout_triggered();

    // �����û��趨�����ñ�IP�˿�
    void recviceMeter(QString host, int port);
    // �����ü��趨��ͨ������
    void recviveSlots(QMap<QString, QPair<QString, int> >*);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
