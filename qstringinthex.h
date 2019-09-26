#ifndef QSTRINGINTHEX_H
#define QSTRINGINTHEX_H
#include <QString>

bool QStringIsBase10Int(QString addr);  // 判断字符串参数是否为十进制整数
bool QStringIsBase16Int(QString addr);  // 判断字符串参数是否为十六进制整数
bool QStringIsInt(QString addr);  // 判断字符串参数是否为整数
int QString2int(QString addr);  // 将字符串参数转换成整数
QString Qstring2hexString(QString addr);  // 将字符串参数转换成16进制整数的字符串
QString int2hexString(int);  // 将10进制整数转换成16进制整数的字符串形式

#endif // QSTRINGINTHEX_H
