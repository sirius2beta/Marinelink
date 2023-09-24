#include "networksettings.h"
#include "ui_networksettings.h"

NetworkSettings::NetworkSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkSettings),
    config('1')
{
    ui->setupUi(this);
    settings = new QSettings(this);

    connect(ui->p_pc_ip_edit, &QLineEdit::editingFinished, this, &NetworkSettings::onPCPIPChanged);
    connect(ui->s_pc_ip_edit, &QLineEdit::editingFinished, this, &NetworkSettings::onPCSIPChanged);
}

NetworkSettings::~NetworkSettings()
{
    delete ui;
}

void NetworkSettings::setConfig(QString _config)
{
    config = _config;
}

void NetworkSettings::setIP(QString PCPIP, QString PCSIP)
{
    ui->p_pc_ip_edit->setText(PCPIP);
    ui->s_pc_ip_edit->setText(PCSIP);

}

void NetworkSettings::onPCPIPChanged()
{
    settings->setValue(QString("%1/PCPIP").arg(config),ui->p_pc_ip_edit->text());
    emit PIPChanged(ui->p_pc_ip_edit->text());


}

void NetworkSettings::onPCSIPChanged()
{
    settings->setValue(QString("%1/PCSIP").arg(config),ui->s_pc_ip_edit->text());
    emit SIPChanged(ui->s_pc_ip_edit->text());

}
