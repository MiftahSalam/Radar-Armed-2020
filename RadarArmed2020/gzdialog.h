#ifndef GZDIALOG_H
#define GZDIALOG_H

#include <QDialog>

namespace Ui {
class GZDialog;
}

class GZDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GZDialog(QWidget *parent = 0);
    ~GZDialog();

private slots:
    void on_checkBoxShowGZ_clicked(bool checked);

    void on_checkBoxAlarmGZ_clicked(bool checked);

    void on_radioButtonArc_clicked(bool checked);

    void on_radioButtonCircle_clicked(bool checked);

    void on_lineEditInner_textChanged(const QString &arg1);

    void on_lineEditOuter_textChanged(const QString &arg1);

    void on_lineEditStart_textChanged(const QString &arg1);

    void on_lineEditEnd_textChanged(const QString &arg1);

    void on_lineEditNotifTHR_textChanged(const QString &arg1);

private:
    Ui::GZDialog *ui;
};

#endif // GZDIALOG_H
