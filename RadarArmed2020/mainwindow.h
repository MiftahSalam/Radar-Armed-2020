#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <radar-engine-armed/radarengine.h>
#include "radarwidget.h"
#include "gzdialog.h"
#include "dialogalarm.h"
#include "traildialog.h"
#include "adsbdialog.h"
#include "dialogradar.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    RI *ri;

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void trigger_rangeChange(int range);
    void trigger_gainChange(int value);
    void trigger_seaChange(int value);
    void trigger_rainChange(int value);
    void trigger_shutdown();
    void trigger_rangeChangeReq(int range);
    void trigger_ReqRadarSetting();

    void timerTimeout();

    void on_pushButtonSetGZ_clicked();

    void on_pushButtonSetTrail_clicked();

    void on_pushButtonSetADSB_clicked();

    void on_pushButtonRadar_clicked();

private:
    Ui::MainWindow *ui;
    GZDialog *dialogGz;
    DialogAlarm *dialAlarm;
    TrailDialog *dialTrail;
    adsbDialog *dialADSB;
    DialogRadar *dialRadar;
    RadarWidget *radarWidget;
    radarTransmit *rt;
    QTimer *timer;

    int curRange;
    int range_from_radar;
    RadarState curState;
};

#endif // MAINWINDOW_H
