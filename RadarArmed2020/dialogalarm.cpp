#include "dialogalarm.h"
#include "ui_dialogalarm.h"
#include <radar-engine-armed/radarengine_global.h>

#include <QDateTime>

DialogAlarm::DialogAlarm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAlarm)
{
    ui->setupUi(this);
}

DialogAlarm::~DialogAlarm()
{
    delete ui;
}
void DialogAlarm::setSpotCount(int count)
{
    ui->label->setText(QString("Spot Count\n(%1)").arg(count));
}

void DialogAlarm::on_pushButton_clicked()
{
    gz_settings.confirmed = true;
    gz_settings.time = QDateTime::currentMSecsSinceEpoch()+gz_settings.timeout;
}
