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
    void dlgReturn(QString, int); //自定义的信号
    
private slots:
    void on_pushBtnTest_clicked();

    void on_pushBtnCancel_clicked();
    void onConnected();
    void disconnect();

    void on_pushBtnOk_clicked();

private:
    Ui::meterAddress *ui;
    QString host;
    int port;
    QTcpSocket *tcpSocket;
};

#endif // METERADDRESS_H
