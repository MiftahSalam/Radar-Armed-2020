#include "dialogalarm.h"
#include "ui_dialogalarm.h"
#include <radarengine_global.h>

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
    gz_settings[0].confirmed = true;
    gz_settings[0].time = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch())+gz_settings[0].timeout;
}
