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
    // �ػ�
    void rePaint();

signals:
    // �������ú��ͨ���б��������
    void slotsConfigDone(QMap<QString, QPair<QString, int> >*);
    
private slots:
    // ȡ��
    void on_pushBtnCancel_clicked();
    // ���
    void on_pushBtnAdd_clicked();
    // ɾ��
    void on_pushBtnDel_clicked();
    // ȷ��
    void on_pushBtnOk_clicked();
    // �޸Ķ˿�
    void port_lineedit_modify(QString);
    // �޸�ip
    void ip_lineedit_modify(QString);

private:
    Ui::slotsconfig *ui;
    QMap<QString, QPair<QString, int> > *hosts;
};

#endif // SLOTSCONFIG_H
