#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <utility>
#include <QButtonGroup>
#include <QResizeEvent>
#include "meteraddress.h"
#include "slotsconfig.h"
#include "voldataconfig.h"
#include "curdataconfig.h"
#include "testitem.h"
#include "currentitem.h"

// 通道列表
QMap<QString, QPair<QString, int> > slotsMap;

MainWindow::MainWindow(QWidget *parent):
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

    ui->radioBtnCH0->setHidden(true);  // 隐藏多的单选按钮,用来显示CH1和CH2未被选中的状态
    ui->radioBtnPSU0->setHidden(true);  // 隐藏多的单选按钮,用来显示PSU1和PSU2未被选中的状态

    ch1 = NULL;
    ch2 = NULL;
}
// 析构
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
// 保存
void MainWindow::on_actionSave_triggered()
{
    qDebug() << "保存配置参数为json文件";
}
// 另存为
void MainWindow::on_actionSaveAs_triggered()
{
    qDebug() << "另存为当前配置";
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

//    for(QMap<QString, QPair<QString, int> >::Iterator it = hosts.begin(); it != hosts.end(); it++)
//        qDebug() << it.key() << ": " << it.value().first << ":" << it.value().second;

    slotsconfig * slotsdialog;
    slotsdialog = new slotsconfig(&hosts);
    connect(slotsdialog, SIGNAL(slotsConfigDone(QMap<QString,QPair<QString,int> >*)),
            this, SLOT(recviceSlots(QMap<QString,QPair<QString,int> >*)));
    slotsdialog->show();

}
// 电压校准数据
void MainWindow::on_actionVoltageData_triggered()
{
    qDebug() << tr("电压校准数据");
    volDataConfig * voldatadialog;
    voldatadialog = new volDataConfig();
    connect(voldatadialog, SIGNAL(returnTestItem(testItem*,testItem*)), this, SLOT(recviceVolParam(testItem*,testItem*)));
    voldatadialog->show();
}
// 电流校准数据
void MainWindow::on_actionCurrentData_triggered()
{
    qDebug() << tr("电流校准数据");
    curdataconfig * curdatadialog;
    curdatadialog = new curdataconfig();
    curdatadialog->show();
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
// 大小改变
void MainWindow::resizeEvent(QResizeEvent * event)
{
    int width, height;
    width = event->size().width();
    height = event->size().height();
    // 顶部框
    ui->frameTop->resize(width, 100);
    ui->frameBtn->move(QPoint(width-190, 0));
    ui->frameOptionAll->move(QPoint(width-790, 0));
    ui->labelSecond->resize(width-790-156, 100);
    // 进度条
    ui->frameProgress->resize(width, 30);
    ui->progressBar->resize(width-10, 20);
    // 表格
    ui->frameTable->resize(width, height-175);
    ui->tableView->resize(width-10, height-180);
}
// 接收用户选定的万用表地址和端口
void MainWindow::recviceMeter(QString host, int port)
{
    qDebug() << host << port;
}
// 接收用户设置的通道数据
void MainWindow::recviceSlots(QMap<QString, QPair<QString, int> > *hosts)
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
// 接收电压设置参数
void MainWindow::recviceVolParam(testItem * ch_1, testItem * ch_2)
{
    ch1 = ch_1;
    ch2 = ch_2;
    qDebug() << "ch1 pre command size: " << ch1->getCmdList()->size();
    qDebug() << "ch1 data size: " << ch1->getDataList()->size();
    qDebug() << "ch1 verify set command: " << ch1->getSetCmdVerify()->getName();
    qDebug() << "ch1 verify set multi: " << ch1->getSetMulti();
    qDebug() << "ch1 verify dmm command: " << ch1->getDmmCmdVerify()->getName();
    qDebug() << "ch1 verify dmm multi: " << ch1->getDmmMulti();
    qDebug() << "ch1 verify meter command: " << ch1->getMeterCmdVerify()->getName();
    qDebug() << "ch1 verify meter multi: " << ch1->getMeterMulti();
    qDebug() << "ch1 test set command: " << ch1->getSetCmdTest()->getName();
    qDebug() << "ch1 test dmm command: " << ch1->getDmmCmdTest()->getName();
    qDebug() << "ch1 test meter command: " << ch1->getMeterCmdTest()->getName();

    qDebug() << "ch2 pre command size: " << ch2->getCmdList()->size();
    qDebug() << "ch2 data size: " << ch2->getDataList()->size();
    qDebug() << "ch2 verify set command: " << ch2->getSetCmdVerify()->getName();
    qDebug() << "ch2 verify set multi: " << ch2->getSetMulti();
    qDebug() << "ch2 verify dmm command: " << ch2->getDmmCmdVerify()->getName();
    qDebug() << "ch2 verify dmm multi: " << ch2->getDmmMulti();
    qDebug() << "ch2 verify meter command: " << ch2->getMeterCmdVerify()->getName();
    qDebug() << "ch2 verify meter multi: " << ch2->getMeterMulti();
    qDebug() << "ch2 test set command: " << ch2->getSetCmdTest()->getName();
    qDebug() << "ch2 test dmm command: " << ch2->getDmmCmdTest()->getName();
    qDebug() << "ch2 test meter command: " << ch2->getMeterCmdTest()->getName();
}
// 电压单选按钮
void MainWindow::on_radioBtnVol_clicked()
{
    ui->radioBtnPSU0->setChecked(true);
    ui->checkBoxPart5->setEnabled(false);
    ui->frameChannel->setEnabled(false);
    ui->frameChannel->setFrameShape(QFrame::WinPanel);
    ui->frameCur->setEnabled(false);
    ui->frameVol->setEnabled(true);
    ui->radioBtnCH1->setEnabled(true);
    ui->radioBtnCH1->setChecked(true);
    ui->radioBtnCH2->setEnabled(true);
    ui->frameChannel->setStatusTip(tr("只有选择了电流和PSU后才可选"));
    ui->checkBoxPart1->setStatusTip(tr(""));
    ui->checkBoxPart2->setStatusTip(tr(""));
    ui->checkBoxPart3->setStatusTip(tr(""));
    ui->checkBoxPart4->setStatusTip(tr(""));
    ui->checkBoxPart5->setStatusTip(tr(""));
    ui->checkBoxAll->setStatusTip(tr(""));
}
// 电流单选按钮
void MainWindow::on_radioBtnCur_clicked()
{
    ui->radioBtnCH0->setChecked(true);
    ui->frameVol->setEnabled(false);
    ui->frameCur->setEnabled(true);
    ui->radioBtnPSU1->setEnabled(true);
    ui->radioBtnPSU1->setChecked(true);
    ui->radioBtnPSU2->setEnabled(true);
    ui->frameChannel->setEnabled(true);
    ui->checkBoxPart5->setEnabled(false);
    ui->frameChannel->setFrameShape(QFrame::NoFrame);
    ui->frameChannel->setStatusTip(tr("电流档位选择"));
    ui->checkBoxPart1->setStatusTip(tr("Part 1"));
    ui->checkBoxPart2->setStatusTip(tr("Part 2"));
    ui->checkBoxPart3->setStatusTip(tr("Part 3"));
    ui->checkBoxPart4->setStatusTip(tr("Part 4"));
    ui->checkBoxPart5->setStatusTip(tr("Part 5"));
    ui->checkBoxAll->setStatusTip(tr("全选"));
}
// 全选
void MainWindow::on_checkBoxAll_clicked()
{
    if(ui->checkBoxAll->checkState()){
        ui->checkBoxPart1->setChecked(true);
        ui->checkBoxPart2->setChecked(true);
        ui->checkBoxPart3->setChecked(true);
        ui->checkBoxPart4->setChecked(true);
    }else{
        ui->checkBoxPart1->setChecked(false);
        ui->checkBoxPart2->setChecked(false);
        ui->checkBoxPart3->setChecked(false);
        ui->checkBoxPart4->setChecked(false);
    }
}
// Part1
void MainWindow::on_checkBoxPart1_clicked()
{
    if(ui->checkBoxPart1->checkState() &&
            ui->checkBoxPart2->checkState() &&
            ui->checkBoxPart3->checkState() &&
            ui->checkBoxPart4->checkState())
        ui->checkBoxAll->setChecked(true);
    else if(!ui->checkBoxPart1->checkState())
        ui->checkBoxAll->setChecked(false);
}
// Part2
void MainWindow::on_checkBoxPart2_clicked()
{
    if(ui->checkBoxPart1->checkState() &&
            ui->checkBoxPart2->checkState() &&
            ui->checkBoxPart3->checkState() &&
            ui->checkBoxPart4->checkState())
        ui->checkBoxAll->setChecked(true);
    else if(!ui->checkBoxPart2->checkState())
        ui->checkBoxAll->setChecked(false);
}
// Part3
void MainWindow::on_checkBoxPart3_clicked()
{
    if(ui->checkBoxPart1->checkState() &&
            ui->checkBoxPart2->checkState() &&
            ui->checkBoxPart3->checkState() &&
            ui->checkBoxPart4->checkState())
        ui->checkBoxAll->setChecked(true);
    else if(!ui->checkBoxPart3->checkState())
        ui->checkBoxAll->setChecked(false);
}
// Part4
void MainWindow::on_checkBoxPart4_clicked()
{
    if(ui->checkBoxPart1->checkState() &&
            ui->checkBoxPart2->checkState() &&
            ui->checkBoxPart3->checkState() &&
            ui->checkBoxPart4->checkState())
        ui->checkBoxAll->setChecked(true);
    else if(!ui->checkBoxPart4->checkState())
        ui->checkBoxAll->setChecked(false);
}
// Part5
void MainWindow::on_checkBoxPart5_clicked()
{
    if(ui->checkBoxPart1->checkState() &&
            ui->checkBoxPart2->checkState() &&
            ui->checkBoxPart3->checkState() &&
            ui->checkBoxPart4->checkState() &&
        ui->checkBoxPart5->checkState())
        ui->checkBoxAll->setChecked(true);
    else if(!ui->checkBoxPart5->checkState())
        ui->checkBoxAll->setChecked(false);
}
