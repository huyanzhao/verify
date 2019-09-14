#ifndef QSTRINGINTHEX_H
#define QSTRINGINTHEX_H
#include <QString>

bool QStringIsBase10Int(QString addr);  // 判断字符串参数是否为十进制整数
bool QStringIsBase16Int(QString addr);  // 判断字符串参数是否为十六进制整数
bool QStringIsInt(QString addr);  // 判断字符串参数是否为整数
int QString2int(QString addr);  // 将字符串参数转换成整数

#endif // QSTRINGINTHEX_H
