#ifndef NETWORKSETTINGS_H
#define NETWORKSETTINGS_H

#include <QSettings>
#include <QWidget>

namespace Ui {
class NetworkSettings;
}

class NetworkSettings : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkSettings(QWidget *parent = nullptr);
    ~NetworkSettings();
    void setConfig(QString _config);
    void setIP(QString PCPIP, QString PCSIP);
signals:
    void PIPChanged(QString PCIP);
    void SIPChanged(QString PCIP);
protected slots:
    void onPCPIPChanged();
    void onPCSIPChanged();

private:
    Ui::NetworkSettings *ui;
    QSettings* settings;
    QString config;
};

#endif // NETWORKSETTINGS_H
