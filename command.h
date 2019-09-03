#ifndef COMMAND_H
#define COMMAND_H
#include <QString>
#include <QStringList>

class command
{
public:
    command(QString);

    void setName(QString);  // 设置命令名称
    void setParam(QString);  // 设置命令参数
    void setResult(QString);  // 设置返回结果
    void setStart(QString);  // 设置截取起始
    void setEnd(QString);  // 设置截取结尾
    void intercept();  // 从结果中截取
    void setJudge(QString);  // 设置判等用的结果
    bool equalJudge();  // 判断截取后的结果与judgeEqual是否相等
    void computeFloatResult();  // 将截取后的结果转换成浮点数
    void setRatio(float);  // 设置差值比率
    bool judgeRatio(float, float); //判断floatResult与设定值差的绝对值的与设定值的比率,是否大于differenceRatio
    QString getName();  // 获取命令名称
    QString getParam();  // 获取命令参数
    QString getFullName();  // 获取带参数的整个命令
    QString getResult();  // 获取返回结果
    QString getStart();  // 获取截取起始
    QString getEnd();  // 获取截取结尾
    QString getJudge();  // 获取判等用的结果
    QString getRatio();  // 获取差值比率

private:
    QString name;  // 命令名称
    QStringList param;  // 参数
    QString result;  // 命令结果
    QString start;  // 从结果截取的起始位置
    QString end;  // 从结果截取的结束位置
    QString interceptedResult; // 截取后的结果
    QString judgeEqual;  // 判断截取后的结果是否相等
    float floatResult;  // 截取后的结果转化为float后的数据，通常为读回来的电压值或电流值
    float ratio;  // float结果与设定值差值的绝对值，与设定值的比例，后面还有%
};

#endif // COMMAND_H
