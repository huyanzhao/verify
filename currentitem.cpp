#include "currentitem.h"

currentItem::currentItem(QList<command *> * pList, testItem * p1, testItem * p2,
                         testItem * p3, testItem * p4, testItem * p5):
    preCmdList(pList),
    part1(p1), part2(p2), part3(p3), part4(p4), part5(p5)
{
}
// ����
currentItem::~currentItem()
{
    delete preCmdList;
    delete part1;
    delete part2;
    delete part3;
    delete part4;
    delete part5;
}
// ����ǰ�������б�
void currentItem::setPreCmdList(QList<command *> * param)
{
    preCmdList = param;
}
// ����part 1
void currentItem::setPart1(testItem * param)
{
    part1 = param;
}
// ����part 2
void currentItem::setPart2(testItem * param)
{
    part2 = param;
}
// ����part 3
void currentItem::setPart3(testItem * param)
{
    part3 = param;
}
// ����part 4
void currentItem::setPart4(testItem * param)
{
    part4 = param;
}
// ����part 5
void currentItem::setPart5(testItem * param)
{
    part5 = param;
}
// ��ȡǰ�������б�
QList<command *> * currentItem::getPreCmdList()
{
    return preCmdList;
}
// ��ȡpart1
testItem * currentItem::getPart1()
{
    return part1;
}
// ��ȡpart2
testItem * currentItem::getPart2()
{
    return part2;
}
// ��ȡpart3
testItem * currentItem::getPart3()
{
    return part3;
}
// ��ȡpart4
testItem * currentItem::getPart4()
{
    return part4;
}
// ��ȡpart5
testItem * currentItem::getPart5()
{
    return part5;
}
