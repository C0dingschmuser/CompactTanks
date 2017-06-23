#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QWidget>

namespace Ui {
class FrmLogin;
}

class FrmLogin : public QWidget
{
    Q_OBJECT

public:
    explicit FrmLogin(QWidget *parent = 0);
    ~FrmLogin();

private:
    Ui::FrmLogin *ui;
};

#endif // FRMLOGIN_H
