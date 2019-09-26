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
// 设置命令名称
void command::setName(QString newName)
{
    name = newName;
}
// 设置命令参数
void command::setParam(QString newParam)
{
    param = newParam.split(",");
}
// 保存返回结果
void command::setResult(QString response)
{
    result = response;
}
// 设置截取头
void command::setStart(QString strStart)
{
    start = strStart;
}
// 设置截取尾
void command::setEnd(QString strEnd)
{
    end = strEnd;
}
// 设置浮点数结果
void command::setFloatResult(float result)
{
    floatResult = result;
}

// 截取结果
void command::intercept()
{
    int startIndex, endIndex;
    startIndex = result.indexOf(QRegExp(start)) + start.size();
    endIndex = result.indexOf(QRegExp(end));
    interceptedResult = result.mid(startIndex, endIndex-startIndex);
}
// 设置判等条件
void command::setJudge(QString strJudge)
{
    judgeEqual = strJudge;
}
// 判等
bool command::equalJudge()
{
    intercept();
    return judgeEqual == interceptedResult;
}
// 转换截取后的结果为float
void command::computeFloatResult()
{
    intercept();
    floatResult = interceptedResult.toFloat();
}
// 设置误差范围
void command::setRatio(float fRatio)
{
    ratio = fRatio;
}
// 判断误差
bool command::judgeRatio(float compreValue, float setValue)
{
    if(!floatResult)
        computeFloatResult();
    return (fabsf(floatResult-compreValue))/setValue >= ratio;
}
// 获取命令名称
QString command::getName()
{
    return name;
}
// 获取命令参数
QString command::getParam()
{
    return param.join(",");
}
// 获取带参数的命令全称
QString command::getFullName()
{
    QString paramString = getParam();
    return name + QString("("+paramString+")");
}
// 获取结果
QString command::getResult()
{
    return result;
}
// 获取截取头条件
QString command::getStart()
{
    return start;
}
// 获取截取尾条件
QString command::getEnd()
{
    return end;
}
// 获取判等条件
QString command::getJudge()
{
    return judgeEqual;
}
// 获取浮点数结果
QString command::getFloatResult()
{
    if(!floatResult)
        computeFloatResult();
    return QString("%1").arg(floatResult);
}
// 获取误差率
QString command::getRatio()
{
    return QString("%1").arg(ratio);
}
