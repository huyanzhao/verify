#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <utility>
#include <QButtonGroup>
#include <QResizeEvent>
#include <QByteArray>
#include <QtNetwork>
#include <QStandardItemModel>
#include <QCoreApplication>
#include <QAbstractItemView>
#include <QFileDialog>
#include <QSettings>
#include "qjson4/QJsonArray.h"
#include "qjson4/QJsonDocument.h"
#include "qjson4/QJsonObject.h"
#include "qjson4/QJsonParseError.h"
#include "qjson4/QJsonValue.h"
#include "qjson4/QJsonParser.h"
#include "qjson4/QJsonRoot.h"
#include "qjson4/QJsonValueRef.h"
#include "meteraddress.h"
#include "slotsconfig.h"
#include "voldataconfig.h"
#include "curdataconfig.h"
#include "testitem.h"
#include "currentitem.h"
#include "mythread.h"
#include "about.h"
#include "paraminfo.h"
#include "platformbuild.h"

// 通道列表
QMap<QString, QPair<QString, int> > slotsMap;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings =new QSettings("setting.ini", QSettings::IniFormat);
    model = new QStandardItemModel(this);
    myTimer = new QTimer(this);
    connect(myTimer, SIGNAL(timeout()), this, SLOT(updateConsume()));
    QTime t = QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    // 初始化万用表地址端品
    meterStatus = new QLabel(this);
    meterStatus->installEventFilter(this);  //安装事件过滤器
    meterStatus->setFrameStyle(QFrame::Box | QFrame::Sunken);
    ui->statusBar->addPermanentWidget(meterStatus);
    meterSocket = new QTcpSocket(this);
    connect(meterSocket, SIGNAL(connected()), this, SLOT(meterConnected()));
    connect(meterSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayMeterError(QAbstractSocket::SocketError)));
    newMeterConnect();

    // 初始化ZYNQ连接
    zynqStatus = new QLabel(this);
    zynqStatus->installEventFilter(this);  // 安装事件过滤器
    zynqStatus->setFrameStyle(QFrame::Box | QFrame::Sunken);
    zynqStatus->setText(tr("ZYNQ未连接"));
    ui->statusBar->addPermanentWidget(zynqStatus);
    zynqSocket = new QTcpSocket(this);

    connect(zynqSocket, SIGNAL(connected()), this, SLOT(zynqConnected()));
    connect(zynqSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayZynqError(QAbstractSocket::SocketError)));

    slotGroup = new QButtonGroup(this);
    currentSlot = 0;
    vot = noneVot;
    voc = noneVoc;
    currentCh = noneCh;
    currentPsu = nonePsu;
    partList = new QList<int>;

    ui->radioBtnCH0->setHidden(true);  // 隐藏多的单选按钮,用来显示CH1和CH2未被选中的状态
    ui->radioBtnPSU0->setHidden(true);  // 隐藏多的单选按钮,用来显示PSU1和PSU2未被选中的状态

    currentPath = QCoreApplication::applicationDirPath();  //获取程序当前运行目录
    confPath = currentPath + "/conf";
    createFolder(confPath);
    settings->beginGroup("METER");
    meterHost = settings->value("IP").toString();
    meterPort = settings->value("PORT").toInt();
    settings->endGroup();
    settings->beginGroup("CONFIG FILE");
    currentConf = settings->value("FILE NAME").toString();
    settings->endGroup();
    readConfFile();  // 读取配置文件
    recviceSlots(&slotsMap);

    this->setWindowTitle(QString(tr("B&P通用电压电流校准测试平台 - %1").arg(currentConf)));

    thread = NULL;
    bit = 1;

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);  // 设置选择行为，以行为单位
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);  // 设置选择模式，选择单行
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers );  // 禁止编辑
}
// 析构
MainWindow::~MainWindow()
{
    delete ui;
}
// 新建配置文件
void MainWindow::on_actionNew_triggered()
{
    QString fileFullName = QFileDialog::getSaveFileName(this,
            QString::fromLocal8Bit("新建配置文件"),
            "./conf/Untitled.json",
            tr("Json Files (*.json)"));
    if(fileFullName == NULL)
        return;
    if(!fileFullName.endsWith(".json")){
        QMessageBox::warning(this, tr("新建配置文件错误"), tr("新建的文件必须是json文件"), QMessageBox::Ok);
        return;
    }
    itemCh1 = NULL;
    itemCh2 = NULL;
    slotsMap.clear();
    qDeleteAll(ui->frameSlot->children());
    qDeleteAll(slotGroup->children());
    itemPsu1 = NULL;
    itemPsu2 = NULL;
    currentConf = fileFullName.mid(fileFullName.lastIndexOf("/")+1);
    on_actionSave_triggered();
    ui->statusBar->showMessage(tr("请依次配置硬件通道、电压校准数据和电流校准数据"));
    this->setWindowTitle(QString(tr("B&P通用电压电流校准测试平台 - %1").arg(currentConf)));
}
// 打开配置文件
void MainWindow::on_actionOpen_triggered()
{
    QString fileFullName = QFileDialog::getOpenFileName(this,
            QString::fromLocal8Bit("打开配置文件"),
            "./conf/default.json",
            tr("Json Files (*.json)"));
    if(fileFullName == NULL)
        return;
    if(!fileFullName.endsWith(".json")){
        QMessageBox::warning(this, tr("打开配置文件错误"), tr("打开的文件必须是json文件"), QMessageBox::Ok);
        return;
    }
    currentConf = fileFullName.mid(fileFullName.lastIndexOf("/")+1);
    slotsMap.clear();
    qDeleteAll(ui->frameSlot->children());
    qDeleteAll(slotGroup->children());
    readConfFile();
    this->setWindowTitle(QString(tr("B&P通用电压电流校准测试平台 - %1").arg(currentConf)));
}
// 保存
void MainWindow::on_actionSave_triggered()
{
    QVariantMap conf;
    if(itemCh1)
        conf.insert("ch1", saveTestItem(itemCh1));
    if(itemCh2)
        conf.insert("ch2", saveTestItem(itemCh2));
    if(!slotsMap.isEmpty())
        conf.insert("slots", saveSlots());
    if(itemPsu1)
        conf.insert("psu1", saveCurTestItem(itemPsu1));
    if(itemPsu2)
        conf.insert("psu2", saveCurTestItem(itemPsu2));
    QJsonDocument  jsonDocument = QJsonDocument::fromVariant(conf);
    if (jsonDocument.isNull())
    {
        QMessageBox::warning(this,"error","conf trans for json failure!");
        return;
    }
    QString fileName(confPath + "/" + currentConf);
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "error", tr("打开配置文件失败!\n保存失败"));
    } else{
        QTextStream textStream(&file);
        QString str = jsonDocument.toJson();
        textStream << str;
        QMessageBox::warning(this, tr("保存成功"),tr("保存配置文件成功!"));
        file.close();
    }
}
// 另存为
void MainWindow::on_actionSaveAs_triggered()
{
    QString fileFullName = QFileDialog::getSaveFileName(this,
            QString::fromLocal8Bit("另存为"),
            "./conf/Untitled.json",
            tr("Json Files (*.json)"));
    if(fileFullName == NULL)
        return;
    if(!fileFullName.endsWith(".json")){
        QMessageBox::warning(this, tr("另存为配置文件错误"), tr("新建的文件必须是json文件"), QMessageBox::Ok);
        return;
    }
    currentConf = fileFullName.mid(fileFullName.lastIndexOf("/")+1);
    this->setWindowTitle(QString(tr("B&P通用电压电流校准测试平台 - %1").arg(currentConf)));
    on_actionSave_triggered();
}
// 重命名配置文件
void MainWindow::on_actionRename_triggered()
{
    QString oldConf = currentConf;
    QString fileFullName = QFileDialog::getSaveFileName(this,
            QString::fromLocal8Bit("重命名"),
            QString("./conf/%1").arg(currentConf),
            tr("Json Files (*.json)"));
    if(fileFullName == NULL)
        return;
    if(!fileFullName.endsWith(".json")){
        QMessageBox::warning(this, tr("重命令配置文件错误"), tr("新建的文件必须是json文件"), QMessageBox::Ok);
        return;
    }
    currentConf = fileFullName.mid(fileFullName.lastIndexOf("/")+1);
    QFile f(confPath + "/" + oldConf);
    f.rename(confPath + "/" + oldConf, fileFullName);
    QMessageBox::information(this, tr("重命名"), QString(tr("重命令成功。新文件名：%1").arg(currentConf)), QMessageBox::Ok);
    this->setWindowTitle(QString(tr("B&P通用电压电流校准测试平台 - %1").arg(currentConf)));
}
// 退出应用程序
void MainWindow::on_actionExit_triggered()
{
    this->close();
}
// 配置万用表地址端口
void MainWindow::on_actionMeter_triggered()
{
    meterAddress *meterdialog;
    meterdialog = new meterAddress(meterHost, meterPort);
    connect(meterdialog, SIGNAL(meterConfigDone(QString, int)), this, SLOT(recviceMeter(QString,int)));
    meterdialog->show();
}
// 配置通道的地址端口
void MainWindow::on_actionSlot_triggered()
{
    slotsconfig * slotsdialog;
    slotsdialog = new slotsconfig(&slotsMap);
    connect(slotsdialog, SIGNAL(slotsConfigDone(QMap<QString,QPair<QString,int> >*)),
            this, SLOT(recviceSlots(QMap<QString,QPair<QString,int> >*)));
    slotsdialog->show();
}
// 电压校准数据
void MainWindow::on_actionVoltageData_triggered()
{
    volDataConfig * voldatadialog;
    voldatadialog = new volDataConfig(itemCh1, itemCh2);
    connect(voldatadialog, SIGNAL(returnTestItem(testItem*,testItem*)), this, SLOT(recviceVolParam(testItem*,testItem*)));
    voldatadialog->show();
}
// 电流校准数据
void MainWindow::on_actionCurrentData_triggered()
{
    curdataconfig * curdatadialog;
    curdatadialog = new curdataconfig(itemPsu1, itemPsu2);
    connect(curdatadialog, SIGNAL(returnTestItem(currentItem*,currentItem*)), this, SLOT(recviceCurParam(currentItem*,currentItem*)));
    curdatadialog->show();
}
// 打开当前数据文件
void MainWindow::on_actionDataFile_triggered()
{
    QFile bfilePath(csvFile);
    if(!bfilePath.exists())
        return;
    QDesktopServices::openUrl(QUrl::fromLocalFile("file:///" + csvFile));
}
// 打开当前数据文件夹
void MainWindow::on_actionDataDir_triggered()
{
    QDir bfilePath(csvPath);
    if(!bfilePath.exists())
        return;
    QDesktopServices::openUrl(QUrl::fromLocalFile("file:///" + csvPath));
}
// 打开当前log文件
void MainWindow::on_actionLogFile_triggered()
{
    QFile bfilePath(logFile);
    if(!bfilePath.exists())
        return;
    QDesktopServices::openUrl(QUrl::fromLocalFile("file:///" + logFile));
}
// 打开log文件夹
void MainWindow::on_actionLogDir_triggered()
{
    QDir bfilePath(logPath);
    if(!bfilePath.exists())
        return;
    QDesktopServices::openUrl(QUrl::fromLocalFile("file:///" + logPath));
}
// 关于菜单
// 帮助里的硬件平台搭建
void MainWindow::on_actionHardwareBuild_triggered()
{
    PlatformBuild * platformbuild = new PlatformBuild();
    platformbuild->show();
}
// 帮助里的配置校准数据
void MainWindow::on_actionDataConfig_triggered()
{
    ParamInfo * paraminfo = new ParamInfo();
    paraminfo->show();
}
// 关于
void MainWindow::on_actionAbout_triggered()
{
    About * about = new About();
    about->show();
}
// 退出事件
void MainWindow::closeEvent(QCloseEvent * event)
{
    // 清空配置文件
    settings->clear();
    settings->beginGroup("METER");
    settings->setValue("IP", meterHost); //调用setValue中键名按字面解析
    settings->setValue("PORT", QString("%1").arg(meterPort));
    settings->endGroup();
    settings->beginGroup("CONFIG FILE");
    settings->setValue("FILE NAME", currentConf);
    settings->endGroup();
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
    ui->labelSecond->resize(width-790-168, 100);
    // 进度条
    ui->frameProgress->resize(width, 30);
    ui->progressBar->resize(width-10, 20);
    // 表格
    ui->frameTable->resize(width, height-175);
    ui->tableView->resize(width-10, height-180);
    repaintTable();
}
// 重画表头
void MainWindow::repaintTable()
{
    int verticalWidth = 8 + bit*7;
    int subWidth = 19 + verticalWidth;
    int totalWidth = ui->tableView->width() - subWidth;
    ui->tableView->verticalHeader()->setFixedWidth(verticalWidth);
    if(vot == verify){
        model->setColumnCount(10);
        if(voc == voltage){
            model->setHeaderData(0,Qt::Horizontal, tr("测试项"));
            model->setHeaderData(1,Qt::Horizontal, tr("设置电压"));
            model->setHeaderData(2,Qt::Horizontal, tr("地址"));
            model->setHeaderData(3,Qt::Horizontal, tr("DMM电压"));
            model->setHeaderData(4,Qt::Horizontal, tr("地址"));
            model->setHeaderData(5,Qt::Horizontal, tr("万用表电压"));
            model->setHeaderData(6,Qt::Horizontal, tr("地址"));
            model->setHeaderData(7,Qt::Horizontal, tr("设置电压 - DMM电压"));
            model->setHeaderData(8,Qt::Horizontal, tr("万用表电压 - DMM电压"));
            model->setHeaderData(9,Qt::Horizontal, tr("结果"));
        } else if(voc == current){
            model->setHeaderData(0,Qt::Horizontal, tr("测试项"));
            model->setHeaderData(1,Qt::Horizontal, tr("设置电流"));
            model->setHeaderData(2,Qt::Horizontal, tr("地址"));
            model->setHeaderData(3,Qt::Horizontal, tr("PSU电流"));
            model->setHeaderData(4,Qt::Horizontal, tr("地址"));
            model->setHeaderData(5,Qt::Horizontal, tr("万用表电流"));
            model->setHeaderData(6,Qt::Horizontal, tr("地址"));
            model->setHeaderData(7,Qt::Horizontal, tr("设置电流 - PSU电流"));
            model->setHeaderData(8,Qt::Horizontal, tr("万用表电流 - PSU电流"));
            model->setHeaderData(9,Qt::Horizontal, tr("结果"));
        }
        ui->tableView->setModel(model);
        int width = totalWidth / 40;
        int surplus = totalWidth - width * 40;
        ui->tableView->setColumnWidth(0, width*8+surplus);
        ui->tableView->setColumnWidth(1, width*4);
        ui->tableView->setColumnWidth(2, width*2);
        ui->tableView->setColumnWidth(3, width*4);
        ui->tableView->setColumnWidth(4, width*2);
        ui->tableView->setColumnWidth(5, width*4);
        ui->tableView->setColumnWidth(6, width*2);
        ui->tableView->setColumnWidth(7, width*6);
        ui->tableView->setColumnWidth(8, width*6);
        ui->tableView->setColumnWidth(9, width*2);
    } else if(vot == test){
        model->setColumnCount(9);
        if(voc == voltage){
            model->setHeaderData(0, Qt::Horizontal, tr("测试项"));
            model->setHeaderData(1, Qt::Horizontal, tr("设置电压"));
            model->setHeaderData(2, Qt::Horizontal, tr("DMM电压"));
            model->setHeaderData(3, Qt::Horizontal, tr("万用表电压"));
            model->setHeaderData(4, Qt::Horizontal, tr("设置电压 - DMM电压"));
            model->setHeaderData(5, Qt::Horizontal, tr("差值与设置电压比率"));
            model->setHeaderData(6, Qt::Horizontal, tr("万用表电压 - DMM电压"));
            model->setHeaderData(7, Qt::Horizontal, tr("差值与设置电压比率"));
            model->setHeaderData(8, Qt::Horizontal, tr("结果"));
        } else if(vot == test && voc == current){
            model->setHeaderData(0, Qt::Horizontal, tr("测试项"));
            model->setHeaderData(1, Qt::Horizontal, tr("设置电流"));
            model->setHeaderData(2, Qt::Horizontal, tr("PSU电流"));
            model->setHeaderData(3, Qt::Horizontal, tr("万用表电流"));
            model->setHeaderData(4, Qt::Horizontal, tr("设置电流 - PSU电流"));
            model->setHeaderData(5, Qt::Horizontal, tr("差值与设置电压比率"));
            model->setHeaderData(6, Qt::Horizontal, tr("万用表电流 - PSU电流"));
            model->setHeaderData(7, Qt::Horizontal, tr("差值与设置电压比率"));
            model->setHeaderData(8, Qt::Horizontal, tr("结果"));
        }
        ui->tableView->setModel(model);
        int width = totalWidth / 42;
        int surplus = totalWidth - width * 42;
        ui->tableView->setColumnWidth(0, width*8+surplus);
        ui->tableView->setColumnWidth(1, width*3);
        ui->tableView->setColumnWidth(2, width*3);
        ui->tableView->setColumnWidth(3, width*3);
        ui->tableView->setColumnWidth(4, width*6);
        ui->tableView->setColumnWidth(5, width*5);
        ui->tableView->setColumnWidth(6, width*6);
        ui->tableView->setColumnWidth(7, width*6);
        ui->tableView->setColumnWidth(8, width*2);
    }
}
// 更新耗时
void MainWindow::updateConsume()
{
    consume += 0.1;
    ui->labelSecond->setText(QString::number(consume,'f',1) + "s");
    if(int(consume * 10) % 10 == 0){
        int r = qrand() % 255;
        int g = qrand() % 255;
        int b = qrand() % 255;
        QPalette palette;
        palette.setColor(QPalette::Background, QColor(r, g, b));
        ui->labelSecond->setAutoFillBackground(true);
        ui->labelSecond->setPalette(palette);
    }
}
// 点击事件
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == meterStatus)  // 万用表状态指示标签
    {
        if (event->type() == QEvent::MouseButtonPress){  // 鼠标按钮被按下
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton){  // 左键
                newMeterConnect();
                return true;
            } else
                return false;
        } else
            return false;
    } else if (obj == zynqStatus){  // ZYNQ状态指示标签
        if (event->type() == QEvent::MouseButtonPress){  // 鼠标按钮被按下
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton){  // 左键
                newZynqConnect();
                return true;
            } else
                return false;
        } else
            return false;
    } else{
        // 主窗口点击事件
        return QMainWindow::eventFilter(obj, event);
    }
}
// 接收用户选定的万用表地址和端口
void MainWindow::recviceMeter(QString host, int port)
{
    meterHost = host;
    meterPort = port;
    newMeterConnect();
}
// 新建万用表连接
void MainWindow::newMeterConnect()
{
    // 取消已有的连接
    meterSocket->abort();
    meterStatus->setText(tr("万用表未连接"));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    meterStatus->setPalette(pe);
    meterSocket->connectToHost(meterHost, meterPort);
}
// 万用表连接成功
void MainWindow::meterConnected()
{
    ui->statusBar->showMessage(tr("万用表连接成功"));
    meterStatus->setText(tr("万用表已连接"));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::blue);
    meterStatus->setPalette(pe);
}
// 万用表连接错误
void MainWindow::displayMeterError(QAbstractSocket::SocketError)
{
    ui->statusBar->showMessage(tr("Meter: ") + meterSocket->errorString());
    if(meterSocket->error() == QAbstractSocket::RemoteHostClosedError){ // 断开连接
        meterStatus->setText(tr("万用表已断开连接！"));
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::red);
        meterStatus->setPalette(pe);
    }
}
// 接收用户设置的通道数据
void MainWindow::recviceSlots(QMap<QString, QPair<QString, int> > *hosts)
{
    qDeleteAll(ui->frameSlot->children());
    slotsMap = *hosts;
    hosts = &slotsMap;
    int headheight, slotheight;
    headheight = ui->frameSlot->height() % hosts->count() / 2;
    slotheight = ui->frameSlot->height() / hosts->count();
    qDeleteAll(slotGroup->children());
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
        slotGroup->addButton(slot, i);
    }
    connect(slotGroup, SIGNAL(buttonClicked(int)), this, SLOT(radioSlot_clicked(int)));
}
// 新建ZYNQ连接
void MainWindow::newZynqConnect()
{
    // 取消已有的连接
    zynqSocket->abort();
    zynqStatus->setText(tr("ZYNQ未连接"));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::black);
    zynqStatus->setPalette(pe);
    zynqSocket->connectToHost(zynqHost, zynqPort);
}
// ZTNQ连接成功
void MainWindow::zynqConnected()
{
    ui->statusBar->showMessage(tr("ZYNQ连接成功"));
    zynqStatus->setText(tr("ZYNQ已连接"));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::blue);
    zynqStatus->setPalette(pe);
}
// ZYNQ连接错误
void MainWindow::displayZynqError(QAbstractSocket::SocketError)
{
    ui->statusBar->showMessage(tr("ZYNQ: ") + zynqSocket->errorString());
    if(zynqSocket->error() == QAbstractSocket::RemoteHostClosedError){ // 断开连接
        zynqStatus->setText(tr("ZYNQ已断开连接！"));
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::red);
        zynqStatus->setPalette(pe);
    }
}
// 接收电压设置参数
void MainWindow::recviceVolParam(testItem * ch1, testItem * ch2)
{
    itemCh1 = ch1;
    itemCh2 = ch2;
}
// 接收电流设置参数
void MainWindow::recviceCurParam(currentItem * psu1, currentItem * psu2)
{
    itemPsu1 = psu1;
    itemPsu2 = psu2;
}
// 读取配置文件
void MainWindow::readConfFile()
{
    QString fileName = confPath + "/" + currentConf;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this,"error", tr("打开文件错误!"));
        return;
    }
    if(!file.isReadable()){
        QMessageBox::warning(this,"error",tr("文件不可读取!"));
        return;
    }
    const QByteArray array = file.readAll();
    file.close();
    QJsonParseError jsonParseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(array, &jsonParseError);
    if(QJsonParseError::NoError != jsonParseError.error)
    {
        QMessageBox::warning(this,"error",QString("JsonParseError: %1").arg(jsonParseError.errorString()));
        return;
    }
    QJsonObject conf = jsonDocument.object();
    initConfig(conf);
}
// 初始化配置
void MainWindow::initConfig(QJsonObject conf)
{
    if(conf.contains("ch1"))
        itemCh1 = parseItem(conf.value("ch1").toObject());
    else
        itemCh1 = NULL;
    if(conf.contains("ch2"))
        itemCh2 = parseItem(conf.value("ch2").toObject());
    else
        itemCh2 = NULL;
    if(conf.contains("slots"))
        parseSlots(conf.value("slots").toObject());
    else
        slotsMap.clear();
    if(conf.contains("psu1"))
        itemPsu1 = parseCurItem(conf.value("psu1").toObject());
    else
        itemPsu1 = NULL;
    if(conf.contains("psu2"))
        itemPsu2 = parseCurItem(conf.value("psu2").toObject());
    else
        itemPsu2 = NULL;
}
// 将json对象解析成通道列表
void MainWindow::parseSlots(QJsonObject json)
{
    slotsMap.clear();
    for(int i=0; i != json.length(); ++i)
    {
        QString key = QString("slot%1").arg(i+1);
        QJsonObject slot = json.value(key).toObject();
        QString host = slot.value("host").toString();
        int port = slot.value("port").toInt();
        slotsMap[QString("slot%1").arg(i+1)] = qMakePair(host, port);
    }
    recviceSlots(&slotsMap);
}
// 将json对象解析成电流测试项
currentItem * MainWindow::parseCurItem(QJsonObject json)
{
    // 解析前置命令
    // 解析前置命令列表
    QList<command *> * preCmdList = new QList<command *>;
    if(json.contains("preCmdList")){
        QJsonArray cmdArray = json.value("preCmdList").toArray();
        for(QJsonArray::iterator it = cmdArray.begin(); it != cmdArray.end(); ++it){
            preCmdList->append(parseCmd(it->toObject()));
        }
    }else
        preCmdList = NULL;
    testItem * part1 = NULL;
    testItem * part2 = NULL;
    testItem * part3 = NULL;
    testItem * part4 = NULL;
    testItem * part5 = NULL;
    if(json.contains("part1")){
        part1 = parseItem(json.value("part1").toObject());
    }
    if(json.contains("part2")){
        part2 = parseItem(json.value("part2").toObject());
    }
    if(json.contains("part3")){
        part3 = parseItem(json.value("part3").toObject());
    }
    if(json.contains("part4")){
        part4 = parseItem(json.value("part4").toObject());
    }
    if(json.contains("part5")){
        part5 = parseItem(json.value("part5").toObject());
    }
    return new currentItem(preCmdList, part1, part2, part3, part4, part5);
}
// 将json对象解析成测试项
testItem * MainWindow::parseItem(QJsonObject json)
{
    if(json.isEmpty())
        return NULL;
    // 解析前置命令列表
    QList<command *> * cmdList = new QList<command *>;
    if(json.contains("cmdList")){
        QJsonArray cmdArray = json.value("cmdList").toArray();
        for(QJsonArray::iterator it = cmdArray.begin(); it != cmdArray.end(); ++it){
            cmdList->append(parseCmd(it->toObject()));
        }
    }
    // 解析数据列表
    QList<DataItem *> * dataList = new QList<DataItem *>;
    if(json.contains("dataList")){
        QJsonArray dataArray = json.value("dataList").toArray();
        for(QJsonArray::iterator it = dataArray.begin(); it != dataArray.end(); ++it){
            QJsonObject dataObj = it->toObject();
            bool check = dataObj.value("check").toBool();
            double data = dataObj.value("data").toDouble();
            QString dacAddr = dataObj.value("dacAddr").toString();
            QString adcAddr = dataObj.value("adcAddr").toString();
            QString refAddr = dataObj.value("refAddr").toString();
            dataList->append(new DataItem(check, data, dacAddr, adcAddr, refAddr));
        }
    }
    int dataLength = 6;
    if(json.contains("dataLength"))
        dataLength = json.value("dataLength").toInt();
    // 解析校准项
    command * setCmdVerify = parseCmd(json.value("setCmdVerify").toObject());
    int setMulti = json.value("setMulti").toInt();
    command * dmmCmdVerify = parseCmd(json.value("dmmCmdVerify").toObject());
    int dmmMulti = json.value("dmmMulti").toInt();
    command * meterCmdVerify = parseCmd(json.value("meterCmdVerify").toObject());
    int meterMulti = json.value("meterMulti").toInt();
    command * setCmdTest = parseCmd(json.value("setCmdTest").toObject());
    command * dmmCmdTest = parseCmd(json.value("dmmCmdTest").toObject());
    command * meterCmdTest = parseCmd(json.value("meterCmdTest").toObject());
    return new testItem(cmdList, dataList, dataLength, setCmdVerify, setMulti, dmmCmdVerify, dmmMulti,
                        meterCmdVerify, meterMulti, setCmdTest, dmmCmdTest, meterCmdTest);
}
// 将json对象解析为命令
command * MainWindow::parseCmd(QJsonObject cmdObj)
{
    QString name = cmdObj.value("name").toString();
    command * cmd = new command(name);
    QJsonArray paramArray = cmdObj.value("param").toArray();
    QStringList paramList;
    for(QJsonArray::iterator iit = paramArray.begin(); iit != paramArray.end(); ++iit){
        QString param = iit->toString();
        paramList.append(param);
    }
    cmd->setParam(paramList.join(","));
    cmd->setStart(cmdObj.value("start").toString());
    cmd->setEnd(cmdObj.value("end").toString());
    cmd->setJudge(cmdObj.value("judge").toString());
    cmd->setRatio(cmdObj.value("ratio").toString().toDouble());
    return cmd;
}
// 保存通道列表
QVariantMap MainWindow::saveSlots()
{
    if(slotsMap.size() == 0)
        return QVariantMap();
    QVariantMap slotMap;
    for(QMap<QString, QPair<QString, int> >::iterator it= slotsMap.begin(); it != slotsMap.end(); ++it){
        QVariantMap slot;
        slot.insert("host", it.value().first);
        slot.insert("port", it.value().second);
        slotMap.insert(it.key(), slot);
    }
    return slotMap;
}
// 保存电流测试项
QVariantMap MainWindow::saveCurTestItem(currentItem * item)
{
    if(item == NULL){
        return QVariantMap();
    }
    QVariantMap psuItem;
    psuItem.insert("preCmdList", saveCommandList(item->getPreCmdList()));
    if(item->getPart1())
        psuItem.insert("part1", saveTestItem(item->getPart1()));
    if(item->getPart2())
        psuItem.insert("part2", saveTestItem(item->getPart2()));
    if(item->getPart3())
        psuItem.insert("part3", saveTestItem(item->getPart3()));
    if(item->getPart4())
        psuItem.insert("part4", saveTestItem(item->getPart4()));
    if(item->getPart5())
        psuItem.insert("part5", saveTestItem(item->getPart5()));
    return psuItem;
}
// 保存电压测试项
QVariantMap MainWindow::saveTestItem(testItem * item)
{
    if(item == NULL){
        return QVariantMap();
    }
    QVariantMap chOrPart;
    chOrPart.insert("cmdList", saveCommandList(item->getCmdList()));
    chOrPart.insert("dataList", saveDataList(item->getDataList()));
    chOrPart.insert("dataLength", item->getDataLength());
    chOrPart.insert("setCmdVerify", saveCommand(item->getSetCmdVerify()));
    chOrPart.insert("setMulti", item->getSetMulti());
    chOrPart.insert("dmmCmdVerify", saveCommand(item->getDmmCmdVerify()));
    chOrPart.insert("dmmMulti", item->getDmmMulti());
    chOrPart.insert("meterCmdVerify", saveCommand(item->getMeterCmdVerify()));
    chOrPart.insert("meterMulti", item->getMeterMulti());
    chOrPart.insert("setCmdTest", saveCommand(item->getSetCmdTest()));
    chOrPart.insert("dmmCmdTest", saveCommand(item->getDmmCmdTest()));
    chOrPart.insert("meterCmdTest", saveCommand(item->getMeterCmdTest()));
    return chOrPart;
}
// 保存命令列表
QVariantList MainWindow::saveCommandList(QList<command *> * cmdList)
{
    QVariantList cmdMapList;
    for(int i=0; i != cmdList->size(); ++i){
        QVariantMap cmd = saveCommand(cmdList->at(i));
        cmdMapList.append(cmd);
    }
    return cmdMapList;
}
// 保存数据列表
QVariantList MainWindow::saveDataList(QList<DataItem *> * dataList)
{
    QVariantList dataPairList;
    for(int i=0; i != dataList->size(); ++i){
        QVariantMap dataPair;
        dataPair.insert("check", dataList->at(i)->check);
        dataPair.insert("data", dataList->at(i)->data);
        dataPair.insert("dacAddr", dataList->at(i)->dacAddr);
        dataPair.insert("dacAddr", dataList->at(i)->adcAddr);
        dataPair.insert("dacAddr", dataList->at(i)->refAddr);
        dataPairList.append(dataPair);
    }
    return dataPairList;
}
// 保存命令
QVariantMap MainWindow::saveCommand(command * cmd)
{
    QVariantMap cmdMap;
    cmdMap.insert("name", cmd->getName());
    QStringList params = cmd->getParam().split(',');
    QVariantList paramList;
    for(int j=0; j != params.size(); ++j){
        paramList.append(params.at(j));
    }
    cmdMap.insert("param", paramList);
    cmdMap.insert("start", cmd->getStart());
    cmdMap.insert("end", cmd->getEnd());
    cmdMap.insert("judge", cmd->getJudge());
    cmdMap.insert("ratio", cmd->getRatio());
    return cmdMap;
}
// 通道单选框
void MainWindow::radioSlot_clicked(int id)
{
    QString key = QString("slot%1").arg(id+1);
    QPair<QString, int> hostPort = slotsMap.value(key);
    zynqHost = hostPort.first;
    zynqPort = hostPort.second;
    newZynqConnect();
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
// 开始按钮
void MainWindow::on_pushBtnStart_clicked()
{
    getParameters();
    if(currentSlot == -1 || vot == 0 || voc == 0)
        return;
    curTableLine = 0;
    model->clear();
    repaintTable();
    endResult = true;    
    QDateTime local(QDateTime::currentDateTime());
    QString date = local.toString("yyyyMMdd");
    QString time = local.toString("hhmmss");
    logPath = currentPath + "/log/" + date;
    csvPath = currentPath + "/data/" + date;
    QString Str;
    if(voc == voltage){
        testItem * ch;
        if(ui->radioBtnCH1->isChecked()){
            if(itemCh1 == NULL){
                ui->statusBar->showMessage(tr("没有CH1的校准测试数据"));
                return;
            } else{
                ch = itemCh1;
                Str = "CH1";
            }
        } else if(ui->radioBtnCH2->isChecked()){
            if(itemCh2 == NULL){
                ui->statusBar->showMessage(tr("没有CH2的校准测试数据"));
                return;
            } else{
                ch = itemCh2;
                Str = "CH2";
            }
        }
        consume = 0.0;
        myTimer->start(100);
        createFolder(logPath);
        createFolder(csvPath);
        ui->actionDataDir->setEnabled(true);
        ui->actionLogDir->setEnabled(true);
        ui->actionDataFile->setEnabled(true);
        ui->actionLogFile->setEnabled(true);
        if(vot == verify){
            logFile = logPath + "/" + time + "-" + Str + "-verify.log";
            csvFile = csvPath + "/" + time + "-" + Str + "-verify.csv";
            thread = new verifyVoltageThread(ch, meterSocket, zynqSocket, logFile, csvFile);
            connect(thread, SIGNAL(statusBarShow(QString)), this, SLOT(statusBarShow(QString)));
            connect(thread, SIGNAL(setProgressMaxSize(int)), this, SLOT(setProGressMax(int)));
            connect(thread, SIGNAL(finished()), this, SLOT(runCompleted()));
            connect(thread, SIGNAL(setProgressCurSize(int)), this, SLOT(setProGress(int)));
            connect(thread, SIGNAL(showTable(QStringList)), this, SLOT(showTable(QStringList)));
            thread->start();
        } else if(vot == test){
            logFile = logPath + '/' + time + '-' + Str + '-test.log';
            csvFile = csvPath + '/' + time + '-' + Str + '-test.csv';
            thread = new testVoltageThread(ch, meterSocket, zynqSocket, logFile, csvFile);
            connect(thread, SIGNAL(statusBarShow(QString)), this, SLOT(statusBarShow(QString)));
            connect(thread, SIGNAL(setProgressMaxSize(int)), this, SLOT(setProGressMax(int)));
            connect(thread, SIGNAL(finished()), this, SLOT(runCompleted()));
            connect(thread, SIGNAL(setProgressCurSize(int)), this, SLOT(setProGress(int)));
            connect(thread, SIGNAL(showTable(QStringList)), this, SLOT(showTable(QStringList)));
            thread->start();
        }
    } else if(voc == current){
        currentItem * psu;
        if(ui->radioBtnPSU1->isChecked()){
            if(itemPsu1 == NULL){
                ui->statusBar->showMessage(tr("没有PSU1的校准测试数据"));
                return;
            } else{
                psu = itemPsu1;
                Str = "PSU1";
            }
        } else if(ui->radioBtnPSU2->isChecked()){
            if(itemPsu2 == NULL){
                ui->statusBar->showMessage(tr("没有PSU2的校准测试数据"));
                return;
            } else{
                psu = itemPsu2;
                Str = "PSU2";
            }
        }
        consume = 0.0;
        myTimer->start(100);
        createFolder(logPath);
        createFolder(csvPath);
        ui->actionDataDir->setEnabled(true);
        ui->actionLogDir->setEnabled(true);
        ui->actionDataFile->setEnabled(true);
        ui->actionLogFile->setEnabled(true);
        if(vot == verify){
            logFile = logPath + "/" + time + "-" + Str + "-verify.log";
            csvFile = csvPath + "/" + time + "-" + Str + "-verify.csv";
            thread = new verifyCurrentThread(psu, partList, Str, meterSocket, zynqSocket, logFile, csvFile);
            connect(thread, SIGNAL(statusBarShow(QString)), this, SLOT(statusBarShow(QString)));
            connect(thread, SIGNAL(setProgressMaxSize(int)), this, SLOT(setProGressMax(int)));
            connect(thread, SIGNAL(finished()), this, SLOT(runCompleted()));
            connect(thread, SIGNAL(setProgressCurSize(int)), this, SLOT(setProGress(int)));
            connect(thread, SIGNAL(showTable(QStringList)), this, SLOT(showTable(QStringList)));
            thread->start();
        } else if(vot == test){
            logFile = logPath + '/' + time + '-' + Str + '-test.log';
            csvFile = csvPath + '/' + time + '-' + Str + '-test.csv';
            thread = new testCurrentThread(psu, partList, Str, meterSocket, zynqSocket, logFile, csvFile);
            connect(thread, SIGNAL(statusBarShow(QString)), this, SLOT(statusBarShow(QString)));
            connect(thread, SIGNAL(setProgressMaxSize(int)), this, SLOT(setProGressMax(int)));
            connect(thread, SIGNAL(finished()), this, SLOT(runCompleted()));
            connect(thread, SIGNAL(setProgressCurSize(int)), this, SLOT(setProGress(int)));
            connect(thread, SIGNAL(showTable(QStringList)), this, SLOT(showTable(QStringList)));
            thread->start();
        }
    }
    ui->pushBtnStart->setEnabled(false);
    ui->pushBtnStop->setEnabled(true);
}
// 获取参数
void MainWindow::getParameters()
{
    currentSlot = slotGroup->checkedId();
    if(ui->radioBtnVerify->isChecked()){
        vot = verify;
    }else if(ui->radioBtnTest->isChecked()){
        vot = test;
    }else{
        vot = noneVot;
    }
    if(ui->radioBtnVol->isChecked()){
        voc = voltage;
    }else if(ui->radioBtnCur->isChecked()){
        voc = current;
    }else {
        voc = noneVoc;
    }
    if(ui->radioBtnCH1->isChecked()){
        currentCh = ch1;
    }else if(ui->radioBtnCH2->isChecked()){
        currentCh = ch2;
    }else {
        currentCh = noneCh;
    }
    if(ui->radioBtnPSU1->isChecked()){
        currentPsu = psu1;
    }else if(ui->radioBtnPSU2->isChecked()){
        currentPsu = psu2;
    }else {
        currentPsu = nonePsu;
    }
    partList->clear();
    if(ui->checkBoxPart1->isChecked()){
        partList->append(1);
    }
    if(ui->checkBoxPart2->isChecked()){
        partList->append(2);
    }
    if(ui->checkBoxPart3->isChecked()){
        partList->append(3);
    }
    if(ui->checkBoxPart4->isChecked()){
        partList->append(4);
    }
    if(ui->checkBoxPart5->isChecked()){
        partList->append(5);
    }
}
// 停止按钮
void MainWindow::on_pushBtnStop_clicked()
{
    runCompleted();
    if(thread){
        thread->terminate();
        thread->wait();
    }
}
// 线程运行结束
void MainWindow::runCompleted()
{
    myTimer->stop();
    QPalette palette;
    if(endResult){
        palette.setColor(QPalette::Background, QColor(0, 255, 0));
        ui->labelSecond->setText("Pass\n" + QString::number(consume,'f',1) + "s");
    }else{
        palette.setColor(QPalette::Background, QColor(255, 0, 0));
        ui->labelSecond->setText("Fail\n" + QString::number(consume,'f',1) + "s");
    }
    ui->labelSecond->setAutoFillBackground(true);
    ui->labelSecond->setPalette(palette);
    ui->pushBtnStart->setEnabled(true);
    ui->pushBtnStop->setEnabled(false);
    ui->tableView->scrollToTop();
}
// 创建目录
bool MainWindow::createFolder(QString path)
{
    // 检查目录是否存在，若不存在则新建
    QDir dir;
    if (!dir.exists(path))
        return dir.mkpath(path);
    return true;
}
// 状态栏显示
void MainWindow::statusBarShow(QString message)
{
    ui->statusBar->showMessage(message);
}
// 设置滚动条最大值
void MainWindow::setProGressMax(int max)
{
    ui->progressBar->setMaximum(max);
}
// 设置滚动条
void MainWindow::setProGress(int value)
{
    ui->progressBar->setValue(value);
}
// 表格中显示结果
void MainWindow::showTable(QStringList result)
{
    bool pass = (result.last() == "pass");
    if(!pass)
        endResult = false;
    for(int i=0; i != result.size(); ++i){
        model->setItem(curTableLine,i,new QStandardItem(result.at(i)));
        if(!pass)
            model->item(curTableLine,i)->setBackground(QBrush(QColor("red")));
    }
    ui->tableView->scrollToBottom();
    if(result.takeLast() == "fail")
    curTableLine++;
    if(curTableLine == 10){
        bit = 2;
        repaintTable();
    }else if(curTableLine == 100){
        bit = 3;
        repaintTable();
    }else if(curTableLine == 1000){
        bit = 4;
        repaintTable();
    }else if(curTableLine == 10000){
        bit = 5;
        repaintTable();
    }else if(curTableLine == 100000){
        bit = 6;
        repaintTable();
    }else if(curTableLine == 1000000){
        bit = 7;
        repaintTable();
    }
}
