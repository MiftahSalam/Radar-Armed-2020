#ifndef FRAMEOSD_H
#define FRAMEOSD_H

#include <QFrame>

#include "qtmosq.h"

namespace Ui {
class FrameOSD;
}

class FrameOSD : public QFrame
{
    Q_OBJECT

public:
    explicit FrameOSD(QWidget *parent = 0);
    ~FrameOSD();

    double getHDT();
    double getSPD();
    double getCOG();
    double getSOG();
    double getLat();
    double getLon();

private slots:
    void on_timeout();

    void on_receive(QString msg);

    void on_pushButtonApply_clicked();

private:
    Ui::FrameOSD *ui;
    qtmosq *m_mqtt;

    enum SensorSource
    {
        GYRO,
        GPS,
        LOG
    };

    double hdt,spd,cog,sog,lat,lon;

    quint8 no_hdg_count,no_gps_count;
    bool cur_hdg_auto,cur_gps_auto;
    bool hdg_col_normal,gps_col_normal;

};

#endif // FRAMEOSD_H
