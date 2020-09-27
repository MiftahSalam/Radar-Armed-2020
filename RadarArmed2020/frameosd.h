#ifndef FRAMEOSD_H
#define FRAMEOSD_H

#include <QFrame>
#include <QUdpSocket>
#include <QTcpSocket>

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
    void on_receiveUDP();
    void on_receiveTCP();

    void on_pushButtonApply_clicked();

private:
    Ui::FrameOSD *ui;
    QUdpSocket *osdSocket;
    QTcpSocket *roomSocket;

    enum SensorSource
    {
        GYRO,
        GPS,
        LOG
    };

    double hdt,spd,cog,sog,lat,lon;

    quint8 no_hdg_count,no_gps_count,no_room_count;
    bool cur_hdg_auto,cur_gps_auto;
    bool hdg_col_normal,gps_col_normal;
    QString append_data_osd;

};

#endif // FRAMEOSD_H
