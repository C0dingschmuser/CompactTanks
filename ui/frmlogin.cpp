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
    path.append("/settings.ini");
    file.setFileName(path);
    if(file.exists()) {
        ui->edtPassword->setFocus();
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        QStringList list = in.readLine().split("#");
        if(list.size()>2) {
            ui->edtUsername->setText(list.at(0));
            //ui->cbGraphics->setCurrentIndex(list.at(1).toInt());
            if(!list.at(1).toInt()) {
                vsync = false;
            } else {
                vsync = true;
            }
            this->vol1 = list.at(2).toDouble();
            this->vol2 = list.at(3).toInt();
        }
    } else {
        file.open(QIODevice::ReadWrite);
    }
    file.close();
    login = false;
    registration = new FrmRegister();
    connect(registration,SIGNAL(sendEmail(QString)),this,SIGNAL(sendEmail(QString)));
    connect(registration,SIGNAL(confirm(QString)),this,SIGNAL(confirm(QString)));
    connect(registration,SIGNAL(connect()),this,SLOT(on_connect()));
}

FrmLogin::~FrmLogin()
{
    file.close();
    delete ui;
}

void FrmLogin::on_btnConnect_clicked()
{
    if(registration->isVisible()) return;
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
        QString sel = "0";
        if(vsync) sel = "1";
        out << name << "#" << sel << "#" <<
               QString::number(vol1,'f',2) << "#" << QString::number(vol2) << "#" << endl;
        file.close();
        //while(login) {
            emit connectWithData(name,pw,vol1,vol2,vsync);
            //sleep(1000)
            //qApp->processEvents();
        //}
    }
}

void FrmLogin::reg(int code)
{
    registration->reg(code);
}

void FrmLogin::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Return) on_btnConnect_clicked();
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

void FrmLogin::on_btnRegister_clicked()
{
    registration->show();
}

void FrmLogin::on_connect()
{
    emit connectWithData("#0","",0,0,0);
}
