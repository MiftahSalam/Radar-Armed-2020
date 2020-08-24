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

    ui->lineEditInner_2->setValidator(new QDoubleValidator(0.0,60.0,2,ui->lineEditInner_2));
    ui->lineEditOuter_2->setValidator(new QDoubleValidator(0.0,60.0,2,ui->lineEditOuter_2));
    ui->lineEditStart_2->setValidator(new QDoubleValidator(0.0,360.0,1,ui->lineEditStart_2));
    ui->lineEditEnd_2->setValidator(new QDoubleValidator(0.0,360.0,1,ui->lineEditEnd_2));
    ui->lineEditNotifTHR_2->setValidator(new QIntValidator(10,500,ui->lineEditNotifTHR_2));

    ui->lineEditInner_3->setValidator(new QDoubleValidator(0.0,60.0,2,ui->lineEditInner_3));
    ui->lineEditOuter_3->setValidator(new QDoubleValidator(0.0,60.0,2,ui->lineEditOuter_3));
    ui->lineEditStart_3->setValidator(new QDoubleValidator(0.0,360.0,1,ui->lineEditStart_3));
    ui->lineEditEnd_3->setValidator(new QDoubleValidator(0.0,360.0,1,ui->lineEditEnd_3));
    ui->lineEditNotifTHR_3->setValidator(new QIntValidator(10,500,ui->lineEditNotifTHR_3));

    ui->checkBoxShowGZ->setChecked(gz_settings[0].show);
    ui->checkBoxAlarmGZ->setChecked(gz_settings[0].enable_alarm);
    ui->radioButtonArc->setChecked(!gz_settings[0].circle_type);
    ui->radioButtonCircle->setChecked(gz_settings[0].circle_type);
    ui->lineEditInner->setText(QString::number(gz_settings[0].inner_range/1000,'f',2));
    ui->lineEditOuter->setText(QString::number(gz_settings[0].outer_range/1000,'f',2));
    ui->lineEditStart->setText(QString::number(gz_settings[0].start_bearing,'f',1));
    ui->lineEditEnd->setText(QString::number(gz_settings[0].end_bearing,'f',1));
    ui->lineEditNotifTHR->setText(QString::number(gz_settings[0].notif_thr));

    ui->checkBoxShowGZ_2->setChecked(gz_settings[1].show);
    ui->checkBoxAlarmGZ_2->setChecked(gz_settings[1].enable_alarm);
    ui->radioButtonArc_2->setChecked(!gz_settings[1].circle_type);
    ui->radioButtonCircle_2->setChecked(gz_settings[1].circle_type);
    ui->lineEditInner_2->setText(QString::number(gz_settings[1].inner_range/1000,'f',2));
    ui->lineEditOuter_2->setText(QString::number(gz_settings[1].outer_range/1000,'f',2));
    ui->lineEditStart_2->setText(QString::number(gz_settings[1].start_bearing,'f',1));
    ui->lineEditEnd_2->setText(QString::number(gz_settings[1].end_bearing,'f',1));
    ui->lineEditNotifTHR_2->setText(QString::number(gz_settings[1].notif_thr));

    ui->checkBoxShowGZ_3->setChecked(gz_settings[2].show);
    ui->checkBoxAlarmGZ_3->setChecked(gz_settings[2].enable_alarm);
    ui->radioButtonArc_3->setChecked(!gz_settings[2].circle_type);
    ui->radioButtonCircle_3->setChecked(gz_settings[2].circle_type);
    ui->lineEditInner_3->setText(QString::number(gz_settings[2].inner_range/1000,'f',2));
    ui->lineEditOuter_3->setText(QString::number(gz_settings[2].outer_range/1000,'f',2));
    ui->lineEditStart_3->setText(QString::number(gz_settings[2].start_bearing,'f',1));
    ui->lineEditEnd_3->setText(QString::number(gz_settings[2].end_bearing,'f',1));
    ui->lineEditNotifTHR_3->setText(QString::number(gz_settings[2].notif_thr));
}

GZDialog::~GZDialog()
{
    delete ui;
}

void GZDialog::on_checkBoxShowGZ_clicked(bool checked)
{
    gz_settings[0].show = checked;
}

void GZDialog::on_checkBoxAlarmGZ_clicked(bool checked)
{
    gz_settings[0].enable_alarm = checked;
}

void GZDialog::on_radioButtonArc_clicked(bool checked)
{
    gz_settings[0].circle_type = !checked;
}

