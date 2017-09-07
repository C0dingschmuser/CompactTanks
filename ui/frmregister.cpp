#include "frmregister.h"
#include "ui_frmregister.h"

FrmRegister::FrmRegister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmRegister)
{
    ui->setupUi(this);
    wait = false;
}

FrmRegister::~FrmRegister()
{
    delete ui;
}

void FrmRegister::on_btnSend_clicked()
{
    if(check(ui->edtUsername->text(),"Benutzername","|#äöü:.,- ",15,4)) return;
    if(check(ui->edtPassword1->text(),"Passwort","|#äöü ",20,6)) return;
    if(ui->edtPassword1->text()!=ui->edtPassword2->text()) {
        QMessageBox::critical(this,"Fehler","Passwörter stimmen nicht überein!");
        return;
    }
    if(!ui->edtEmail->text().contains("@")||!ui->edtEmail->text().split("@").at(1).contains(".")) {
        QMessageBox::critical(this,"Fehler","Ungültige Email");
        return;
    }
    if(!wait) {
        emit connect();
        emit sendEmail("|-1#"+ui->edtUsername->text()+"#"+ui->edtEmail->text()+"#~");
    } else {
        emit confirm("|-2#"+ui->edtUsername->text()+"#"+ui->edtCode->text()+"#"+ui->edtPassword1->text()+"#~");
    }
}

bool FrmRegister::check(QString checkString, QString field, QString exclude, int length, int minLength)
{
    //|#äöü:.,-
    bool ok = false;
    for(int i=0;i<exclude.size();i++) {
        if(checkString.contains(exclude.at(i))) {
            QMessageBox::critical(this,"Fehler","Feld: "+field+" , Zeichen '"+exclude.at(i)+"' nicht erlaubt!");
            ok = true;
            break;
        }
    }
    if(checkString.length()>length) {
        QMessageBox::critical(this,"Fehler","Feld: "+field+" , Zu lang! (Max: "+QString::number(length)+" Zeichen)");
        ok = true;
    }
    if(checkString.length()<minLength) {
        QMessageBox::critical(this,"Fehler","Feld: "+field+" , Zu kurz! (Min: "+QString::number(minLength)+" Zeichen)");
        ok = true;
    }
    return ok;
}

void FrmRegister::reg(int code)
{
    switch(code) {
        case 2: //email & name ok
            wait = true;
            ui->edtPassword1->setEnabled(false);
            ui->edtPassword2->setEnabled(false);
            ui->edtEmail->setEnabled(false);
            ui->edtUsername->setEnabled(false);
            ui->edtCode->setEnabled(true);
            ui->btnSend->setText("Code bestätigen");
            QMessageBox::information(this,"Info","Ihnen wird nun ein Code per Email gesendet, \nbitte geben Sie diesen zum fortfahren im vorgesehenen Feld ein");
        break;
        case 1:
            QMessageBox::information(this,"Info","Registrierung erfolgreich!");
            this->hide();
        break;
        case 0:
            QMessageBox::critical(this,"Fehler","Falscher Code!");
        break;
        case -1:
            QMessageBox::critical(this,"Fehler","Name bereits registriert!");
        break;
        case -2:
            QMessageBox::critical(this,"Fehler","Email bereits registriert!");
        break;
    }

}

void FrmRegister::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e)
    this->hide();
}
