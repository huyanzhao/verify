#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <utility>
#include "meteraddress.h"
#include "slotsconfig.h"
#include "voldataconfig.h"

// 通道列表
QMap<QString, QPair<QString, int> > slotsMap;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 在状态栏添加两个标签，用来显示zynq和万用表的接连状态
    QLabel * zynqStatus = new QLabel(this);
    zynqStatus->setFrameStyle(QFrame::Box | QFrame::Sunken);
    zynqStatus->setText(tr("zynq已断开连接"));
    ui->statusBar->addPermanentWidget(zynqStatus);
    QLabel * meterStatus = new QLabel(this);
    meterStatus->setFrameStyle(QFrame::Box | QFrame::Sunken);
    meterStatus->setText(tr("万用表已连接"));
    ui->statusBar->addPermanentWidget(meterStatus);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 文件菜单
// 新建配置文件
void MainWindow::on_actionNew_triggered()
{
    qDebug() << tr("新建配置文件");
}

// 打开配置文件
void MainWindow::on_actionOpen_triggered()
{
    qDebug() << tr("打开配置文件");
}

// 重命名配置文件
void MainWindow::on_actionRename_triggered()
{
    qDebug() << tr("重命名配置文件");
}

// 最近打开
void MainWindow::on_actionRecent_triggered()
{
    qDebug() << tr("最近打开");
}

// 导入ini
void MainWindow::on_actionImport_triggered()
{
    qDebug() << tr("导入ini");
}

// 导出ini
void MainWindow::on_actionExport_triggered()
{
    qDebug() << tr("导出ini");
}

// 退出应用程序
void MainWindow::on_actionExit_triggered()
{
    qDebug() << tr("退出应用程序");
    this->close();
}

// 配置菜单
// 配置万用表地址端口
void MainWindow::on_actionMeter_triggered()
{
    qDebug() << tr("配置万用表地址端口");
    QString host = "114.115.181.41";
    int port = 80;
    meterAddress *meterdialog;
    meterdialog = new meterAddress(host, port);
    connect(meterdialog, SIGNAL(meterConfigDone(QString, int)), this, SLOT(recviceMeter(QString,int)));
    meterdialog->show();
}

// 配置通道的地址端口
void MainWindow::on_actionSlot_triggered()
{
    qDebug() << tr("配置通道的地址端口");
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

// 电压校准数据
void MainWindow::on_actionVoltageData_triggered()
{
    qDebug() << tr("电压校准数据");
    volDataConfig * voldatadialog;
    voldatadialog = new volDataConfig();
    voldatadialog->show();
}

// 电流校准数据
void MainWindow::on_actionCurrentData_triggered()
{
    qDebug() << tr("电流校准数据");
}

// 查看菜单
// 打开当前数据文件
void MainWindow::on_actionDataFile_triggered()
{
    qDebug() << tr("打开当前数据文件");
}

// 打开当前数据文件夹
void MainWindow::on_actionDataDir_triggered()
{
    qDebug() << tr("打开当前数据文件夹");
}

// 打开当前log文件
void MainWindow::on_actionLogFile_triggered()
{
    qDebug() << tr("打开当前log文件");
}

// 打开log文件夹
void MainWindow::on_actionLogDir_triggered()
{
    qDebug() << tr("打开log文件夹");
}

// 关于菜单
// 帮助里的硬件平台搭建
void MainWindow::on_actionHardwareBuild_triggered()
{
    qDebug() << tr("帮助里的硬件平台搭建");
}

// 帮助里的配置校准数据
void MainWindow::on_actionDataConfig_triggered()
{
    qDebug() << tr("帮助里的配置校准数据");
}

// 关于
void MainWindow::on_actionAbout_triggered()
{
    qDebug() << tr("关于");
}

// 接收用户选定的万用表地址和端口
void MainWindow::recviceMeter(QString host, int port)
{
    qDebug() << host << port;
}

// 接收用户设置的通道数据
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
