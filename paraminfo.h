#ifndef PARAMINFO_H
#define PARAMINFO_H

#include <QDialog>

namespace Ui {
class ParamInfo;
}

class ParamInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParamInfo(QWidget *parent = 0);
    ~ParamInfo();
    
private:
    Ui::ParamInfo *ui;
};

#endif // PARAMINFO_H
