#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <radar-engine-armed/radarengine_global.h>

#include <QDebug>
#include <unistd.h>

//#define LOCALHOST

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    curRange = radar_settings.last_scale;
    range_from_radar = 0;

    rt = new radarTransmit(this);
    ri = new RI(this);
    radarWidget = new RadarWidget(centralWidget(),ri);
    timer = new QTimer(this);
    dialogGz = new GZDialog(this);
    dialAlarm = new DialogAlarm(this);
    dialTrail = new TrailDialog(this);
    dialADSB = new adsbDialog(this);
    dialRadar = new DialogRadar(this);

#ifdef LOCALHOST
    ri->receiveThread->setMulticastData("127.0.0.1",6678);
    ri->receiveThread->setMulticastReport("127.0.0.1",6679);
    rt->setMulticastData("127.0.0.1",6136);
#else
    ri->receiveThread->setMulticastData(radar_settings.ip_data,radar_settings.port_data);
    ri->receiveThread->setMulticastReport(radar_settings.ip_report,radar_settings.port_report);
    rt->setMulticastData(radar_settings.ip_command,radar_settings.port_command);
    /*
ip_data=236.6.7.103
port_data=6135
ip_report=236.6.7.105
port_report=6137
ip_command=236.6.7.104
port_command=6136*/
#endif
    ri->receiveThread->start();


    connect(ui->frameControl1,SIGNAL(signal_req_Stby()),
            rt,SLOT(RadarStby()));
    connect(ui->frameControl1,SIGNAL(signal_req_Tx()),
            rt,SLOT(RadarTx()));
    connect(ui->frameControl1,SIGNAL(signal_req_shutdown()),
            this,SLOT(trigger_shutdown()));
    connect(ui->frameControl1,SIGNAL(signal_req_range(int)),
            this,SLOT(trigger_rangeChangeReq(int)));

    connect(ui->frameControl2,SIGNAL(signal_change_gain_req(int)),
            this,SLOT(trigger_gainChange(int)));
    connect(ui->frameControl2,SIGNAL(signal_change_rain_req(int)),
            this,SLOT(trigger_rainChange(int)));
    connect(ui->frameControl2,SIGNAL(signal_change_sea_req(int)),
            this,SLOT(trigger_seaChange(int)));

    connect(ri,SIGNAL(signal_plotRadarSpoke(int,int,u_int8_t*,size_t)),
            radarWidget,SLOT(trigger_DrawSpoke(int,int,u_int8_t*,size_t)));
    connect(ri,SIGNAL(signal_range_change(int)),this,SLOT(trigger_rangeChange(int)));
    connect(ri,SIGNAL(signal_stay_alive()),rt,SLOT(RadarStayAlive()));
    connect(dialTrail,SIGNAL(signal_clearTrailReq()),ri,SLOT(trigger_clearTrail()));

    connect(ui->frameTrackInf,SIGNAL(signal_request_del_track(int)),
            radarWidget,SLOT(trigger_ReqDelTrack(int)));

    connect(radarWidget,SIGNAL(signal_target_param(int,double,double,double,double)),
            ui->frameTrackInf,SLOT(trigger_target_update(int,double,double,double,double)));
    connect(radarWidget,SIGNAL(signal_adsb_param(QString,double,double,double,double,double)),
            ui->frameADSBInf,SLOT(trigger_target_update(QString,double,double,double,double,double)));
    connect(radarWidget,SIGNAL(signal_cursorMove(double,double)),ui->frameCursor,SLOT(trigger_cursorMove(double, double)));

    connect(dialADSB,SIGNAL(signal_settingChange()),radarWidget,SLOT(trigger_ReqADSBSetting()));
    connect(dialRadar,SIGNAL(signal_settingChange()),ri,SLOT(trigger_ReqRadarSetting()));
    connect(dialRadar,SIGNAL(signal_settingChange()),this,SLOT(trigger_ReqRadarSetting()));

    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
    connect(timer,SIGNAL(timeout()),ui->frameOSD,SLOT(on_timeout()));
    connect(timer,SIGNAL(timeout()),dialADSB,SLOT(on_timeout()));

    ui->frameControl1->stateChange(state_radar);
    ui->frameControl2->initParam(enable_mti,mti_value);
    timer->start(1000);
}
void MainWindow::trigger_ReqRadarSetting()
{
    rt->setMulticastData(radar_settings.ip_command,radar_settings.port_command);
}

