#include "voldataconfig.h"
#include "ui_voldataconfig.h"
#include "command.h"
#include <QDebug>

volDataConfig::volDataConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::volDataConfig)
{
    ui->setupUi(this);
}

volDataConfig::~volDataConfig()
{
    delete ui;
}

void volDataConfig::on_pushBtnCh1PreAdd_clicked()
{
    command * newCommand = new command(QString("default"));
    ui->ch1PreCmdList->addItem(newCommand->getName());
}

void volDataConfig::on_ch1PreCmdList_clicked(const QModelIndex &index)
{
    qDebug() << index;
    qDebug() << index.data().toString();
}
