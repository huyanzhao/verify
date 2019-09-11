#ifndef QSTRINGINTHEX_H
#define QSTRINGINTHEX_H
#include <QString>

bool QStringIsBase10Int(QString addr);
bool QStringIsBase16Int(QString addr);
bool QStringIsInt(QString addr);
int QString2int(QString addr);

//bool QStringIsBase10Int(QString addr)
//{
//    bool ok;
//    int intAddr;
//    intAddr = addr.toInt(&ok, 10);
//    return ok;
//}

//bool QStringIsBase16Int(QString addr)
//{
//    bool ok;
//    int intAddr;
//    if(addr.startsWith("0x")){
//        intAddr = addr.mid(2).toInt(&ok, 16);
//        return ok;
//    }else{
//        return false;
//    }
//}

//bool QStringIsInt(QString addr)
//{
//    if(QStringIsBase10Int(addr)){
//        return true;
//    }else if(QStringIsBase16Int(addr)){
//        return true;
//    }else{
//        return false;
//    }
//}

//int QString2int(QString addr)
//{
//    bool ok;
//    if(QStringIsBase10Int(addr)){
//        return addr.toInt();
//    }else if(QStringIsBase16Int(addr)){
//        return addr.toInt(&ok, 16);
//    }
//}

#endif // QSTRINGINTHEX_H
