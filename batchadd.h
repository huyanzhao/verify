#ifndef BATCHADD_H
#define BATCHADD_H

#include <QDialog>

namespace Ui {
class BatchAdd;
}

class BatchAdd : public QDialog
{
    Q_OBJECT
    
public:
    explicit BatchAdd(QWidget *parent = 0);
    ~BatchAdd();

signals:
    void returnParams(int, double, double, QString, bool);
    
private slots:
    // …˙≥…
    void on_pushBtnGenerate_clicked();
    // ÷ÿ÷√
    void on_pushBtnReset_clicked();

private:
    Ui::BatchAdd *ui;
};

#endif // BATCHADD_H
