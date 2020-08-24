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

    void on_checkBoxShowGZ_2_clicked(bool checked);

    void on_checkBoxAlarmGZ_2_clicked(bool checked);

    void on_radioButtonArc_2_clicked(bool checked);

    void on_radioButtonCircle_2_clicked(bool checked);

    void on_lineEditInner_2_textChanged(const QString &arg1);

    void on_lineEditOuter_2_textChanged(const QString &arg1);

    void on_lineEditStart_2_textChanged(const QString &arg1);

    void on_lineEditEnd_2_textChanged(const QString &arg1);

    void on_lineEditNotifTHR_2_textChanged(const QString &arg1);

    void on_checkBoxShowGZ_3_clicked(bool checked);

    void on_checkBoxAlarmGZ_3_clicked(bool checked);

    void on_radioButtonArc_3_clicked(bool checked);

    void on_radioButtonCircle_3_clicked(bool checked);

    void on_lineEditInner_3_textChanged(const QString &arg1);

    void on_lineEditOuter_3_textChanged(const QString &arg1);

    void on_lineEditStart_3_textChanged(const QString &arg1);

    void on_lineEditEnd_3_textChanged(const QString &arg1);

    void on_lineEditNotifTHR_3_textChanged(const QString &arg1);

private:
    Ui::GZDialog *ui;
};

#endif // GZDIALOG_H
