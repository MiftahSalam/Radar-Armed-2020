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

private:
    Ui::FrameRadarStatus *ui;
};

#endif // FRAMERADARSTATUS_H
