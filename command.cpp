#include "command.h"
#include "QDebug"
#include <QStringList>
#include <QRegExp>
#include <math.h>

command::command(QString nameParam):
    name(nameParam)
{
//    qDebug() << QString("create new command:%1").arg(name);
//    judgeEqual = QString("Done");
}
// ������������
void command::setName(QString newName)
{
    name = newName;
}
// �����������
void command::setParam(QString newParam)
{
    param = newParam.split(",");
}
// ���淵�ؽ��
void command::setResult(QString response)
{
    result = response;
}
// ���ý�ȡͷ
void command::setStart(QString strStart)
{
    start = strStart;
}
// ���ý�ȡβ
void command::setEnd(QString strEnd)
{
    end = strEnd;
}
// ���ø��������
void command::setFloatResult(float result)
{
    floatResult = result;
}

// ��ȡ���
void command::intercept()
{
    int startIndex, endIndex;
    startIndex = result.indexOf(QRegExp(start)) + start.size();
    endIndex = result.indexOf(QRegExp(end));
    interceptedResult = result.mid(startIndex, endIndex-startIndex);
}
// �����е�����
void command::setJudge(QString strJudge)
{
    judgeEqual = strJudge;
}
// �е�
bool command::equalJudge()
{
    intercept();
    return judgeEqual == interceptedResult;
}
// ת����ȡ��Ľ��Ϊfloat
void command::computeFloatResult()
{
    intercept();
    floatResult = interceptedResult.toFloat();
}
// ������Χ
void command::setRatio(float fRatio)
{
    ratio = fRatio;
}
// �ж����
bool command::judgeRatio(float compreValue, float setValue)
{
    if(!floatResult)
        computeFloatResult();
    return (fabsf(floatResult-compreValue))/setValue >= ratio;
}
// ��ȡ��������
QString command::getName()
{
    return name;
}
// ��ȡ�������
QString command::getParam()
{
    return param.join(",");
}
// ��ȡ������������ȫ��
QString command::getFullName()
{
    QString paramString = getParam();
    return name + QString("("+paramString+")");
}
// ��ȡ���
QString command::getResult()
{
    return result;
}
// ��ȡ��ȡͷ����
QString command::getStart()
{
    return start;
}
// ��ȡ��ȡβ����
QString command::getEnd()
{
    return end;
}
// ��ȡ�е�����
QString command::getJudge()
{
    return judgeEqual;
}
// ��ȡ���������
QString command::getFloatResult()
{
    if(!floatResult)
        computeFloatResult();
    return QString("%1").arg(floatResult);
}
// ��ȡ�����
QString command::getRatio()
{
    return QString("%1").arg(ratio);
}
