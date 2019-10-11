#ifndef CURRENTITEM_H
#define CURRENTITEM_H

#include "testitem.h"
#include "command.h"

class currentItem
{
public:
    currentItem(QList<command *> *, testItem *, testItem *, testItem *, testItem *, testItem *);
    ~currentItem();

    void setPreCmdList(QList<command *> *);  // ����ǰ������
    void setPart1(testItem *);  // ����part1
    void setPart2(testItem *);  // ����part2
    void setPart3(testItem *);  // ����part3
    void setPart4(testItem *);  // ����part4
    void setPart5(testItem *);  // ����part5

    QList<command *> * getPreCmdList();  // ��ȡǰ������
    testItem * getPart1();      // ��ȡpart1
    testItem * getPart2();      // ��ȡpart2
    testItem * getPart3();      // ��ȡpart3
    testItem * getPart4();      // ��ȡpart4
    testItem * getPart5();      // ��ȡpart5

    currentItem * deepcopy();   // ��ȸ���

private:
    QList<command *> * preCmdList;  // ǰ������
    testItem * part1;  // Part 1
    testItem * part2;  // Part 2
    testItem * part3;  // Part 3
    testItem * part4;  // Part 4
    testItem * part5;  // Part 5
};

#endif // CURRENTITEM_H
