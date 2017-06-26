#include "frmlogin.h"
#include "ui_frmlogin.h"

FrmLogin::FrmLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmLogin)
{
    ui->setupUi(this);
    //this->setWindowFlag(Qt::FramelessWindowHint,true);
    //ui->edtPassword->setEchoMode(QLineEdit::Password);
    //ui->edtPassword->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
}

FrmLogin::~FrmLogin()
{
    delete ui;
}

void FrmLogin::on_btnConnect_clicked()
{
    QString name = ui->edtUsername->text();
    QString pw = ui->edtPassword->text();
    if((contains(name,"|#äöü.,-_<>")||name.length()>8||name=="")||
            (contains(pw,"|#")||pw==""||pw.length()>20)) {
        QMessageBox box;
        box.setText("Falsche Eingabe!");
    } else {
        ui->btnConnect->setText("Verbinde...");
        emit connectWithData(name,pw);
    }
}

bool FrmLogin::contains(QString data,QString c)
{
    bool ok = false;
    for(int i=0;i<c.size();i++) {
        if(data.contains(c.at(i))) {
            ok = true;
        }
    }
    return ok;
}
