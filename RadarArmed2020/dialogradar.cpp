#include "dialogradar.h"
#include "ui_dialogradar.h"
#include <radarengine_global.h>

DialogRadar::DialogRadar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRadar)
{
    ui->setupUi(this);
    ui->tabWidget->tabBar()->setStyleSheet("QTabBar::tab {color:black;}");
    ui->checkBoxShowARPABottom->setChecked(arpa_settings[0].show);
    ui->checkBoxShowARPATop->setChecked(arpa_settings[1].show);

    ui->lineEditIPDataTop->setValidator(new QIntValidator(0,255,ui->lineEditIPDataTop));
    ui->lineEditPortDataTop->setValidator(new QIntValidator(3000,65536,ui->lineEditPortDataTop));
    ui->lineEditIPReportTop->setValidator(new QIntValidator(0,255,ui->lineEditIPReportTop));
    ui->lineEditPortReportTop->setValidator(new QIntValidator(3000,65536,ui->lineEditPortReportTop));
    ui->lineEditIPcmdTop->setValidator(new QIntValidator(0,255,ui->lineEditIPcmdTop));
    ui->lineEditPortcmdTop->setValidator(new QIntValidator(3000,65536,ui->lineEditPortcmdTop));

    ui->lineEditIPDataTop->setText(radar_settings.ip_data1);
    ui->lineEditPortDataTop->setText(QString::number(radar_settings.port_data1));
    ui->lineEditIPReportTop->setText(radar_settings.ip_report1);
    ui->lineEditPortReportTop->setText(QString::number(radar_settings.port_report1));
    ui->lineEditIPcmdTop->setText(radar_settings.ip_command1);
    ui->lineEditPortcmdTop->setText(QString::number(radar_settings.port_command1));

    ui->lineEditIPDataBottom->setValidator(new QIntValidator(0,255,ui->lineEditIPDataBottom));
    ui->lineEditPortDataBottom->setValidator(new QIntValidator(3000,65536,ui->lineEditPortDataBottom));
    ui->lineEditIPReportBottom->setValidator(new QIntValidator(0,255,ui->lineEditIPReportBottom));
    ui->lineEditPortReportBottom->setValidator(new QIntValidator(3000,65536,ui->lineEditPortReportBottom));
    ui->lineEditIPcmdBottom->setValidator(new QIntValidator(0,255,ui->lineEditIPcmdBottom));
    ui->lineEditPortcmdBottom->setValidator(new QIntValidator(3000,65536,ui->lineEditPortcmdBottom));

    ui->lineEditIPDataBottom->setText(radar_settings.ip_data);
    ui->lineEditPortDataBottom->setText(QString::number(radar_settings.port_data));
    ui->lineEditIPReportBottom->setText(radar_settings.ip_report);
    ui->lineEditPortReportBottom->setText(QString::number(radar_settings.port_report));
    ui->lineEditIPcmdBottom->setText(radar_settings.ip_command);
    ui->lineEditPortcmdBottom->setText(QString::number(radar_settings.port_command));
}

DialogRadar::~DialogRadar()
{
    delete ui;
}

void DialogRadar::on_checkBoxShowARPABottom_clicked(bool checked)
{
    arpa_settings[0].show = checked;
}

void DialogRadar::on_checkBoxShowARPATop_clicked(bool checked)
{
    arpa_settings[1].show = checked;
}

void DialogRadar::on_pushButtonApplyTop_clicked()
{
    radar_settings.ip_data1 = ui->lineEditIPDataTop->text().remove(" ");
    radar_settings.port_data1 = ui->lineEditPortDataTop->text().toUInt();
    radar_settings.ip_report1 = ui->lineEditIPReportTop->text().remove(" ");
    radar_settings.port_report1 = ui->lineEditPortReportTop->text().toUInt();
    radar_settings.ip_command1 = ui->lineEditIPcmdTop->text().remove(" ");
    radar_settings.port_command1 = ui->lineEditPortcmdTop->text().toUInt();
    radar_settings.enable1 = ui->checkBoxEnableTop->isChecked();

    emit signal_settingChange1();
}

void DialogRadar::on_pushButtonApplyBottom_clicked()
{
    radar_settings.ip_data = ui->lineEditIPDataBottom->text().remove(" ");
    radar_settings.port_data = ui->lineEditPortDataBottom->text().toUInt();
    radar_settings.ip_report = ui->lineEditIPReportBottom->text().remove(" ");
    radar_settings.port_report = ui->lineEditPortReportBottom->text().toUInt();
    radar_settings.ip_command = ui->lineEditIPcmdBottom->text().remove(" ");
    radar_settings.port_command = ui->lineEditPortcmdBottom->text().toUInt();
    radar_settings.enable = ui->checkBoxEnableBottom->isChecked();

    emit signal_settingChange();

}
