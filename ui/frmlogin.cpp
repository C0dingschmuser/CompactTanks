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
    QString path = qApp->applicationDirPath();
    path.append("/login.dat");
    file.setFileName(path);
    if(file.exists()) {
        ui->edtPassword->setFocus();
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        ui->edtUsername->setText(in.readLine());
    } else {
        file.open(QIODevice::ReadWrite);
    }
    file.close();
}

FrmLogin::~FrmLogin()
{
    file.close();
    delete ui;
}

void FrmLogin::on_btnConnect_clicked()
{
    QString name = ui->edtUsername->text();
    QString pw = ui->edtPassword->text();
    if((contains(name,"|#äöü.,-")||name.length()>15||name=="")||
            (contains(pw,"|#")||pw==""||pw.length()>20)) {
        QMessageBox::critical(this,"FEHLER","Falsche Eingabe!");
    } else {
        file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        QTextStream out(&file);
        out << name << endl;
        file.close();
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
