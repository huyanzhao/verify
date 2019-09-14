#ifndef METERADDRESS_H
#define METERADDRESS_H

#include <QDialog>
#include <QAbstractSocket>
class QTcpSocket;

namespace Ui {
class meterAddress;
}

class meterAddress : public QDialog
{
    Q_OBJECT
    
public:
    explicit meterAddress(QString hostIP = "169.254.1.61", int hostPort = 5025, QWidget *parent = 0);
    ~meterAddress();

signals:
    void meterConfigDone(QString, int); //自定义的信号
    
private slots:
    void on_pushBtnTest_clicked();    // 测试
    void on_pushBtnCancel_clicked();  // 取消
    void onConnected();               // 连接
    void disconnect();                // 断开
    void on_pushBtnOk_clicked();      // 确定

private:
    Ui::meterAddress *ui;
    QString host;           // IP
    int port;               // 端口
    QTcpSocket *tcpSocket;  // socket
};

#endif // METERADDRESS_H
