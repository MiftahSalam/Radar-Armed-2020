#include "frameradarstatus.h"
#include "ui_frameradarstatus.h"
#include <radarengine_global.h>

FrameRadarStatus::FrameRadarStatus(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameRadarStatus)
{
    ui->setupUi(this);
}

void FrameRadarStatus::updateStatus()
{
    switch (state_radar)
    {
    case RADAR_OFF:
        ui->labelRadarBottomStatus->setText("No Radar");
        ui->labelRadarBottomStatus->setStyleSheet("color: rgb(170,0,0);");
        break;
    case RADAR_STANDBY:
        ui->labelRadarBottomStatus->setText("Standby");
        ui->labelRadarBottomStatus->setStyleSheet("color: rgb(170,170,0);");
        break;
    case RADAR_WAKING_UP:
        ui->labelRadarBottomStatus->setText("Waking Up");
        ui->labelRadarBottomStatus->setStyleSheet("color: rgb(170,170,0);");
        break;
    case RADAR_TRANSMIT:
        ui->labelRadarBottomStatus->setText("Transmiting");
        ui->labelRadarBottomStatus->setStyleSheet("color: rgb(0,170,0);");
    }

    switch (state_radar1)
    {
    case RADAR_OFF:
        ui->labelRadarTopStatus->setText("No Radar");
        ui->labelRadarTopStatus->setStyleSheet("color: rgb(170,0,0);");
        break;
    case RADAR_STANDBY:
        ui->labelRadarTopStatus->setText("Standby");
        ui->labelRadarTopStatus->setStyleSheet("color: rgb(170,170,0);");
        break;
    case RADAR_WAKING_UP:
        ui->labelRadarTopStatus->setText("Waking Up");
        ui->labelRadarTopStatus->setStyleSheet("color: rgb(170,170,0);");
        break;
    case RADAR_TRANSMIT:
        ui->labelRadarTopStatus->setText("Transmiting");
        ui->labelRadarTopStatus->setStyleSheet("color: rgb(0,170,0);");
    }
}
FrameRadarStatus::~FrameRadarStatus()
{
    delete ui;
}
