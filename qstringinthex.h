#ifndef QSTRINGINTHEX_H
#define QSTRINGINTHEX_H
#include <QString>

bool QStringIsBase10Int(QString addr);  // �ж��ַ��������Ƿ�Ϊʮ��������
bool QStringIsBase16Int(QString addr);  // �ж��ַ��������Ƿ�Ϊʮ����������
bool QStringIsInt(QString addr);  // �ж��ַ��������Ƿ�Ϊ����
int QString2int(QString addr);  // ���ַ�������ת��������
QString Qstring2hexString(QString addr);  // ���ַ�������ת����16�����������ַ���
QString int2hexString(int);  // ��10��������ת����16�����������ַ�����ʽ

#endif // QSTRINGINTHEX_H
