#include "framecontrol3.h"
#include "ui_framecontrol3.h"
#include <radar-engine-armed/radarengine_global.h>

FrameControl3::FrameControl3(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameControl3)
{
    ui->setupUi(this);

    ui->comboBoxMotion->setCurrentIndex(radar_settings.headingUp ? 1 : 0);
    ui->checkBoxShowCompass->setChecked(radar_settings.show_compass);
    ui->checkBoxShowHM->setChecked(radar_settings.show_heading_marker);
}

FrameControl3::~FrameControl3()
{
    delete ui;
}

void FrameControl3::on_comboBoxMotion_currentIndexChanged(int index)
{
    radar_settings.headingUp = index>0 ? true : false;
}

void FrameControl3::on_checkBoxShowCompass_clicked(bool checked)
{
    radar_settings.show_compass = checked;
}

void FrameControl3::on_checkBoxShowHM_clicked(bool checked)
{
    radar_settings.show_heading_marker = checked;
}
