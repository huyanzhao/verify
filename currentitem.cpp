#include "currentitem.h"
#include <QDebug>

currentItem::currentItem(QList<command *> * pList, testItem * p1, testItem * p2,
                         testItem * p3, testItem * p4, testItem * p5):
    preCmdList(pList),
    part1(p1), part2(p2), part3(p3), part4(p4), part5(p5)
{
}
// ����
currentItem::~currentItem()
{
    // ���ǰ�������б�
    for(int i=0; i != preCmdList->size(); ++i){
        command * temp = preCmdList->at(i);
        delete temp;
    }
    preCmdList->clear();
    delete preCmdList;  // ����ǰ�������б�
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
// ��ȸ���
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
