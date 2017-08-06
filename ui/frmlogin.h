#ifndef FRMLOGIN_H
#define FRMLOGIN_H

#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTime>
#include <QTextStream>

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
private slots:
    void on_btnConnect_clicked();
    void on_sliderVolume_sliderMoved(int position);

private:
    Ui::FrmLogin *ui;
    bool contains(QString data, QString c);
    QFile file;
    bool login;
    void sleep(int ms);
signals:
    void connectWithData(QString username,QString pw, double volume);
};

#endif // FRMLOGIN_H
