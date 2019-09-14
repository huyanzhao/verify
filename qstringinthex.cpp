#include "qstringinthex.h"
#include <QDebug>
// 判断字符串参数是否为十进制整数
bool QStringIsBase10Int(QString addr)
{
    bool ok;
    int intAddr;
    intAddr = addr.toInt(&ok, 10);
    return ok;
}
// 判断字符串参数是否为十六进制整数
bool QStringIsBase16Int(QString addr)
{
    bool ok;
    int intAddr;
    if(addr.startsWith("0x")){
        intAddr = addr.mid(2).toInt(&ok, 16);
        return ok;
    }else{
        return false;
    }
}
// 判断字符串参数是否为整数
bool QStringIsInt(QString addr)
{
    if(QStringIsBase10Int(addr)){
        return true;
    }else if(QStringIsBase16Int(addr)){
        return true;
    }else{
        return false;
    }
}
// 将字符串参数转换成整数
int QString2int(QString addr)
{
    bool ok;
    if(QStringIsBase10Int(addr)){
        return addr.toInt();
    }else if(QStringIsBase16Int(addr)){
        return addr.toInt(&ok, 16);
    }
}
