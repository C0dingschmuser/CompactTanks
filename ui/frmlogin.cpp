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
    login = false;
}

FrmLogin::~FrmLogin()
{
    file.close();
    delete ui;
}

void FrmLogin::on_btnConnect_clicked()
{
    login = true;
    ui->btnConnect->setText("Verbinde...");
    ui->btnConnect->repaint();
    qApp->processEvents();
    QString name = ui->edtUsername->text();
    if(name.size()<4) {
        QMessageBox::critical(this,"FEHLER","Name zu kurz!");
        reset();
        return;
    }
    QString pw = ui->edtPassword->text();
    if((contains(name,"|#äöü:.,-")||name.length()>15||name=="")||
            (contains(pw,"|#:")||pw==""||pw.length()>20)) {
        QMessageBox::critical(this,"FEHLER","Falsche Eingabe!");
    } else {
        file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);
        QTextStream out(&file);
        out << name << endl;
        file.close();
        double vol = (double)ui->sliderVolume->value()/100;
        //while(login) {
            emit connectWithData(name,pw,vol);
            //sleep(1000)
            //qApp->processEvents();
        //}
    }
}

void FrmLogin::sleep(int ms)
{
    QTime dieTime = QTime::currentTime().addMSecs( ms );
    while( QTime::currentTime() < dieTime )
    {
        //QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void FrmLogin::setLogin(bool login)
{
    this->login = login;
}

void FrmLogin::fail()
{
    login = false;
    ui->btnConnect->setText("Verbinden");
    QMessageBox::critical(this,"FEHLER","Keine Verbindung möglich!");
    update();
}

void FrmLogin::reset()
{
    login = false;
    ui->btnConnect->setText("Verbinden");
    update();
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

void FrmLogin::on_sliderVolume_sliderMoved(int position)
{
    ui->lblVolume->setText(QString::number(position,'f',0)+"%");
}
