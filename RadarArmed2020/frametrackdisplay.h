#ifndef FRAMETRACKDISPLAY_H
#define FRAMETRACKDISPLAY_H

#include <QFrame>
#include <QStandardItemModel>
#include <QTimer>

#include "qtmosq.h"

namespace Ui {
class FrameTrackDisplay;
}

class FrameTrackDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit FrameTrackDisplay(QWidget *parent = 0);
    ~FrameTrackDisplay();

signals:
    void signal_request_del_track(int id);

private slots:
    void trigger_target_update(int id,double spd, double crs, double rng, double brn);
    void timerTimeout();

    void on_pushButtonDelSel_clicked();

    void on_pushButtonDelAll_clicked();

private:
    Ui::FrameTrackDisplay *ui;
    QTimer *timer;
    qtmosq *m_mqtt;
    int dataCount_mqtt;
    QHash<int,quint64> target_time_tag_list;
    QStandardItemModel *model;

    void insertList(int id,double spd, double crs, double rng, double brn);
};

#endif // FRAMETRACKDISPLAY_H
