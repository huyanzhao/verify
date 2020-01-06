#include "currentitem.h"
#include <QDebug>

currentItem::currentItem(QList<command *> * pList, testItem * p1, testItem * p2,
                         testItem * p3, testItem * p4, testItem * p5):
    preCmdList(pList),
    part1(p1), part2(p2), part3(p3), part4(p4), part5(p5)
{
}
// 析构
currentItem::~currentItem()
{
    // 清空前置命令列表
    for(int i=0; i != preCmdList->size(); ++i){
        command * temp = preCmdList->at(i);
        delete temp;
    }
    preCmdList->clear();
    delete preCmdList;  // 销毁前置命令列表
    delete part1;
    delete part2;
    delete part3;
    delete part4;
    delete part5;
}
// 设置前置命令列表
void currentItem::setPreCmdList(QList<command *> * param)
{
    preCmdList = param;
}
// 设置part 1
void currentItem::setPart1(testItem * param)
{
    part1 = param;
}
// 设置part 2
void currentItem::setPart2(testItem * param)
{
    part2 = param;
}
// 设置part 3
void currentItem::setPart3(testItem * param)
{
    part3 = param;
}
// 设置part 4
void currentItem::setPart4(testItem * param)
{
    part4 = param;
}
// 设置part 5
void currentItem::setPart5(testItem * param)
{
    part5 = param;
}
// 获取前置命令列表
QList<command *> * currentItem::getPreCmdList()
{
    return preCmdList;
}
// 获取part1
testItem * currentItem::getPart1()
{
    return part1;
}
// 获取part2
testItem * currentItem::getPart2()
{
    return part2;
}
// 获取part3
testItem * currentItem::getPart3()
{
    return part3;
}
// 获取part4
testItem * currentItem::getPart4()
{
    return part4;
}
// 获取part5
testItem * currentItem::getPart5()
{
    return part5;
}
// 深度复制
currentItem * currentItem::deepcopy()
{
    QList<command *> * newCmdListPre = new QList<command *>;
    for(int i =0; i != preCmdList->size(); ++i){
        newCmdListPre->append(preCmdList->at(i)->deepcopy());
    }
    testItem * newPart1 = part1->deepcopy();
    testItem * newPart2 = part2->deepcopy();
    testItem * newPart3 = part3->deepcopy();
    testItem * newPart4 = part4->deepcopy();
    testItem * newPart5 = part5->deepcopy();
    return new currentItem(newCmdListPre, newPart1, newPart2, newPart3, newPart4, newPart5);
}
