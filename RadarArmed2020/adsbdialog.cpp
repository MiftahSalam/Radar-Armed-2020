#include "adsbdialog.h"
#include "ui_adsbdialog.h"
#include <radar-engine-armed/radarengine_global.h>

adsbDialog::adsbDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adsbDialog)
{
    ui->setupUi(this);
    ui->checkBoxShow->setChecked(adsb_settings.show);

    ui->lineEditIP->setValidator(new QIntValidator(0,255,ui->lineEditIP));
    ui->lineEditPort->setValidator(new QIntValidator(3000,65536,ui->lineEditPort));

    ui->lineEditIP->setText(adsb_settings.ip);
    ui->lineEditPort->setText(QString::number(adsb_settings.port));

}

adsbDialog::~adsbDialog()
{
    delete ui;
}


void adsbDialog::on_checkBoxShow_clicked(bool checked)
{
    adsb_settings.show = checked;
}

void adsbDialog::on_timeout()
{
    if(adsb_settings.connected != connected)
    {
        if(adsb_settings.connected)
            ui->pushButtonClear->setText("Connected");
        else
            ui->pushButtonClear->setText("Not Connected");

        connected = adsb_settings.connected;
    }
}

void adsbDialog::on_pushButtonApply_clicked()
{
    adsb_settings.ip = ui->lineEditIP->text().remove(" ");
    adsb_settings.port = ui->lineEditPort->text().toUInt();

    emit signal_settingChange();
}
