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
    QString name;  // 命令名称
    QStringList *param;  // 参数
    QString result;  // 命令结果
    QString start;  // 从结果截取的起始位置
    QString end;  // 从结果截取的结束位置
    QString interceptedResult; // 截取后的结果
    QString judgeEqual;  // 判断截取后的结果是否相等
    float floatResult;  // 截取后的结果转化为float后的数据，通常为读回来的电压值或电流值
    float differenceRatio;  // float结果与设定值差值的绝对值，与设定值的比例，后面还有%
};

#endif // COMMAND_H
