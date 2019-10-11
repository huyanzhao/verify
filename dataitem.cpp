#include "dataitem.h"

DataItem::DataItem(bool check, double data, QString dacAddr,
                   QString adcAddr, QString refAddr):
    check(check),
    data(data),
    dacAddr(dacAddr),
    adcAddr(adcAddr),
    refAddr(refAddr)
{
}
