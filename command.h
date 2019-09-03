#ifndef COMMAND_H
#define COMMAND_H
#include <QString>
#include <QStringList>

class command
{
public:
    command(QString);

    void setName(QString);  // ������������
    void setParam(QString);  // �����������
    void setResult(QString);  // ���÷��ؽ��
    void setStart(QString);  // ���ý�ȡ��ʼ
    void setEnd(QString);  // ���ý�ȡ��β
    void intercept();  // �ӽ���н�ȡ
    void setJudge(QString);  // �����е��õĽ��
    bool equalJudge();  // �жϽ�ȡ��Ľ����judgeEqual�Ƿ����
    void computeFloatResult();  // ����ȡ��Ľ��ת���ɸ�����
    void setRatio(float);  // ���ò�ֵ����
    bool judgeRatio(float, float); //�ж�floatResult���趨ֵ��ľ���ֵ�����趨ֵ�ı���,�Ƿ����differenceRatio
    QString getName();  // ��ȡ��������
    QString getParam();  // ��ȡ�������
    QString getFullName();  // ��ȡ����������������
    QString getResult();  // ��ȡ���ؽ��
    QString getStart();  // ��ȡ��ȡ��ʼ
    QString getEnd();  // ��ȡ��ȡ��β
    QString getJudge();  // ��ȡ�е��õĽ��
    QString getRatio();  // ��ȡ��ֵ����

private:
    QString name;  // ��������
    QStringList param;  // ����
    QString result;  // ������
    QString start;  // �ӽ����ȡ����ʼλ��
    QString end;  // �ӽ����ȡ�Ľ���λ��
    QString interceptedResult; // ��ȡ��Ľ��
    QString judgeEqual;  // �жϽ�ȡ��Ľ���Ƿ����
    float floatResult;  // ��ȡ��Ľ��ת��Ϊfloat������ݣ�ͨ��Ϊ�������ĵ�ѹֵ�����ֵ
    float ratio;  // float������趨ֵ��ֵ�ľ���ֵ�����趨ֵ�ı��������滹��%
};

#endif // COMMAND_H
