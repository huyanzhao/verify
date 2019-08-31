#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <utility>
#include "meteraddress.h"
#include "slotsconfig.h"
#include "voldataconfig.h"

// ͨ���б�
QMap<QString, QPair<QString, int> > slotsMap;

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
    connect(meterdialog, SIGNAL(meterConfigDone(QString, int)), this, SLOT(recviceMeter(QString,int)));
    meterdialog->show();
}

// ����ͨ���ĵ�ַ�˿�
void MainWindow::on_actionSlot_triggered()
{
    qDebug() << tr("����ͨ���ĵ�ַ�˿�");
    QMap<QString, QPair<QString, int> > hosts;
    hosts.insert(QString("slot1"), qMakePair(QString("169.254.1.21"), 7801));
    hosts.insert(QString("slot2"), qMakePair(QString("169.254.1.21"), 7802));
    hosts.insert(QString("slot3"), qMakePair(QString("169.254.1.23"), 7801));
    hosts.insert(QString("slot4"), qMakePair(QString("169.254.1.23"), 7802));
    hosts.insert(QString("slot5"), qMakePair(QString("169.254.1.25"), 7801));
    hosts.insert(QString("slot6"), qMakePair(QString("169.254.1.25"), 7802));

    for(QMap<QString, QPair<QString, int> >::Iterator it = hosts.begin(); it != hosts.end(); it++)
        qDebug() << it.key() << ": " << it.value().first << ":" << it.value().second;

    slotsconfig * slotsdialog;
    slotsdialog = new slotsconfig(&hosts);
    connect(slotsdialog, SIGNAL(slotsConfigDone(QMap<QString,QPair<QString,int> >*)), this, SLOT(recviveSlots(QMap<QString,QPair<QString,int> >*)));
    slotsdialog->show();

}

// ��ѹУ׼����
void MainWindow::on_actionVoltageData_triggered()
{
    qDebug() << tr("��ѹУ׼����");
    volDataConfig * voldatadialog;
    voldatadialog = new volDataConfig();
    voldatadialog->show();
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

// �����û�ѡ�������ñ��ַ�Ͷ˿�
void MainWindow::recviceMeter(QString host, int port)
{
    qDebug() << host << port;
}

// �����û����õ�ͨ������
void MainWindow::recviveSlots(QMap<QString, QPair<QString, int> > *hosts)
{
    qDebug() << "mainWindow";
    for(QMap<QString, QPair<QString, int> >::Iterator it = hosts->begin(); it != hosts->end(); it++)
        qDebug() << it.key() << ": " << it.value().first << ":" << it.value().second;
    qDeleteAll(ui->frameSlot->children());
    slotsMap = *hosts;
    hosts = &slotsMap;
    int headheight, slotheight;
    headheight = ui->frameSlot->height() % hosts->count() / 2;
    slotheight = ui->frameSlot->height() / hosts->count();
    for(int i=0; i != hosts->count(); ++i){
        QString ip;
        ip = hosts->value(QString("slot%1").arg(i+1)).first;
        int port;
        port = hosts->value(QString("slot%1").arg(i+1)).second;
        QRadioButton * slot = new QRadioButton(ui->frameSlot);
        slot->setGeometry(QRect(10, headheight+slotheight*i, 190, slotheight));
        slot->setText(QString("Slot %1    %2:%3").arg(i+1).arg(ip).arg(QString::number(port)));
        slot->setObjectName(QString("slotRadioBtn%1").arg(i+1));
        slot->setStatusTip(QString("slot %1").arg(i+1));
        slot->show();
    }

}
