#ifndef VOLTAGE_H
#define VOLTAGE_H

#include "command.h"

class voltage
{
public:
    voltage();

private:
    QList<command *> cmdList;

};

#endif // VOLTAGE_H
