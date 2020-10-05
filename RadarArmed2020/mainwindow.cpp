#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <radarengine_global.h>

#include <QDebug>
#ifdef Q_OS_LINUX
#include <unistd.h>
#endif

//#define LOCALHOST

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("WLR Display");

    curRange = radar_settings.last_scale;
    range_from_radar = 0;

    rt = new radarTransmit(this);
    rt1 = new radarTransmit(this);
    ri = new RI(this,0);
    ri1 = new RI(this,1);
    radarWidget = new RadarWidget(centralWidget(),ri,ri1);
    timer = new QTimer(this);
    dialogGz = new GZDialog(this);
    dialAlarm = new DialogAlarm(this);
    dialTrail = new TrailDialog(this);
    dialRadar = new DialogRadar(this);

#ifdef LOCALHOST
    ri->receiveThread->setMulticastData("127.0.0.1",6678);
    ri->receiveThread->setMulticastReport("127.0.0.1",6679);
    rt->setMulticastData("127.0.0.1",6136);
#else
    ri->receiveThread->setMulticastData(radar_settings.ip_data,radar_settings.port_data);
    ri->receiveThread->setMulticastReport(radar_settings.ip_report,radar_settings.port_report);
    rt->setMulticastData(radar_settings.ip_command,radar_settings.port_command);
    ri1->receiveThread->setMulticastData(radar_settings.ip_data1,radar_settings.port_data1);
    ri1->receiveThread->setMulticastReport(radar_settings.ip_report1,radar_settings.port_report1);
    rt1->setMulticastData(radar_settings.ip_command1,radar_settings.port_command1);
#endif
    ri->receiveThread->start();
    ri1->receiveThread->start();

    connect(ui->frameControl1,SIGNAL(signal_req_Stby()),
            rt,SLOT(RadarStby()));
    connect(ui->frameControl1,SIGNAL(signal_req_Stby()),
            rt1,SLOT(RadarStby()));
    connect(ui->frameControl1,SIGNAL(signal_req_Tx()),
            this,SLOT(trigger_RadarTx()));
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

    connect(ui->frameControl3,&FrameControl3::signal_PPIFullChanged,
            this,&MainWindow::trigger_PPIFullReq);

    connect(ri,&RI::signal_changeAntena,this,&MainWindow::trigger_changeAntena);
    connect(ri,&RI::signal_plotRadarSpoke,
            radarWidget,&RadarWidget::trigger_DrawSpoke);
    connect(ri,SIGNAL(signal_range_change(int)),this,SLOT(trigger_rangeChange(int)));
    connect(ri,SIGNAL(signal_stay_alive()),rt,SLOT(RadarStayAlive()));
    connect(dialTrail,SIGNAL(signal_clearTrailReq()),ri,SLOT(trigger_clearTrail()));

    connect(ri1,&RI::signal_plotRadarSpoke,
            radarWidget,&RadarWidget::trigger_DrawSpoke1);
    connect(ri1,SIGNAL(signal_range_change(int)),this,SLOT(trigger_rangeChange(int)));
    connect(ri1,SIGNAL(signal_stay_alive()),rt1,SLOT(RadarStayAlive()));
    connect(dialTrail,SIGNAL(signal_clearTrailReq()),ri1,SLOT(trigger_clearTrail()));

    connect(ui->frameTrackInf,SIGNAL(signal_request_del_track(int)),
            radarWidget,SLOT(trigger_ReqDelTrack(int)));

    connect(radarWidget,SIGNAL(signal_target_param(int,double,double,double,double,double,double,double)),
            ui->frameTrackInf,SLOT(trigger_target_update(int,double,double,double,double,double,double,double)));
    connect(radarWidget,SIGNAL(signal_cursorMove(double,double)),ui->frameCursor,SLOT(trigger_cursorMove(double, double)));

    connect(dialRadar,SIGNAL(signal_settingChange()),ri,SLOT(trigger_ReqRadarSetting()));
    connect(dialRadar,SIGNAL(signal_settingChange1()),ri1,SLOT(trigger_ReqRadarSetting()));
    connect(dialRadar,SIGNAL(signal_settingChange()),this,SLOT(trigger_ReqRadarSetting()));
    connect(dialRadar,SIGNAL(signal_settingChange1()),this,SLOT(trigger_ReqRadarSetting1()));

    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));
    connect(timer,SIGNAL(timeout()),ui->frameOSD,SLOT(on_timeout()));

    sockAntena = new QTcpSocket(this);
    sockAntena->connectToHost("192.168.1.100",80);

    ui->frameControl1->stateChange(state_radar);
    ui->frameControl2->initParam(enable_mti,mti_value);
    timer->start(1000);
}

