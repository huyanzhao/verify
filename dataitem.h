#ifndef DATAITEM_H
#define DATAITEM_H
#include <QString>

class DataItem
{
public:
    DataItem(bool, double, QString, QString, QString);

    bool check;
    double data;
    QString dacAddr;
    QString adcAddr;
    QString refAddr;
};

#endif // DATAITEM_H
