#include "qstringinthex.h"
#include <QDebug>
// �ж��ַ��������Ƿ�Ϊʮ��������
bool QStringIsBase10Int(QString addr)
{
    bool ok;
    int intAddr;
    intAddr = addr.toInt(&ok, 10);
    return ok;
}
// �ж��ַ��������Ƿ�Ϊʮ����������
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
// �ж��ַ��������Ƿ�Ϊ����
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
// ���ַ�������ת��������
int QString2int(QString addr)
{
    bool ok;
    if(QStringIsBase10Int(addr)){
        return addr.toInt();
    }else if(QStringIsBase16Int(addr)){
        return addr.toInt(&ok, 16);
    }
}