void MainWindow::trigger_PPIFullReq()
{
    int radarWidgetWidth = width()-ui->frameRight->width();
    int radarWidgetHeight = height();
    int side = qMin(radarWidgetWidth, radarWidgetHeight);
    int radarWidgetX = ((width()-ui->frameRight->width())/2)-(side/2);
    int radarWidgetY = (height()/2)-(side/2);

    radarWidget->clearMask();

    /*
    */
    QRect rect;
    QRegion region = QRegion(rect,QRegion::Ellipse);
    if(radar_settings.show_ppi_full)
    {
        radarWidget->setGeometry(radarWidgetX,radarWidgetY,side,side);
        rect = QRect(5,5,radarWidget->width()-10,radarWidget->height()-10);
        radarWidget->setMask(region);
        QRegion region = QRegion(rect,QRegion::Ellipse);
        radarWidget->setMask(region);
    }
    else
    {
        radarWidget->setGeometry(radarWidgetX,radarWidgetY+50,side,side);
        rect = QRect(5,5,radarWidget->width()-10,radarWidget->height()-10);
        QRect rect1 = QRect(5,5,radarWidget->width()-10,20+((radarWidget->height()-10)/2));
        QRegion region = QRegion(rect,QRegion::Ellipse);
        QRegion region1 = QRegion(rect1,QRegion::Rectangle);
        QRegion region2 = region.intersected(region1);
        radarWidget->setMask(region2);
    }
    radarWidget->setRectRegoin(rect);
    radarWidget->computetRingWidth();
}
void MainWindow::trigger_RadarTx()
{
    if(radar_settings.enable)
        rt->RadarTx();
    if(radar_settings.enable1)
        rt1->RadarTx();
}
void MainWindow::trigger_changeAntena(QString sig)
{
    if(radar_settings.show_ppi_full)
        return;

    qDebug()<<Q_FUNC_INFO<<sig;
    if(sockAntena->state() == QAbstractSocket::ConnectedState)
        sockAntena->write(sig.toUtf8());
    else if(sockAntena->state() == QAbstractSocket::UnconnectedState)
        sockAntena->connectToHost("192.168.1.100",80);
}
void MainWindow::trigger_ReqRadarSetting()
{
    rt->setMulticastData(radar_settings.ip_command,radar_settings.port_command);
}
void MainWindow::trigger_ReqRadarSetting1()
{
    rt1->setMulticastData(radar_settings.ip_command1,radar_settings.port_command1);
}

