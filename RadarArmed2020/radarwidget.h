#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTimer>

#include <radar-engine-armed/radarengine_global.h>
#include <radar-engine-armed/radarengine.h>
//#include "qtmosq.h"


class RadarWidget : public QGLWidget
{
    Q_OBJECT

public:
    RadarWidget(QWidget *parent = 0, RI *ri = 0, RI *ri1=0);
    ~RadarWidget();

    void setRectRegoin(QRect rect);
    void setRange(int range);
    void computetRingWidth();
    int getRange();
    double getRingWidth();
    void trigger_shutdown();

signals:
    void signal_target_param(int id,double spd, double crs, double lat, double lon);
    void signal_adsb_param(QString icao,double spd, double crs, double lat, double lon,double alt);
    void signal_cursorMove(double range,double brn);

protected:
    void initializeGL();
    void paintEvent(QPaintEvent *event);
    void resizeGL(int width, int height);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void timeOut();
    void trigger_DrawSpoke(int transparency, int angle, u_int8_t* data, size_t len);
    void trigger_DrawSpoke1(int transparency, int angle, u_int8_t* data, size_t len);
    void trigger_ReqDelTrack(int id);

private:
    void setupViewport(int width, int height);
    void createMARPA(QPoint pos);

    RD *spokeDrawer;
    RD *spokeDrawer1;
    RI *m_ri;
    RI *m_ri1;
    RA *arpa;
    QTimer *timer;
    QRect region;

    double ringWidth;
    int curRange;
    int cur_arpa_id_count;
    int cur_arpa_number;
    quint64 arpa_measure_time;
    bool old_motion_mode;

    int old_x1;
    int old_y1;

    int counter;
};


#endif
