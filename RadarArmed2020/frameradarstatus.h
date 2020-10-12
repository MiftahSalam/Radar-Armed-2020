#ifndef FRAMERADARSTATUS_H
#define FRAMERADARSTATUS_H

#include <QFrame>

namespace Ui {
class FrameRadarStatus;
}

class FrameRadarStatus : public QFrame
{
    Q_OBJECT

public:
    explicit FrameRadarStatus(QWidget *parent = nullptr);
    ~FrameRadarStatus();

    void updateStatus();
    void updateAntena(QString data);

public slots:
    void trigger_reportR1();
    void trigger_reportR2();

private:
    Ui::FrameRadarStatus *ui;

    QString append_data_osd;
    int reportCounter1,reportCounter2,no_anten_count;

};

#endif // FRAMERADARSTATUS_H
