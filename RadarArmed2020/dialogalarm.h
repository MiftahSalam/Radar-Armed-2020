#ifndef DIALOGALARM_H
#define DIALOGALARM_H

#include <QDialog>

namespace Ui {
class DialogAlarm;
}

class DialogAlarm : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAlarm(QWidget *parent = 0);
    ~DialogAlarm();

    void setSpotCount(int count);

signals:
    void signal_confirmed();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogAlarm *ui;
};

#endif // DIALOGALARM_H
