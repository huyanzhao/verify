#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // �ڴ����п���ʹ�������ַ���
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    MainWindow w;
    w.show();
    // https://s3.amazonaws.com/psiphon/web/mjr4-p23r-puwl/zh/download.html#direct
    return a.exec();
}
