#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include <radarengine.h>
#include "radarwidget.h"
#include "gzdialog.h"
#include "dialogalarm.h"
#include "traildialog.h"
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
    RI *ri1;

//public slots:

protected:
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void trigger_antenaFeedback();
    void trigger_rangeChange(int range);
    void trigger_gainChange(int value);
    void trigger_seaChange(int value);
    void trigger_rainChange(int value);
    void trigger_shutdown();
    void trigger_rangeChangeReq(int range);
    void trigger_ReqRadarSetting();
    void trigger_ReqRadarSetting1();
    void trigger_RadarTx();
    void trigger_changeAntena(QString sig);
    void trigger_changeAntenaMan();
    void trigger_PPIFullReq();

    void timerTimeout();

    void on_pushButtonSetGZ_clicked();

    void on_pushButtonSetTrail_clicked();

    void on_pushButtonRadar_clicked();

    void on_pushButtonTilting_clicked();

private:
    Ui::MainWindow *ui;
    GZDialog *dialogGz;
    DialogAlarm *dialAlarm;
    TrailDialog *dialTrail;
    DialogRadar *dialRadar;
    RadarWidget *radarWidget;
    radarTransmit *rt,*rt1;
    QTimer *timer;
    QTcpSocket *sockAntena;
    QString cur_antena;

    int curRange;
    int range_from_radar;
    RadarState curState,curState1;
};

#endif // MAINWINDOW_H
