#include <QApplication>
#include <QDir>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    filter.gain = 0;
    filter.rain = 0;
    filter.sea = 0;
    filter.targetBoost = 0;
    filter.LInterference = 0;
    filter.targetExpan = 0;
    filter.range = 0;

    align.bearing = 0;
    align.antena_height = 0;

    scanSignal.scan_speed = 0;
    scanSignal.noise_reject = 0;
    scanSignal.target_sep = 0;
    scanSignal.side_lobe_suppression = 0;
    scanSignal.local_interference_rejection = 0;

    QSettings config(QDir::homePath()+"/.armed20/radar.conf",QSettings::IniFormat);

    radar_settings.show_rings = config.value("radar/show_ring",false).toBool();
    radar_settings.headingUp = config.value("radar/heading_up",false).toBool();
    radar_settings.show_compass = config.value("radar/show_compass",false).toBool();
    radar_settings.show_heading_marker = config.value("radar/show_heading_marker",false).toBool();
    radar_settings.last_scale = config.value("radar/last_scale",2000).toInt();
    radar_settings.ip_data = config.value("radar/ip_data","127.0.0.1").toString();
    radar_settings.port_data = config.value("radar/port_data",10001).toUInt();
    radar_settings.ip_report = config.value("radar/ip_report","127.0.0.1").toString();
    radar_settings.port_report = config.value("radar/port_report",10001).toUInt();
    radar_settings.ip_command = config.value("radar/ip_command","127.0.0.1").toString();
    radar_settings.port_command = config.value("radar/port_command",10001).toUInt();

    arpa_settings.min_contour_length = config.value("arpa/min_contour_len",90).toInt();
    arpa_settings.create_arpa_by_click = config.value("arpa/min_contour_len",true).toBool();
    arpa_settings.show = config.value("arpa/show",true).toBool();

    for(int gz_i=0; gz_i<3; gz_i++)
    {
        gz_settings[gz_i].show = config.value(QString("guardZone%1/show").arg(gz_i),false).toBool();
        gz_settings[gz_i].enable_alarm = config.value(QString("guardZone%1/enable_notif").arg(gz_i),false).toBool();
        gz_settings[gz_i].circle_type = config.value(QString("guardZone%1/circle_type").arg(gz_i),false).toBool();
        gz_settings[gz_i].inner_range = config.value(QString("guardZone%1/inner_range").arg(gz_i),100).toInt();
        gz_settings[gz_i].notif_thr = config.value(QString("guardZone%1/notif_thr").arg(gz_i),10).toUInt();
        gz_settings[gz_i].outer_range = config.value(QString("guardZone%1/outer_range").arg(gz_i),1000).toInt();
        gz_settings[gz_i].start_bearing = config.value(QString("guardZone%1/start_bearing").arg(gz_i),0).toDouble();
        gz_settings[gz_i].end_bearing = config.value(QString("guardZone%1/end_bearing").arg(gz_i),90).toDouble();
        gz_settings[gz_i].timeout = config.value(QString("guardZone%1/timeout").arg(gz_i),90).toUInt();
        gz_settings[gz_i].confirmed = false;
    }

    trail_settings.enable = config.value("trail/enable",true).toBool();
    trail_settings.trail = config.value("trail/trail",1).toInt();

    radarHeading = 0;

    currentHeading = config.value("sensor/heading",0).toDouble();
    currentOwnShipLat = config.value("sensor/lat",0).toDouble();
    currentOwnShipLon = config.value("sensor/lon",0).toDouble();
    gps_auto = config.value("sensor/gps_auto",false).toBool();
    hdg_auto = config.value("sensor/hdg_auto",false).toBool();

    enable_mti = config.value("mti/enable",true).toBool();
    mti_value = config.value("mti/threshold",100).toInt();

    MainWindow w;
    w.resize(850,850);
    w.show();
    
    return a.exec();

}
