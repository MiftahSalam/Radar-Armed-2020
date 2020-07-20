#include "framecontrol1.h"
#include "ui_framecontrol1.h"
#include <radar-engine-armed/radarengine_global.h>

FrameControl1::FrameControl1(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameControl1)
{
    ui->setupUi(this);

    ui->checkBoxShowRing->setChecked(radar_settings.show_rings);
    double init_rng = radar_settings.last_scale/1000;
    ui->labelRange->setText(QString::number(init_rng)+" km");
    ringValue = "";
}
void FrameControl1::stateChange(int state)
{
    if((RadarState)state == RADAR_STANDBY)
    {
        ui->pushButtonTxStnb->setEnabled(true);
        ui->pushButtonTxStnb->setText("Transmit");
    }
    else if((RadarState)state == RADAR_TRANSMIT)
    {
        ui->pushButtonTxStnb->setEnabled(true);
        ui->pushButtonTxStnb->setText("Standby");
    }
    else
    {
        ui->pushButtonTxStnb->setEnabled(false);
        ui->pushButtonTxStnb->setText("Tx/Stby");
    }
}

void FrameControl1::setRangeRing(double ringWidth)
{
    ringValue = QString::number(ringWidth,'f',2);
    if(radar_settings.show_rings)
        ui->labelRingRange->setText("Rings "+ringValue+" km");
    else
        ui->labelRingRange->setText("Rings off");
}

FrameControl1::~FrameControl1()
{
    delete ui;
}

void FrameControl1::on_pushButtonZoomIn_clicked()
{
    int g;
    QString rngName = ui->labelRange->text();
    for (g = 0; g < ARRAY_SIZE(g_ranges_metric); g++)
    {
        if (QString(g_ranges_metric[g].name )== rngName)
            break;
    }
    g++;
    if(g >= ARRAY_SIZE(g_ranges_metric))
        g--;

    ui->labelRange->setText(g_ranges_metric[g].name);
    emit signal_req_range(g_ranges_metric[g].meters);
}

void FrameControl1::on_pushButtonZoomOut_clicked()
{
    int g;
    QString rngName = ui->labelRange->text();
    for (g = 0; g < ARRAY_SIZE(g_ranges_metric); g++)
    {
        if (QString(g_ranges_metric[g].name )== rngName)
            break;
    }
    if(g != 0)
        g--;

    ui->labelRange->setText(g_ranges_metric[g].name);
    emit signal_req_range(g_ranges_metric[g].meters);
}

void FrameControl1::on_pushButtonTxStnb_clicked()
{
    if(ui->pushButtonTxStnb->text().contains("Transmit"))
        emit signal_req_Tx();
    if(ui->pushButtonTxStnb->text().contains("Standby"))
        emit signal_req_Stby ();
}

void FrameControl1::on_checkBoxShowRing_clicked(bool checked)
{
    radar_settings.show_rings = checked;
    if(checked)
        ui->labelRingRange->setText("Rings "+ringValue+" km");
    else
        ui->labelRingRange->setText("Rings off");

}

void FrameControl1::on_pushButtonTxStnb_2_clicked()
{
    emit signal_req_shutdown();
}
