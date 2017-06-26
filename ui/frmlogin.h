#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class FrmLogin;
}

class FrmLogin : public QWidget
{
    Q_OBJECT

public:
    explicit FrmLogin(QWidget *parent = 0);
    ~FrmLogin();

private slots:
    void on_btnConnect_clicked();

private:
    Ui::FrmLogin *ui;
    bool contains(QString data, QString c);
signals:
    void connectWithData(QString username,QString pw);
};

#endif // FRMLOGIN_H