void MainWindow::trigger_shutdown()
{
    QSettings config(QDir::homePath()+"/.simrad/radar.conf",QSettings::IniFormat);

    config.setValue("radar/show_ring",radar_settings.show_rings);
    config.setValue("radar/heading_up",radar_settings.headingUp);
    config.setValue("radar/show_compass",radar_settings.show_compass);
    config.setValue("radar/show_heading_marker",radar_settings.show_heading_marker);
    config.setValue("radar/last_scale",curRange);
    config.setValue("radar/ip_data",radar_settings.ip_data);
    config.setValue("radar/port_data",radar_settings.port_data);
    config.setValue("radar/ip_report",radar_settings.ip_report);
    config.setValue("radar/port_report",radar_settings.port_report);
    config.setValue("radar/ip_command",radar_settings.ip_command);
    config.setValue("radar/port_command",radar_settings.port_command);

    config.setValue("arpa/min_contour_len",arpa_settings.min_contour_length);
    config.setValue("arpa/create_arpa_by_click",arpa_settings.create_arpa_by_click);
    config.setValue("arpa/show",arpa_settings.show);

    config.setValue("guardZone/show",gz_settings.show);
    config.setValue("guardZone/enable_notif",gz_settings.enable_alarm);
    config.setValue("guardZone/circle_type",gz_settings.circle_type);
    config.setValue("guardZone/inner_range",gz_settings.inner_range);
    config.setValue("guardZone/outer_range",gz_settings.outer_range);
    config.setValue("guardZone/start_bearing",gz_settings.start_bearing);
    config.setValue("guardZone/end_bearing",gz_settings.end_bearing);
    config.setValue("guardZone/notif_thr",gz_settings.notif_thr);

    config.setValue("trail/enable",trail_settings.enable);
    config.setValue("trail/trail",trail_settings.trail);

    config.setValue("adsb/show",adsb_settings.show);
    config.setValue("adsb/ip",adsb_settings.ip);
    config.setValue("adsb/port",adsb_settings.port);

    config.setValue("mti/enable",enable_mti);
    config.setValue("mti/threshold",mti_value);

    config.setValue("sensor/heading",currentHeading);
    config.setValue("sensor/lat",currentOwnShipLat);
    config.setValue("sensor/lon",currentOwnShipLon);

    config.setValue("sensor/gps_auto",gps_auto);
    config.setValue("sensor/hdg_auto",hdg_auto);

    ri->receiveThread->exitReq();
    radarWidget->trigger_shutdown();
    sleep(1);
    close();
}

