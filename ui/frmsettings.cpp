#include "frmsettings.h"
#include "ui_frmsettings.h"

FrmSettings::FrmSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmSettings)
{
    ui->setupUi(this);
}

FrmSettings::~FrmSettings()
{
    delete ui;
}

void FrmSettings::setData(double vol1, int vol2, bool vsync)
{
    this->soundVol = vol1;
    this->musicVol = vol2;
    this->vsync = vsync;
    ui->sliderSound->setValue(vol1*100);
    ui->sliderMusic->setValue(vol2);
    if(vsync) {
        ui->cbVSync->setChecked(true);
    } else {
        ui->cbVSync->setChecked(false);
    }
}

void FrmSettings::on_cbVSync_stateChanged(int arg1)
{
    vsync = arg1;
}

void FrmSettings::on_sliderSound_valueChanged(int value)
{
    double e = value/(double)100;
    this->soundVol = e;
}

void FrmSettings::on_sliderMusic_valueChanged(int value)
{
    this->musicVol = value;
}

void FrmSettings::on_btnSave_clicked()
{
    emit save(soundVol,musicVol,vsync);
}
