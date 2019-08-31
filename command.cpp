#include "command.h"
#include "QDebug"
command::command(QString cmdName):
    name(cmdName)
{
    qDebug() << QString("生成新命令，命令名称：%1").arg(name);
}

command::~command()
{

}

void command::setName(QString newName)
{
    name = newName;
}

QString command::getName()
{
    return name;
}
