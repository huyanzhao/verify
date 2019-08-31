#ifndef COMMAND_H
#define COMMAND_H
#include <QString>

class command
{
public:
    command(QString);
    ~command();

    void setName(QString);
    QString getName();


private:
    QString name;  // ��������
    QStringList *param;  // ����
    QString result;  // ������
    QString start;  // �ӽ����ȡ����ʼλ��
    QString end;  // �ӽ����ȡ�Ľ���λ��
    QString interceptedResult; // ��ȡ��Ľ��
    QString judgeEqual;  // �жϽ�ȡ��Ľ���Ƿ����
    float floatResult;  // ��ȡ��Ľ��ת��Ϊfloat������ݣ�ͨ��Ϊ�������ĵ�ѹֵ�����ֵ
    float differenceRatio;  // float������趨ֵ��ֵ�ľ���ֵ�����趨ֵ�ı��������滹��%
};

#endif // COMMAND_H
