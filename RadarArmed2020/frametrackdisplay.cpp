#include "frametrackdisplay.h"
#include "ui_frametrackdisplay.h"

#include <QDebug>
#include <QDateTime>
#include <QMenu>
#include <QSettings>
#include <QDir>

FrameTrackDisplay::FrameTrackDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameTrackDisplay)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerTimeout()));

    model = new QStandardItemModel(this);
    model->setColumnCount(5);

    dataCount_mqtt = 0;

    QSettings config(QDir::homePath()+"/.armed20/radar.conf",QSettings::IniFormat);

    QString ip = config.value("server/ip","127.0.0.1").toString();
    uint port = config.value("server/port",1883).toUInt();
    qDebug()<<Q_FUNC_INFO<<ip<<port;

    udpSocket = new QUdpSocket(this);

    ui->tableViewTrack->setModel(model);

    timer->start(1000);
}

void FrameTrackDisplay::timerTimeout()
{
    QHashIterator<int,quint64> i(target_time_tag_list);
    QList<int> target_to_delete;
    quint64 now = QDateTime::currentMSecsSinceEpoch();

    target_to_delete.clear();
    while(i.hasNext())
    {
        i.next();
        if(now-i.value()>5000)
            target_to_delete.append(i.key());
    }

    for(int i=0;i<target_to_delete.size();i++)
    {
        target_time_tag_list.remove(target_to_delete.at(i));

        QList<QStandardItem *> listTarget = model->findItems(QString::number(target_to_delete.at(i)),0);
        if(!listTarget.isEmpty())
        {
            int row = listTarget.at(0)->row();
            model->removeRow(row);
        }
    }


    if(model->rowCount()>0 && model->rowCount()>dataCount_mqtt)
    {
        QString id,rng,brn,spd,crs,mq_data;
        QModelIndex index = model->index(dataCount_mqtt,0);
        QByteArray mq_databyte;

        id = model->data(index).toString();
        index = model->index(dataCount_mqtt,1);
        rng = model->data(index).toString();
        index = model->index(dataCount_mqtt,2);
        brn = model->data(index).toString();
        index = model->index(dataCount_mqtt,3);
        spd = model->data(index).toString();
        index = model->index(dataCount_mqtt,4);
        crs = model->data(index).toString();

        mq_data = id+"#"+rng+"#"+brn+"#"+spd+"#"+crs;
        mq_databyte = mq_data.toUtf8();
        udpSocket->writeDatagram(mq_databyte,QHostAddress(serverUdpIP),serverUdpPort);

        dataCount_mqtt++;
        //            qDebug()<<"dataCount_mqtt1"<<dataCount_mqtt;
    }


    if(dataCount_mqtt==model->rowCount())
    {
//        qDebug()<<"dataCount_mqtt2"<<dataCount_mqtt;
        dataCount_mqtt = 0;
    }
    else if(dataCount_mqtt>model->rowCount())
    {
//        qDebug()<<"dataCount_mqtt3"<<dataCount_mqtt;
        dataCount_mqtt = model->rowCount() - 1;
        if(dataCount_mqtt<1)
            dataCount_mqtt = 0;
    }




}

void FrameTrackDisplay::trigger_target_update(int id, double spd, double crs, double rng, double brn)
{

    quint64 new_target_tt;
    if(target_time_tag_list.contains(id))
    {
        new_target_tt = target_time_tag_list.value(id);
        new_target_tt = QDateTime::currentMSecsSinceEpoch();
        target_time_tag_list.remove(id);
        target_time_tag_list.insert(id,new_target_tt);

        QList<QStandardItem *> listTarget = model->findItems(QString::number(id),0);
        if(!listTarget.isEmpty())
        {
            int row = listTarget.at(0)->row();
            model->setData(model->index(row,0,QModelIndex()),
                           id);
            model->setData(model->index(row,1,QModelIndex()),
                           QString::number(rng,'f',1));
            model->setData(model->index(row,2,QModelIndex()),
                           QString::number(brn,'f',1));
            model->setData(model->index(row,3,QModelIndex()),
                           QString::number(spd,'f',1));
            model->setData(model->index(row,4,QModelIndex()),
                           QString::number(crs,'f',1));
        }
        else
            insertList(id, spd, crs, rng, brn);
    }
    else
    {
        target_time_tag_list.insert(id,QDateTime::currentMSecsSinceEpoch());
        insertList(id, spd, crs, rng, brn);
    }
}
void FrameTrackDisplay::insertList(int id, double spd, double crs, double rng, double brn)
{
    model->insertRow(model->rowCount(),QModelIndex());
    model->setData(model->index(model->rowCount()-1,0,QModelIndex()),
                   id);
    model->setData(model->index(model->rowCount()-1,1,QModelIndex()),
                   QString::number(rng,'f',1));
    model->setData(model->index(model->rowCount()-1,2,QModelIndex()),
                   QString::number(brn,'f',1));
    model->setData(model->index(model->rowCount()-1,3,QModelIndex()),
                   QString::number(spd,'f',1));
    model->setData(model->index(model->rowCount()-1,4,QModelIndex()),
                   QString::number(crs,'f',1));

    model->item(model->rowCount()-1,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    model->item(model->rowCount()-1,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    model->item(model->rowCount()-1,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    model->item(model->rowCount()-1,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    model->item(model->rowCount()-1,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}

FrameTrackDisplay::~FrameTrackDisplay()
{
    delete ui;
}

void FrameTrackDisplay::on_pushButtonDelSel_clicked()
{
    int row_count = model->rowCount();
    if(row_count>0)
    {
        int row = ui->tableViewTrack->currentIndex().row();
        int id = model->index(row,0).data().toInt();

        emit signal_request_del_track(id);
    }
}

void FrameTrackDisplay::on_pushButtonDelAll_clicked()
{
    int row_count = model->rowCount();
    if(row_count>0)
        emit signal_request_del_track(-100);
}
