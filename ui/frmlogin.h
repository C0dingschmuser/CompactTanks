#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTime>
#include <QKeyEvent>
#include <QTextStream>
#include <QDebug>
#include "frmregister.h"

namespace Ui {
class FrmLogin;
}

class FrmLogin : public QWidget
{
    Q_OBJECT

public:
    explicit FrmLogin(QWidget *parent = 0);
    ~FrmLogin();
    void fail();
    void reset();
    void setLogin(bool login);
    void reg(int code);
private slots:
    void on_btnConnect_clicked();
    void on_btnRegister_clicked();
    void on_connect();

private:
    Ui::FrmLogin *ui;
    bool contains(QString data, QString c);
    QFile file;
    bool login;
    void sleep(int ms);
    FrmRegister *registration;
    bool vsync;
    double vol1;
    int vol2;
protected:
    void keyPressEvent(QKeyEvent *event);
signals:
    void connectWithData(QString username,QString pw, double volume, int volume2, bool vsync);
    void sendEmail(QString email);
    void confirm(QString code);
};

#endif // FRMLOGIN_H
