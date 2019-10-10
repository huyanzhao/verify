#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 在代码中可以使用中文字符串
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    MainWindow w;
    w.show();
    // https://s3.amazonaws.com/psiphon/web/mjr4-p23r-puwl/zh/download.html#direct
    return a.exec();
}
