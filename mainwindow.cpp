#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include "meteraddress.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ��״̬�����������ǩ��������ʾzynq�����ñ�Ľ���״̬
    QLabel * zynqStatus = new QLabel(this);
    zynqStatus->setFrameStyle(QFrame::Box | QFrame::Sunken);
    zynqStatus->setText(tr("zynq�ѶϿ�����"));
    ui->statusBar->addPermanentWidget(zynqStatus);
    QLabel * meterStatus = new QLabel(this);
    meterStatus->setFrameStyle(QFrame::Box | QFrame::Sunken);
    meterStatus->setText(tr("���ñ�������"));
    ui->statusBar->addPermanentWidget(meterStatus);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// �ļ��˵�
// �½������ļ�
void MainWindow::on_actionNew_triggered()
{
    qDebug() << tr("�½������ļ�");
}

// �������ļ�
void MainWindow::on_actionOpen_triggered()
{
    qDebug() << tr("�������ļ�");
}

// �����������ļ�
void MainWindow::on_actionRename_triggered()
{
    qDebug() << tr("�����������ļ�");
}

// �����
void MainWindow::on_actionRecent_triggered()
{
    qDebug() << tr("�����");
}

// ����ini
void MainWindow::on_actionImport_triggered()
{
    qDebug() << tr("����ini");
}

// ����ini
void MainWindow::on_actionExport_triggered()
{
    qDebug() << tr("����ini");
}

// �˳�Ӧ�ó���
void MainWindow::on_actionExit_triggered()
{
    qDebug() << tr("�˳�Ӧ�ó���");
    this->close();
}

// ���ò˵�
// �������ñ��ַ�˿�
void MainWindow::on_actionMeter_triggered()
{
    qDebug() << tr("�������ñ��ַ�˿�");
    QString host = "114.115.181.41";
    int port = 80;
    meterAddress *meterdialog;
    meterdialog = new meterAddress(host, port);
    meterdialog->show();
}

// �����ĸ�ͨ���ĵ�ַ�˿�
void MainWindow::on_actionSlot_triggered()
{
    qDebug() << tr("�����ĸ�ͨ���ĵ�ַ�˿�");
}

// ��ѹУ׼����
void MainWindow::on_actionVoltageData_triggered()
{
    qDebug() << tr("��ѹУ׼����");
}

// ����У׼����
void MainWindow::on_actionCurrentData_triggered()
{
    qDebug() << tr("����У׼����");
}

// �鿴�˵�
// �򿪵�ǰ�����ļ�
void MainWindow::on_actionDataFile_triggered()
{
    qDebug() << tr("�򿪵�ǰ�����ļ�");
}

// �򿪵�ǰ�����ļ���
void MainWindow::on_actionDataDir_triggered()
{
    qDebug() << tr("�򿪵�ǰ�����ļ���");
}

// �򿪵�ǰlog�ļ�
void MainWindow::on_actionLogFile_triggered()
{
    qDebug() << tr("�򿪵�ǰlog�ļ�");
}

// ��log�ļ���
void MainWindow::on_actionLogDir_triggered()
{
    qDebug() << tr("��log�ļ���");
}

// ���ڲ˵�
// �������Ӳ��ƽ̨�
void MainWindow::on_actionHardwareBuild_triggered()
{
    qDebug() << tr("�������Ӳ��ƽ̨�");
}

// �����������У׼����
void MainWindow::on_actionDataConfig_triggered()
{
    qDebug() << tr("�����������У׼����");
}

// ����
void MainWindow::on_actionAbout_triggered()
{
    qDebug() << tr("����");
}