void MainWindow::timerTimeout()
{
    if(gz_settings.show)
    {
        if(gz_settings.enable_alarm)
        {
            quint64 now = QDateTime::currentMSecsSinceEpoch();
            int bogey = ri->m_gz->GetBogeyCount();
            if(bogey>gz_settings.notif_thr)
            {
                if(!gz_settings.confirmed || (gz_settings.confirmed && TIMED_OUT(now,gz_settings.time)))
                {
                    qDebug()<<gz_settings.confirmed<<gz_settings.time;
                    if(TIMED_OUT(now,gz_settings.time))
                        gz_settings.confirmed = false;

                    if(dialAlarm->isHidden())
                        dialAlarm->show();

                    dialAlarm->move((width()/2)-(dialAlarm->width()/2),(height()/2)-(dialAlarm->height()/2));
                    dialAlarm->setSpotCount(bogey);
                }
                if((gz_settings.confirmed && !TIMED_OUT(now,gz_settings.time)))
                    if(!dialAlarm->isHidden())
                        dialAlarm->hide();
            }
            else
            {
                if(!dialAlarm->isHidden())
                    dialAlarm->hide();
            }
        }
        else
        {
            if(ri->m_gz->GetBogeyCount()>0)
                ri->m_gz->ResetBogeys();
            if(!dialAlarm->isHidden())
                dialAlarm->hide();
            gz_settings.confirmed = false;
        }
    }
    else
    {
        if(ri->m_gz->GetBogeyCount()>0)
            ri->m_gz->ResetBogeys();
        if(!dialAlarm->isHidden())
            dialAlarm->hide();
        gz_settings.confirmed = false;
    }

    if(curState != state_radar)
    {
        curState = state_radar;
        ui->frameControl1->stateChange(state_radar);
    }

    currentOwnShipLat = ui->frameOSD->getLat();
    currentOwnShipLon = ui->frameOSD->getLon();

    /*difference range*/
    if(curRange != range_from_radar)
    {
        /*
        int g;
        for (g = 0; g < ARRAY_SIZE(g_ranges_metric); g++)
        {
            if (g_ranges_metric[g].meters == curRange)
                break;
        }
        g++;

        rt->setRange(g_ranges_metric[g].meters);
        */
        if(state_radar == RADAR_TRANSMIT)
            rt->setRange(curRange);
    }
    if(curRange != radarWidget->getRange())
    {
        radarWidget->setRange(curRange);
        radarWidget->computetRingWidth();
        ui->frameControl1->setRangeRing(radarWidget->getRingWidth());
    }


    /*next select HDG procedure*/
    currentHeading = ui->frameOSD->getHDT();
}
void MainWindow::trigger_rangeChangeReq(int range)
{
    int g;
    for (g = 0; g < ARRAY_SIZE(g_ranges_metric); g++)
    {
        if (g_ranges_metric[g].meters == range)
        {
            curRange = range;
            break;
        }
    }
}

void MainWindow::trigger_gainChange(int value)
{
    rt->setControlValue(CT_GAIN,value);
}
void MainWindow::trigger_rainChange(int value)
{
    rt->setControlValue(CT_RAIN,value);
}
void MainWindow::trigger_seaChange(int value)
{
    rt->setControlValue(CT_SEA,value);
}

void MainWindow::trigger_rangeChange(int range)
{
    int g;
    for (g = 0; g < ARRAY_SIZE(g_ranges_metric); g++)
    {
        if (g_ranges_metric[g].actual_meters == range)
        {
            range_from_radar = g_ranges_metric[g].meters;
            break;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{    
    ui->frameRight->move(width()-ui->frameRight->width(),0);
    ui->frameRight->resize(ui->frameRight->width(),height());

    ui->frameCursor->move(ui->frameRight->x()-ui->frameCursor->width(),0);
    ui->frameControl3->move(ui->frameRight->x()-ui->frameControl3->width(),height()-ui->frameControl3->height());
    ui->frameControl2->move(0,height()-ui->frameControl2->height());
    ui->frameControl1->move(0,height()-ui->frameControl2->height());
    ui->frameControl1->move(0,0);

    int radarWidgetWidth = width()-ui->frameRight->width();
    int radarWidgetHeight = height();
    int side = qMin(radarWidgetWidth, radarWidgetHeight);
    int radarWidgetX = ((width()-ui->frameRight->width())/2)-(side/2);
    int radarWidgetY = (height()/2)-(side/2);


    radarWidget->clearMask();
    radarWidget->setGeometry(radarWidgetX,radarWidgetY,side,side);
    QRect rect = QRect(5,5,radarWidget->width()-10,radarWidget->height()-10);
    QRegion region = QRegion(rect,QRegion::Ellipse);
    radarWidget->setMask(region);
    radarWidget->setRectRegoin(rect);
    radarWidget->computetRingWidth();
}


void MainWindow::on_pushButtonSetGZ_clicked()
{
    dialogGz->show();
    dialogGz->move(width()/2,height()/2);
}

void MainWindow::on_pushButtonSetTrail_clicked()
{
    dialTrail->show();
}

void MainWindow::on_pushButtonSetADSB_clicked()
{
    dialADSB->show();
}

void MainWindow::on_pushButtonRadar_clicked()
{
    dialRadar->show();
}
