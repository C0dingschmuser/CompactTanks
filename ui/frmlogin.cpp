#include "frmlogin.h"
#include "ui_frmlogin.h"

FrmLogin::FrmLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmLogin)
{
    ui->setupUi(this);
}

FrmLogin::~FrmLogin()
{
    delete ui;
}
