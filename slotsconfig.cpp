#include "slotsconfig.h"
#include "ui_slotsconfig.h"
#include <QDebug>
#include <QLabel>
#include <QLineEdit>

QMap<QString, QPair<QString, int> > hostMap;
QList<QLineEdit * > IPList;
QList<QLineEdit * > PortList;

slotsconfig::slotsconfig(QMap<QString, QPair<QString, int> > * remoteHosts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::slotsconfig)
{
    qDebug() << "Initialization Channel Configuration Interface";
    ui->setupUi(this);
    hostMap = * remoteHosts;
    hosts = &hostMap;
    rePaint();
}

// 重画通道列表
void slotsconfig::rePaint()
{
    qDeleteAll(ui->groupBox->children());
    IPList.clear();
    PortList.clear();
    // 重置主窗口大小
    this->resize(380, 80+hosts->count()*40);
    // 重置按钮框位置
    ui->frameBtn->setGeometry(QRect(10, 10+hosts->count()*40+20+10, 361, 30));
    // 重置groupBox位置大小
    ui->groupBox->setGeometry(QRect(10, 10, 361, hosts->count()*40+20));
    for(int i=0; i != hosts->count(); ++i){
        QPair<QString, int> host;
        host = hosts->value(QString("slot%1").arg(i+1));
        // 新建通道框
        QFrame * slot = new QFrame(ui->groupBox);
        slot->setGeometry(10, i*40+20, 341, 30);
        slot->setFrameShape(QFrame::Box);
        slot->setFrameShadow(QFrame::Raised);
        slot->show();
        // Slot i+1
        QLabel * labelSlot = new QLabel(slot);
        labelSlot->setGeometry(10, 5, 45, 21);
        QFont * font = new QFont("Arial Unicode MS", 12);
        labelSlot->setText(QString("Slot  %1").arg(i+1));
        labelSlot->setFont(*font);
        labelSlot->show();
        // IP
        QLabel * labelIP = new QLabel(slot);
        labelIP->setGeometry(80, 8, 21, 20);
        labelIP->setText(QString("IP:"));
        labelIP->show();
        // line IP
        QLineEdit * lineEditIP = new QLineEdit(slot);
        lineEditIP->setGeometry(100, 6, 121, 20);
        lineEditIP->setFont(*font);
        lineEditIP->setText(host.first);
        lineEditIP->setObjectName(QString("IP%1").arg(i+1));
        IPList.append(lineEditIP);
        connect(lineEditIP, SIGNAL(textChanged(QString)), this, SLOT(ip_lineedit_modify(QString)));
        lineEditIP->show();
        // Port
        QLabel * labelPort = new QLabel(slot);
        labelPort->setGeometry(240, 10, 31, 16);
        labelPort->setText(QString("Port:"));
        labelPort->show();
        // line Port
        QLineEdit * lineEditPort = new QLineEdit(slot);
        lineEditPort->setGeometry(270, 6, 60, 20);
        lineEditPort->setFont(*font);
        lineEditPort->setText(QString::number(host.second));
        lineEditPort->setValidator(new QIntValidator(1, 65535, this));
        lineEditPort->setObjectName(QString("Port%1").arg(i+1));
        PortList.append(lineEditPort);
        connect(lineEditPort, SIGNAL(textChanged(QString)), this, SLOT(port_lineedit_modify(QString)));
        lineEditPort->show();
    }
}

slotsconfig::~slotsconfig()
{
    delete ui;
}

// 取消
void slotsconfig::on_pushBtnCancel_clicked()
{
    qDebug() << "Cancel";
    this->close();
}

// 添加
void slotsconfig::on_pushBtnAdd_clicked()
{
    qDebug() << "Add" << hosts->count();
    if(hosts->count() < 8){
        hosts->insert(QString("slot%1").arg(hosts->count()+1), qMakePair(QString("127.0.0.1"), 80));
        rePaint();
    }else{
        qDebug() << "The maximum number of channels has been reached, and up to 8 channels can be connected simultaneously.";
    }
}

// 删除
void slotsconfig::on_pushBtnDel_clicked()
{
    qDebug() << "Del" << hosts->count();
    hosts->remove(QString("slot%1").arg(hosts->count()));
    for(QMap<QString, QPair<QString, int> >::Iterator it = hosts->begin(); it != hosts->end(); it++)
        qDebug() << it.key() << ": " << it.value().first << ":" << it.value().second;
    rePaint();
}

// 确定
void slotsconfig::on_pushBtnOk_clicked()
{
    qDebug() << "Ok" << hosts->count();
    for(QMap<QString, QPair<QString, int> >::Iterator it = hosts->begin(); it != hosts->end(); it++){
        QString key;
        key = it.key();
        int slotindex;
        slotindex = key.right(1).toInt();
        QPair<QString, int> tempHost;
        tempHost = qMakePair(IPList.at(slotindex-1)->text(), PortList.at(slotindex-1)->text().toInt());
//        qDebug() << tempHost;
        hostMap[key] = tempHost;
    }
    for(QMap<QString, QPair<QString, int> >::Iterator it = hosts->begin(); it != hosts->end(); it++)
        qDebug() << it.key() << ": " << it.value().first << ":" << it.value().second;
    emit slotsConfigDone(hosts);
    this->close();
}

// 保存修改的端口
void slotsconfig::port_lineedit_modify(QString port)
{
    QLineEdit *plineEdit = qobject_cast<QLineEdit*>(sender());
    int index;
    index = plineEdit->objectName().right(1).toInt();
    hostMap[QString("slot%1").arg(index)] = qMakePair(IPList.at(index-1)->text(), port.toInt());
}

// 保存修改的ip
void slotsconfig::ip_lineedit_modify(QString ip)
{
    QLineEdit *plineEdit = qobject_cast<QLineEdit*>(sender());
    int index;
    index = plineEdit->objectName().right(1).toInt();
    hostMap[QString("slot%1").arg(index)] = qMakePair(ip, PortList.at(index-1)->text().toInt());
}