void GZDialog::on_radioButtonCircle_clicked(bool checked)
{
    gz_settings[0].circle_type = checked;
}

void GZDialog::on_lineEditInner_textChanged(const QString &arg1)
{
    bool ok;
    double range = arg1.toDouble(&ok);
    if(ok)
        gz_settings[0].inner_range = range*1000;
}

void GZDialog::on_lineEditOuter_textChanged(const QString &arg1)
{
    bool ok;
    double range = arg1.toDouble(&ok);
    if(ok)
        gz_settings[0].outer_range = range*1000;
}

void GZDialog::on_lineEditStart_textChanged(const QString &arg1)
{
    bool ok;
    double brn = arg1.toDouble(&ok);
    if(ok)
        gz_settings[0].start_bearing = brn;

}

void GZDialog::on_lineEditEnd_textChanged(const QString &arg1)
{
    bool ok;
    double brn = arg1.toDouble(&ok);
    if(ok)
        gz_settings[0].end_bearing = brn;

}

void GZDialog::on_lineEditNotifTHR_textChanged(const QString &arg1)
{
    bool ok;
    uint thr = arg1.toUInt(&ok);
    if(ok)
        gz_settings[0].notif_thr = thr;
}

void GZDialog::on_checkBoxShowGZ_2_clicked(bool checked)
{
    gz_settings[1].show = checked;
}

void GZDialog::on_checkBoxAlarmGZ_2_clicked(bool checked)
{
    gz_settings[1].enable_alarm = checked;
}

void GZDialog::on_radioButtonArc_2_clicked(bool checked)
{
    gz_settings[1].circle_type = !checked;
}

void GZDialog::on_radioButtonCircle_2_clicked(bool checked)
{
    gz_settings[1].circle_type = checked;
}

void GZDialog::on_lineEditInner_2_textChanged(const QString &arg1)
{
    bool ok;
    double range = arg1.toDouble(&ok);
    if(ok)
        gz_settings[1].inner_range = range*1000;
}

void GZDialog::on_lineEditOuter_2_textChanged(const QString &arg1)
{
    bool ok;
    double range = arg1.toDouble(&ok);
    if(ok)
        gz_settings[1].outer_range = range*1000;

}

void GZDialog::on_lineEditStart_2_textChanged(const QString &arg1)
{
    bool ok;
    double brn = arg1.toDouble(&ok);
    if(ok)
        gz_settings[1].start_bearing = brn;
}

void GZDialog::on_lineEditEnd_2_textChanged(const QString &arg1)
{
    bool ok;
    double brn = arg1.toDouble(&ok);
    if(ok)
        gz_settings[1].end_bearing = brn;
}

void GZDialog::on_lineEditNotifTHR_2_textChanged(const QString &arg1)
{
    bool ok;
    uint thr = arg1.toUInt(&ok);
    if(ok)
        gz_settings[1].notif_thr = thr;
}

void GZDialog::on_checkBoxShowGZ_3_clicked(bool checked)
{
    gz_settings[2].show = checked;
}

void GZDialog::on_checkBoxAlarmGZ_3_clicked(bool checked)
{
    gz_settings[2].enable_alarm = checked;
}

void GZDialog::on_radioButtonArc_3_clicked(bool checked)
{
    gz_settings[2].circle_type = !checked;
}
void GZDialog::on_radioButtonCircle_3_clicked(bool checked)
{
    gz_settings[2].circle_type = checked;
}

void GZDialog::on_lineEditInner_3_textChanged(const QString &arg1)
{
    bool ok;
    double range = arg1.toDouble(&ok);
    if(ok)
        gz_settings[2].inner_range = range*1000;
}

void GZDialog::on_lineEditOuter_3_textChanged(const QString &arg1)
{
    bool ok;
    double range = arg1.toDouble(&ok);
    if(ok)
        gz_settings[2].outer_range = range*1000;
}

void GZDialog::on_lineEditStart_3_textChanged(const QString &arg1)
{
    bool ok;
    double brn = arg1.toDouble(&ok);
    if(ok)
        gz_settings[2].start_bearing = brn;
}

void GZDialog::on_lineEditEnd_3_textChanged(const QString &arg1)
{
    bool ok;
    double brn = arg1.toDouble(&ok);
    if(ok)
        gz_settings[2].end_bearing = brn;
}

void GZDialog::on_lineEditNotifTHR_3_textChanged(const QString &arg1)
{
    bool ok;
    uint thr = arg1.toUInt(&ok);
    if(ok)
        gz_settings[2].notif_thr = thr;
}
