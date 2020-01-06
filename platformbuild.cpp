#include "platformbuild.h"
#include "ui_platformbuild.h"

PlatformBuild::PlatformBuild(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlatformBuild)
{
    ui->setupUi(this);
}

PlatformBuild::~PlatformBuild()
{
    delete ui;
}
