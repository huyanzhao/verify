#include "command.h"
#include "QDebug"
command::command(QString cmdName):
    name(cmdName)
{
    qDebug() << QString("����������������ƣ�%1").arg(name);
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
