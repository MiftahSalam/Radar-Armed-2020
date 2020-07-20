#ifndef ADSBDIALOG_H
#define ADSBDIALOG_H

#include <QDialog>

namespace Ui {
class adsbDialog;
}

class adsbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit adsbDialog(QWidget *parent = 0);
    ~adsbDialog();

signals:
    void signal_settingChange();

private slots:

    void on_checkBoxShow_clicked(bool checked);

    void on_timeout();

    void on_pushButtonApply_clicked();

private:
    Ui::adsbDialog *ui;

    bool connected;
};

#endif // ADSBDIALOG_H