void MainWindow::trigger_shutdown()
{
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings config(QDir::homePath()+"/.armed20/radar.conf",QSettings::IniFormat);

    config.setValue("radar/show_ring",radar_settings.show_rings);
    config.setValue("radar/heading_up",radar_settings.headingUp);
    config.setValue("radar/show_compass",radar_settings.show_compass);
    config.setValue("radar/show_ppi_full",false);
    config.setValue("radar/show_heading_marker",radar_settings.show_heading_marker);
    config.setValue("radar/last_scale",curRange);
    config.setValue("radar/ip_data",radar_settings.ip_data);
    config.setValue("radar/port_data",radar_settings.port_data);
    config.setValue("radar/ip_report",radar_settings.ip_report);
    config.setValue("radar/port_report",radar_settings.port_report);
    config.setValue("radar/ip_command",radar_settings.ip_command);
    config.setValue("radar/port_command",radar_settings.port_command);
    config.setValue("radar/ip_data1",radar_settings.ip_data1);
    config.setValue("radar/port_data1",radar_settings.port_data1);
    config.setValue("radar/ip_report1",radar_settings.ip_report1);
    config.setValue("radar/port_report1",radar_settings.port_report1);
    config.setValue("radar/ip_command1",radar_settings.ip_command1);
    config.setValue("radar/port_command1",radar_settings.port_command1);

    config.setValue("arpa/min_contour_len",arpa_settings[0].min_contour_length);
    config.setValue("arpa/create_arpa_by_click",arpa_settings[0].create_arpa_by_click);
    config.setValue("arpa/show",arpa_settings[0].show);

    config.setValue("arpa1/min_contour_len",arpa_settings[1].min_contour_length);
    config.setValue("arpa1/create_arpa_by_click",arpa_settings[1].create_arpa_by_click);
    config.setValue("arpa1/show",arpa_settings[1].show);

    for(int gz_i=0; gz_i<3; gz_i++)
    {
        config.setValue(QString("guardZone%1/show").arg(gz_i),gz_settings[gz_i].show);
        config.setValue(QString("guardZone%1/enable_notif").arg(gz_i),gz_settings[gz_i].enable_alarm);
        config.setValue(QString("guardZone%1/circle_type").arg(gz_i),gz_settings[gz_i].circle_type);
        config.setValue(QString("guardZone%1/inner_range").arg(gz_i),gz_settings[gz_i].inner_range);
        config.setValue(QString("guardZone%1/outer_range").arg(gz_i),gz_settings[gz_i].outer_range);
        config.setValue(QString("guardZone%1/start_bearing").arg(gz_i),gz_settings[gz_i].start_bearing);
        config.setValue(QString("guardZone%1/end_bearing").arg(gz_i),gz_settings[gz_i].end_bearing);
        config.setValue(QString("guardZone%1/notif_thr").arg(gz_i),gz_settings[gz_i].notif_thr);
    }

    config.setValue("trail/enable",trail_settings.enable);
    config.setValue("trail/trail",trail_settings.trail);

    config.setValue("mti/enable",enable_mti);
    config.setValue("mti/threshold",mti_value);

    config.setValue("sensor/heading",currentHeading);
    config.setValue("sensor/lat",currentOwnShipLat);
    config.setValue("sensor/lon",currentOwnShipLon);

    config.setValue("sensor/gps_auto",gps_auto);
    config.setValue("sensor/hdg_auto",hdg_auto);

    config.setValue("tilting/path",tilting_path);

    ri->receiveThread->exitReq();
    ri1->receiveThread->exitReq();
    radarWidget->trigger_shutdown();
#ifdef Q_OS_LINUX
    sleep(1);
#elif defined (Q_OS_WIN32)
    Sleep(1000);
#endif

    event->accept();
}
void MainWindow::timerTimeout()
{
    if(gz_settings[0].show)
    {
        if(gz_settings[0].enable_alarm)
        {
            quint64 now = static_cast<quint64>(QDateTime::currentMSecsSinceEpoch());
            int bogey = ri->m_gz->GetBogeyCount();
            if(bogey > (static_cast<int>(gz_settings[0].notif_thr)))
            {
                if(!gz_settings[0].confirmed || (gz_settings[0].confirmed && TIMED_OUT(now,gz_settings[0].time)))
                {
                    qDebug()<<gz_settings[0].confirmed<<gz_settings[0].time;
                    if(TIMED_OUT(now,gz_settings[0].time))
                        gz_settings[0].confirmed = false;

                    if(dialAlarm->isHidden())
                        dialAlarm->show();

                    dialAlarm->move((width()/2)-(dialAlarm->width()/2),(height()/2)-(dialAlarm->height()/2));
                    dialAlarm->setSpotCount(bogey);
                }
                if((gz_settings[0].confirmed && !TIMED_OUT(now,gz_settings[0].time)))
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
            gz_settings[0].confirmed = false;
        }
    }
    else
    {
        if(ri->m_gz->GetBogeyCount()>0)
            ri->m_gz->ResetBogeys();
        if(!dialAlarm->isHidden())
            dialAlarm->hide();
        gz_settings[0].confirmed = false;
    }

    if(curState != state_radar)
    {
        curState = state_radar;
        ui->frameControl1->stateChange(state_radar);
    }
    if(curState1 != state_radar1)
    {
        curState1 = state_radar1;
        ui->frameControl1->stateChange(state_radar1);
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
        {
            rt->setRange(curRange);
            rt1->setRange(curRange);
        }
    }
    if(curRange != radarWidget->getRange())
    {
        radarWidget->setRange(curRange);
        radarWidget->computetRingWidth();
        ui->frameControl1->setRangeRing(radarWidget->getRingWidth());
    }

    ui->frameRadarStatus->updateStatus();
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
    rt1->setControlValue(CT_GAIN,value);
}
void MainWindow::trigger_rainChange(int value)
{
    rt->setControlValue(CT_RAIN,value);
    rt1->setControlValue(CT_RAIN,value);
}
void MainWindow::trigger_seaChange(int value)
{
    rt->setControlValue(CT_SEA,value);
    rt1->setControlValue(CT_SEA,value);
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

void MainWindow::resizeEvent(QResizeEvent *event)
{    
    qDebug()<<event->size()<<width()<<height();
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

    /*
    */
    QRect rect;
    QRegion region = QRegion(rect,QRegion::Ellipse);
    if(radar_settings.show_ppi_full)
    {
        radarWidget->setGeometry(radarWidgetX,radarWidgetY,side,side);
        rect = QRect(5,5,radarWidget->width()-10,radarWidget->height()-10);
        radarWidget->setMask(region);
        QRegion region = QRegion(rect,QRegion::Ellipse);
        radarWidget->setMask(region);
    }
    else
    {
        radarWidget->setGeometry(radarWidgetX,radarWidgetY+50,side,side);
        rect = QRect(5,5,radarWidget->width()-10,radarWidget->height()-10);
        QRect rect1 = QRect(5,5,radarWidget->width()-10,20+((radarWidget->height()-10)/2));
        QRegion region = QRegion(rect,QRegion::Ellipse);
        QRegion region1 = QRegion(rect1,QRegion::Rectangle);
        QRegion region2 = region.intersected(region1);
        radarWidget->setMask(region2);
    }
    radarWidget->setRectRegoin(rect);
    radarWidget->computetRingWidth();


    /*
    radarWidget->setGeometry(radarWidgetX,radarWidgetY+50,side,side);
    QRect rect = QRect(5,5,radarWidget->width()-10,radarWidget->height()-10);
    QRect rect1 = QRect(5,5,radarWidget->width()-10,20+((radarWidget->height()-10)/2));
    QRegion region = QRegion(rect,QRegion::Ellipse);
    QRegion region1 = QRegion(rect1,QRegion::Rectangle);
    QRegion region2 = region.intersected(region1);
    radarWidget->setMask(region2);
    radarWidget->setRectRegoin(rect);
    radarWidget->computetRingWidth();
    */
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

void MainWindow::on_pushButtonRadar_clicked()
{
    dialRadar->show();
}

void MainWindow::on_pushButtonTilting_clicked()
{
    qDebug()<<Q_FUNC_INFO<<tilting_path.toUtf8();
    qDebug()<<Q_FUNC_INFO<<QProcess::startDetached("cmd /k "+tilting_path);
//    system("cmd /k "+tilting_path.toUtf8()); //put complete path to exe file
}
