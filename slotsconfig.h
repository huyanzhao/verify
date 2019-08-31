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
    // 重画
    void rePaint();

signals:
    // 返回配置后的通道列表给主窗口
    void slotsConfigDone(QMap<QString, QPair<QString, int> >*);
    
private slots:
    // 取消
    void on_pushBtnCancel_clicked();
    // 添加
    void on_pushBtnAdd_clicked();
    // 删除
    void on_pushBtnDel_clicked();
    // 确定
    void on_pushBtnOk_clicked();
    // 修改端口
    void port_lineedit_modify(QString);
    // 修改ip
    void ip_lineedit_modify(QString);

private:
    Ui::slotsconfig *ui;
    QMap<QString, QPair<QString, int> > *hosts;
};

#endif // SLOTSCONFIG_H
