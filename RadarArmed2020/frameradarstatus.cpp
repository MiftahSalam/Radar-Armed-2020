#include "frameradarstatus.h"
#include "ui_frameradarstatus.h"
#include <radarengine_global.h>

FrameRadarStatus::FrameRadarStatus(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameRadarStatus)
{
    ui->setupUi(this);

    append_data_osd.clear();
    reportCounter1 = 0;
    reportCounter2 = 0;
    no_anten_count = 20;
}

void FrameRadarStatus::updateAntena(QString data)
{
    append_data_osd.append(data);

    int index_hdr = append_data_osd.indexOf("?");
    if(index_hdr >= 0)
    {
        int index_end = append_data_osd.indexOf("!");
        if(index_end >= 0)
        {
            if(index_end > index_hdr)
            {
                append_data_osd = append_data_osd.mid(index_hdr,index_end-index_hdr);
                qDebug()<<Q_FUNC_INFO<<"filter"<<append_data_osd.remove("!").remove("?").remove("\r").remove("\n");
                QStringList data_list = append_data_osd.split(" ",QString::SkipEmptyParts);
                qDebug()<<Q_FUNC_INFO<<append_data_osd<<data_list;

                if(data_list.size() == 2)
                {
                    ui->labelR1Ant->setText(data_list.at(2));
                    ui->labelR2Ant->setText(data_list.at(2));
                    ui->labelR1Ant->setStyleSheet("color: green;");
                    ui->labelR2Ant->setStyleSheet("color: green;");
                    no_anten_count = 0;
                }
                else
                {
                    ui->labelR1Ant->setStyleSheet("color: red;");
                    ui->labelR2Ant->setStyleSheet("color: red;");
                }

                append_data_osd.clear();
            }
            else
                append_data_osd.remove(0,index_hdr);

        }
    }
    if(append_data_osd.size() > 50)
        append_data_osd.clear();

}

const QStringList REPORT_STR_LOOP = QStringList()<<"\\"<<"|"<<"/"<<"-";
void FrameRadarStatus::trigger_reportR1()
{
    ui->labelR1Act->setText(REPORT_STR_LOOP.at(reportCounter1));
    reportCounter1++;
    if(reportCounter1 >= REPORT_STR_LOOP.size())
        reportCounter1 = 0;
}

void FrameRadarStatus::trigger_reportR2()
{
    ui->labelR2Act->setText(REPORT_STR_LOOP.at(reportCounter2));
    reportCounter2++;
    if(reportCounter2 >= REPORT_STR_LOOP.size())
        reportCounter2 = 0;
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
