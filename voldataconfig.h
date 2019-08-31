#ifndef VOLDATACONFIG_H
#define VOLDATACONFIG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
class volDataConfig;
}

class volDataConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit volDataConfig(QWidget *parent = 0);
    ~volDataConfig();
    
private slots:
    void on_pushBtnCh1PreAdd_clicked();

    void on_ch1PreCmdList_clicked(const QModelIndex &index);

private:
    Ui::volDataConfig *ui;
};

#endif // VOLDATACONFIG_H
