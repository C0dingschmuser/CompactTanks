#ifndef FRMREGISTER_H
#define FRMREGISTER_H

#include <QWidget>
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class FrmRegister;
}

class FrmRegister : public QWidget
{
    Q_OBJECT

public:
    explicit FrmRegister(QWidget *parent = 0);
    ~FrmRegister();
    void reg(int code);
private slots:
    void on_btnSend_clicked();
private:
    Ui::FrmRegister *ui;
    bool wait;
    bool check(QString checkString, QString field, QString exclude, int length, int minLength);
protected:
    void closeEvent(QCloseEvent *e);

signals:
    void sendEmail(QString email);
    void confirm(QString code);
    void connect();
};

#endif // FRMREGISTER_H
