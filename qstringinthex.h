#ifndef QSTRINGINTHEX_H
#define QSTRINGINTHEX_H
#include <QString>

bool QStringIsBase10Int(QString addr);  // �ж��ַ��������Ƿ�Ϊʮ��������
bool QStringIsBase16Int(QString addr);  // �ж��ַ��������Ƿ�Ϊʮ����������
bool QStringIsInt(QString addr);  // �ж��ַ��������Ƿ�Ϊ����
int QString2int(QString addr);  // ���ַ�������ת��������

#endif // QSTRINGINTHEX_H
