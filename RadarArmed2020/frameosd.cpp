#include "frameosd.h"
#include "ui_frameosd.h"
#include <radarengine_global.h>

#include <QTime>
#include <QSettings>
#include <QDir>

FrameOSD::FrameOSD(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameOSD)
{
    ui->setupUi(this);

    hdt = currentHeading;
    spd = 0;
    cog = 0;
    sog = 0;
    no_hdg_count = 0;
    cur_hdg_auto = hdg_auto;
    hdg_col_normal = false;
    no_gps_count = 0;
    cur_gps_auto = gps_auto;
    gps_col_normal = false;
    no_room_count = 0;

//    lat = 52.0;
//    lon = 6.0;

//    lat = -6.96; // jingga
//    lon = 107.66;

//    lat = -6.88; // jingga
//    lon = 107.58;

//    lat = -6.86944; // cisangkan. lat 6 deg 52' 10" S lon 107 deg 32' 17.1" E
//    lon = 107.53805;

//    lat = -6.9072; // cicadas gateway.
//    lon = 107.6456;

    QSettings config(QDir::homePath()+"/.armed20/radar.conf",QSettings::IniFormat);

    QString id = config.value("mqtt/id",false).toString();
    QString ip = config.value("mqtt/ip",false).toString();
    uint port = config.value("mqtt/port",false).toUInt();
    qDebug()<<Q_FUNC_INFO<<id<<ip<<port;

    osdSocket = new QUdpSocket(this);
    connect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);
    osdSocket->bind(QHostAddress::AnyIPv4,static_cast<quint16>(port));

    roomSocket = new QTcpSocket(this);
    connect(roomSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveTCP);
    roomSocket->connectToHost("192.168.1.177",80);

    lat = currentOwnShipLat; // jingga
    lon = currentOwnShipLon;

    ui->lineEditLat->setValidator(new QDoubleValidator(-90,90,6,ui->lineEditLat));
    ui->lineEditLon->setValidator(new QDoubleValidator(-180,180,6,ui->lineEditLon));
    ui->lineEditHDG->setValidator(new QDoubleValidator(0,360,1,ui->lineEditHDG));

    ui->lineEditLat->setText(QString::number(lat,'f',6));
    ui->lineEditLon->setText(QString::number(lon,'f',7));
    ui->lineEditHDG->setText(QString::number(hdt,'f',1));
    ui->lineEditGMT->setText(QDateTime::currentDateTimeUtc().time().toString("hh:mm:ss"));

    ui->checkBoxGPS->setChecked(gps_auto);
    ui->checkBoxHDG->setChecked(hdg_auto);

    if(hdg_auto)
    {
        ui->lineEditHDG->setEnabled(false);
        ui->lineEditHDG->setStyleSheet("color: rgb(0,255,0);");

//        connect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);
    }
    else
    {
        ui->lineEditHDG->setEnabled(true);
        ui->lineEditHDG->setStyleSheet("color: rgb(255,255,255);");

//        disconnect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);
    }

    if(gps_auto)
    {
        ui->lineEditLat->setEnabled(false);
        ui->lineEditLon->setEnabled(false);

//        connect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);
    }
    else
    {
        ui->lineEditLat->setEnabled(true);
        ui->lineEditLon->setEnabled(true);

//        disconnect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);
    }
}

