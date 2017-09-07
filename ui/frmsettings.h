#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include <QWidget>

namespace Ui {
class FrmSettings;
}

class FrmSettings : public QWidget
{
    Q_OBJECT

public:
    explicit FrmSettings(QWidget *parent = 0);
    ~FrmSettings();
    void setData(double vol1, int vol2, bool vsync);
private:
    Ui::FrmSettings *ui;
    double soundVol;
    int musicVol;
    bool vsync;
signals:
    void save(double vol1, int vol2, bool vsync);
private slots:
    void on_cbVSync_stateChanged(int arg1);
    void on_sliderSound_valueChanged(int value);
    void on_sliderMusic_valueChanged(int value);
    void on_btnSave_clicked();
};

#endif // FRMSETTINGS_H
