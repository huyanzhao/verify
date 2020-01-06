#ifndef PLATFORMBUILD_H
#define PLATFORMBUILD_H

#include <QDialog>

namespace Ui {
class PlatformBuild;
}

class PlatformBuild : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlatformBuild(QWidget *parent = 0);
    ~PlatformBuild();
    
private:
    Ui::PlatformBuild *ui;
};

#endif // PLATFORMBUILD_H
