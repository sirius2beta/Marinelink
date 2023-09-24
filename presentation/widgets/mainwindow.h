#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include <QMainWindow>
#include <QSettings>
#include <QDebug>
#include <QDockWidget>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QUdpSocket>
#include <QThread>
#include <QByteArray>
#include "boatsetting.h"
#include "sensorwidget.h"
#include "networksettings.h"
#include "boats.h"
#include "controlwidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class HeartBeat : public QObject
{
    Q_OBJECT
public:
    explicit HeartBeat(QObject *parent = nullptr);
    HeartBeat(QString boatname, int ID, QString PC_ip, QString Boat_ip, int port, bool isPrimary, QObject *parent = nullptr);
    void HeartBeatLoop(); //工作函数
    ~HeartBeat();
    void setPCIP(QString PC_ip);

signals:
    void sendMsg(QString boatname, char topic, QByteArray command);
    void connected(int ID, bool isprimary);
    void disconnected(int ID, bool isprimary);

public slots:
    void beat();
    void checkAlive();
    void alive(QString ip);
    void resetBoatName(QString boatname, QString newname);
    void onChangeIP(QString boatname, QString PIP, QString SIP);
    void onPCPIPChanged(QString IP);
    void onPCSIPChanged(QString IP);

    void onDeleteBoat(QString boatname);

private:
    bool run;
    QTimer *heartBeatTimer;
    QTimer *checkAliveTimer;
    QString boatName;
    int boatID;
    QString boatIP;
    int boatPort;
    QString PCIP;
    bool isAlive;
    bool isHearBeatLoop;
    bool primary;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, QString config=QString());
    ~MainWindow();

    void initBoatSettings();
    void initVideoWindows();
    void initNetworkSettings();
    void initSensorWidget();
    void initControlWidget();
    QMap<QString,QString> USV1format;
    QMap<QString,QString> USV2format;
signals:
    void updateFormat(QMap<QString,QString> format);
    void AliveResponse(QString ip);
    void setFormat(int ID, QStringList format);
    void changePCPIP(QString ip);
    void changePCSIP(QString ip);
    void sensorMsg(QByteArray data);

protected slots:
    void openCreateWindowDialog();
    void addVideoWindow(int index, bool central_widget);
    void sendUDPCommand(QString boatname, QString command, int PCPort);
    void sendMsg(QString IP, char topic, QByteArray command);
    void onUDPMsg();
    void setConfig(QString config);
    void onPCPIPChanged(QString PCIP);
    void onPCSIPChanged(QString PCIP);
    void onConnected(int ID, bool isprimary);
    void onDisonnected(int ID, bool isprimary);
    void onNewBoat(QString boatname, int ID, QString PIP, QString SIP);


private:
    Ui::MainWindow *ui;
    QSettings* settings;
    NetworkSettings* networkSettings;
    BoatSetting* boatSetting;
    struct mosquitto *mosq;
    sensorWidget* sensor_widget;
    ControlWidget* control_widget;
    QUdpSocket *serverSocket;
    QUdpSocket *clientSocket;
    QString _config;
    HeartBeat* primaryHeartBeat;
    HeartBeat* secondaryHeartBeat;
    QString PCPIP;
    QString BoatPIP;
    QString PCSIP;
    QString BoatSIP;
    Boats* boatList;


    char HEARTBEAT = 0x10;

};
#endif // MAINWINDOW_H
