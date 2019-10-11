#ifndef CURRENTITEM_H
#define CURRENTITEM_H

#include "testitem.h"
#include "command.h"

class currentItem
{
public:
    currentItem(QList<command *> *, testItem *, testItem *, testItem *, testItem *, testItem *);
    ~currentItem();

    void setPreCmdList(QList<command *> *);  // 设置前置命令
    void setPart1(testItem *);  // 设置part1
    void setPart2(testItem *);  // 设置part2
    void setPart3(testItem *);  // 设置part3
    void setPart4(testItem *);  // 设置part4
    void setPart5(testItem *);  // 设置part5

    QList<command *> * getPreCmdList();  // 获取前置命令
    testItem * getPart1();      // 获取part1
    testItem * getPart2();      // 获取part2
    testItem * getPart3();      // 获取part3
    testItem * getPart4();      // 获取part4
    testItem * getPart5();      // 获取part5

    currentItem * deepcopy();   // 深度复制

private:
    QList<command *> * preCmdList;  // 前置命令
    testItem * part1;  // Part 1
    testItem * part2;  // Part 2
    testItem * part3;  // Part 3
    testItem * part4;  // Part 4
    testItem * part5;  // Part 5
};

#endif // CURRENTITEM_H