void FrameOSD::on_receiveTCP()
{
    QString data(roomSocket->readAll());

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
                    ui->lineEditTemp->setText(data_list.at(1));
                    ui->lineEditHum->setText(data_list.at(0));
                    ui->lineEditTemp->setStyleSheet("color: green;");
                    ui->lineEditHum->setStyleSheet("color: green;");
                    no_room_count = 0;
                }
                else
                {
                    ui->lineEditTemp->setStyleSheet("color: red;");
                    ui->lineEditHum->setStyleSheet("color: red;");
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
void FrameOSD::on_receiveUDP()
{
    QByteArray datagram;
    while (osdSocket->hasPendingDatagrams())
    {
        datagram.resize(static_cast<quint16>(osdSocket->pendingDatagramSize()));
        osdSocket->readDatagram(datagram.data(), datagram.size());
        append_data_osd.append(QString(datagram));
    }
//    qDebug()<<Q_FUNC_INFO<<append_data_osd;
    int index_hdr = append_data_osd.indexOf("?");
    if(index_hdr >= 0)
    {
        int index_end = append_data_osd.indexOf("!");
        if(index_end >= 0)
        {
            if(index_end > index_hdr)
            {
                //?-6.939176#107.632770#31
                append_data_osd = append_data_osd.mid(index_hdr,index_end-index_hdr);
                append_data_osd.remove("!").remove("?").remove("\r").remove("\n");
//                qDebug()<<Q_FUNC_INFO<<"filter"<<append_data_osd;
                QStringList msg_list = append_data_osd.split("#",QString::SkipEmptyParts);

                if(msg_list.size() == 3)
                {
                    currentOwnShipLat = msg_list.at(0).toDouble();
                    currentOwnShipLon = msg_list.at(1).toDouble();
                    currentHeading = msg_list.at(2).toDouble();

                    ui->lineEditLat->setText(msg_list.at(0));
                    ui->lineEditLon->setText(msg_list.at(1));
                    ui->lineEditHDG->setText(msg_list.at(2));

                    no_gps_count = 0;
                    no_hdg_count = 0;
                }
                else
                    qDebug()<<Q_FUNC_INFO<<"osd invalid";

                append_data_osd.clear();
            }
            else
            {
                append_data_osd.remove(0,index_hdr);
            }
        }
//        qDebug()<<Q_FUNC_INFO<<index_end;
    }
//    qDebug()<<Q_FUNC_INFO<<index_hdr;

}
double FrameOSD::getHDT()
{
    hdt = currentHeading;
    return hdt;
}
double FrameOSD::getSPD()
{
    return spd;
}
double FrameOSD::getCOG()
{
    return cog;
}
double FrameOSD::getSOG()
{
    return sog;
}
double FrameOSD::getLat()
{
    lat = currentOwnShipLat;
    return lat;
}
double FrameOSD::getLon()
{
    lon = currentOwnShipLon;
    return lon;
}

FrameOSD::~FrameOSD()
{
    delete ui;
}
void FrameOSD::on_receive(QString msg)
{
    qDebug()<<Q_FUNC_INFO<<msg;

    if(msg.contains("gyro"))
    {
        no_hdg_count = 0;
        msg.remove("gyro>");
        qDebug()<<Q_FUNC_INFO<<"hdg"<<msg;
        ui->lineEditHDG->setText(msg);
        currentHeading = msg.toDouble();
    }
    else if(msg.contains("gps>") )
    {
        no_gps_count = 0;
        qDebug()<<Q_FUNC_INFO<<"lat"<<msg;
        msg.remove("gps>");
        QStringList gpsData = msg.split("#");
        ui->lineEditLat->setText(gpsData.at(0));
        ui->lineEditLon->setText(gpsData.at(1));
        currentOwnShipLat = gpsData.at(0).toDouble();
        currentOwnShipLon = gpsData.at(1).toDouble();

    }

}

void FrameOSD::on_timeout()
{
    ui->lineEditGMT->setText(QDateTime::currentDateTime().time().toString("hh:mm:ss"));

    if(hdg_auto)
    {
        bool hdg_col_normal_buf;
        no_hdg_count++;
        if(no_hdg_count>200)
            no_hdg_count = 11;

        if(no_hdg_count>10)
            hdg_col_normal_buf = false;
        else
            hdg_col_normal_buf = true;

        if(hdg_col_normal_buf^hdg_col_normal)
        {
            hdg_col_normal = hdg_col_normal_buf;
            if(hdg_col_normal)
            {
                ui->lineEditHDG->setStyleSheet("color: rgb(0,255,0);");
            }
            else
            {
                ui->lineEditHDG->setStyleSheet("color: rgb(255,0,0);");
            }
        }
    }

    no_room_count++;
    if(no_room_count>200)
        no_room_count = 11;

    if(no_room_count>10)
    {
        ui->lineEditTemp->setStyleSheet("color: red;");
        ui->lineEditHum->setStyleSheet("color: red;");
    }

    if(gps_auto)
    {
        bool gps_col_normal_buf;
        no_gps_count++;
        if(no_gps_count>200)
            no_gps_count = 11;

        if(no_gps_count>10)
            gps_col_normal_buf = false;
        else
            gps_col_normal_buf = true;

        if(gps_col_normal_buf^gps_col_normal)
        {
            gps_col_normal = gps_col_normal_buf;
            if(gps_col_normal)
            {
                ui->lineEditLat->setStyleSheet("color: rgb(0,255,0);");
                ui->lineEditLon->setStyleSheet("color: rgb(0,255,0);");
            }
            else
            {
                ui->lineEditLat->setStyleSheet("color: rgb(255,0,0);");
                ui->lineEditLon->setStyleSheet("color: rgb(255,0,0);");
            }
        }

    }

    if(osdSocket->state() != QAbstractSocket::BoundState)
    {
        QSettings config(QDir::homePath()+"/.armed20/radar.conf",QSettings::IniFormat);

        QString id = config.value("mqtt/id",false).toString();
        QString ip = config.value("mqtt/ip",false).toString();
        uint port = config.value("mqtt/port",false).toUInt();
        qDebug()<<Q_FUNC_INFO<<id<<ip<<port;

        osdSocket->bind(QHostAddress::AnyIPv4,static_cast<quint16>(port));
    }

    if(roomSocket->state() != QAbstractSocket::ConnectedState)
        roomSocket->connectToHost("192.168.1.177",80);
//    qDebug()<<Q_FUNC_INFO;

}

void FrameOSD::on_pushButtonApply_clicked()
{
    double currentHeading_buf = ui->lineEditHDG->text().toDouble();
    double currentOwnShipLat_buf = ui->lineEditLat->text().toDouble();
    double currentOwnShipLon_buf = ui->lineEditLon->text().toDouble();

    if(currentHeading_buf >=0 && currentHeading_buf <= 360)
        currentHeading = currentHeading_buf;
    else
        ui->lineEditHDG->setText(QString::number(currentHeading,'f',1));

    if(currentOwnShipLat_buf >=-90 && currentOwnShipLat_buf <= 90)
        currentOwnShipLat = currentOwnShipLat_buf;
    else
        ui->lineEditLat->setText(QString::number(currentOwnShipLat,'f',6));

    if(currentOwnShipLon_buf >=-180 && currentOwnShipLon_buf <= 180)
        currentOwnShipLon = currentOwnShipLon_buf;
    else
        ui->lineEditLon->setText(QString::number(currentOwnShipLon,'f',6));

    hdg_auto = ui->checkBoxHDG->isChecked();
    cur_hdg_auto = hdg_auto;
    gps_auto = ui->checkBoxGPS->isChecked();

    no_hdg_count = 0;
    no_gps_count = 0;

    if(hdg_auto)
    {
        ui->lineEditHDG->setEnabled(false);
        ui->lineEditHDG->setStyleSheet("color: rgb(0,255,0);");

//        connect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);

    }
    else
    {
        ui->lineEditHDG->setEnabled(true);
        ui->lineEditHDG->setStyleSheet("color: rgb(255,255,255);");

//        disconnect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);
    }

    if(gps_auto)
    {
        ui->lineEditLat->setEnabled(false);
        ui->lineEditLon->setEnabled(false);
        ui->lineEditLat->setStyleSheet("color: rgb(0,255,0);");
        ui->lineEditLon->setStyleSheet("color: rgb(0,255,0);");

//        connect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);
    }
    else
    {
        ui->lineEditLat->setEnabled(true);
        ui->lineEditLon->setEnabled(true);
        ui->lineEditLat->setStyleSheet("color: rgb(255,255,255);");
        ui->lineEditLon->setStyleSheet("color: rgb(255,255,255);");

//        disconnect(osdSocket,&QUdpSocket::readyRead,this,&FrameOSD::on_receiveUDP);
    }
}
