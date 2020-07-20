#ifndef FRAMEADSB_H
#define FRAMEADSB_H

#include <QFrame>
#include <QStandardItemModel>
#include <QTimer>

#include "qtmosq.h"

namespace Ui {
class FrameADSB;
}

class FrameADSB : public QFrame
{
    Q_OBJECT

public:
    explicit FrameADSB(QWidget *parent = 0);
    ~FrameADSB();

private slots:
    void trigger_target_update(QString icao, double spd, double crs, double rng, double brn,double alt);
    void timerTimeout();

private:
    Ui::FrameADSB *ui;
    QTimer *timer;
    qtmosq *m_mqtt;
    int dataCount_mqtt;
    QHash<QString,quint64> target_time_tag_list;
    QStandardItemModel *model;

    void insertList(QString icao,double spd, double crs, double rng, double brn, double alt);
};

#endif // FRAMEADSB_H
