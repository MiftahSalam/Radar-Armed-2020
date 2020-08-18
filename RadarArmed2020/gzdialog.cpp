#include "gzdialog.h"
#include "ui_gzdialog.h"
#include <radarengine_global.h>

GZDialog::GZDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GZDialog)
{
    ui->setupUi(this);

    ui->lineEditInner->setValidator(new QDoubleValidator(0.0,60.0,2,ui->lineEditInner));
    ui->lineEditOuter->setValidator(new QDoubleValidator(0.0,60.0,2,ui->lineEditOuter));
    ui->lineEditStart->setValidator(new QDoubleValidator(0.0,360.0,1,ui->lineEditStart));
    ui->lineEditEnd->setValidator(new QDoubleValidator(0.0,360.0,1,ui->lineEditEnd));
    ui->lineEditNotifTHR->setValidator(new QIntValidator(10,500,ui->lineEditNotifTHR));

    ui->checkBoxShowGZ->setChecked(gz_settings.show);
    ui->checkBoxAlarmGZ->setChecked(gz_settings.enable_alarm);
    ui->radioButtonArc->setChecked(!gz_settings.circle_type);
    ui->radioButtonCircle->setChecked(gz_settings.circle_type);
    ui->lineEditInner->setText(QString::number(gz_settings.inner_range/1000,'f',2));
    ui->lineEditOuter->setText(QString::number(gz_settings.outer_range/1000,'f',2));
    ui->lineEditStart->setText(QString::number(gz_settings.start_bearing,'f',1));
    ui->lineEditEnd->setText(QString::number(gz_settings.end_bearing,'f',1));
    ui->lineEditNotifTHR->setText(QString::number(gz_settings.notif_thr));
}

GZDialog::~GZDialog()
{
    delete ui;
}

void GZDialog::on_checkBoxShowGZ_clicked(bool checked)
{
    gz_settings.show = checked;
}

void GZDialog::on_checkBoxAlarmGZ_clicked(bool checked)
{
    gz_settings.enable_alarm = checked;
}

void GZDialog::on_radioButtonArc_clicked(bool checked)
{
    gz_settings.circle_type = !checked;
}

void GZDialog::on_radioButtonCircle_clicked(bool checked)
{
    gz_settings.circle_type = checked;
}

void GZDialog::on_lineEditInner_textChanged(const QString &arg1)
{
    bool ok;
    double range = arg1.toDouble(&ok);
    if(ok)
        gz_settings.inner_range = range*1000;
}

void GZDialog::on_lineEditOuter_textChanged(const QString &arg1)
{
    bool ok;
    double range = arg1.toDouble(&ok);
    if(ok)
        gz_settings.outer_range = range*1000;
}

void GZDialog::on_lineEditStart_textChanged(const QString &arg1)
{
    bool ok;
    double brn = arg1.toDouble(&ok);
    if(ok)
        gz_settings.start_bearing = brn;

}

void GZDialog::on_lineEditEnd_textChanged(const QString &arg1)
{
    bool ok;
    double brn = arg1.toDouble(&ok);
    if(ok)
        gz_settings.end_bearing = brn;

}

void GZDialog::on_lineEditNotifTHR_textChanged(const QString &arg1)
{
    bool ok;
    double thr = arg1.toUInt(&ok);
    if(ok)
        gz_settings.notif_thr = thr;
}
