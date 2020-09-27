#ifndef DIALOGRADAR_H
#define DIALOGRADAR_H

#include <QDialog>

namespace Ui {
class DialogRadar;
}

class DialogRadar : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogRadar(QWidget *parent = nullptr);
    ~DialogRadar();
    
signals:
    void signal_settingChange();
    void signal_settingChange1();

private slots:

    void on_checkBoxShowARPABottom_clicked(bool checked);

    void on_checkBoxShowARPATop_clicked(bool checked);

    void on_pushButtonApplyTop_clicked();

    void on_pushButtonApplyBottom_clicked();

private:
    Ui::DialogRadar *ui;
};

#endif // DIALOGRADAR_H
