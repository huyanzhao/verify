#ifndef SLOTSCONFIG_H
#define SLOTSCONFIG_H

#include <QDialog>

namespace Ui {
class slotsconfig;
}

class slotsconfig : public QDialog
{
    Q_OBJECT

public:
    explicit slotsconfig(QMap<QString, QPair<QString, int> >*, QWidget *parent = 0);
    ~slotsconfig();
    void rePaint();  // �ػ�

signals:
    void slotsConfigDone(QMap<QString, QPair<QString, int> >*);  // �������ú��ͨ���б��������
    
private slots:
    void on_pushBtnCancel_clicked();     // ȡ��
    void on_pushBtnAdd_clicked();        // ���
    void on_pushBtnDel_clicked();        // ɾ��
    void on_pushBtnOk_clicked();         // ȷ��
    void port_lineedit_modify(QString);  // �޸Ķ˿�
    void ip_lineedit_modify(QString);    // �޸�ip

private:
    Ui::slotsconfig *ui;
    QMap<QString, QPair<QString, int> > *hosts;
};

#endif // SLOTSCONFIG_H
