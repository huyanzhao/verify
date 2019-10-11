#include "paraminfo.h"
#include "ui_paraminfo.h"

ParamInfo::ParamInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParamInfo)
{
    ui->setupUi(this);
}

ParamInfo::~ParamInfo()
{
    delete ui;
}
