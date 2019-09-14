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
    void meterConfigDone(QString, int); //�Զ�����ź�
    
private slots:
    void on_pushBtnTest_clicked();    // ����
    void on_pushBtnCancel_clicked();  // ȡ��
    void onConnected();               // ����
    void disconnect();                // �Ͽ�
    void on_pushBtnOk_clicked();      // ȷ��

private:
    Ui::meterAddress *ui;
    QString host;           // IP
    int port;               // �˿�
    QTcpSocket *tcpSocket;  // socket
};

#endif // METERADDRESS_H
