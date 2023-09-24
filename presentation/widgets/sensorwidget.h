#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMap>
#include <QDebug>
#include "sensorlabel.h"
#include "boats.h"

namespace Ui {
class sensorWidget;
}

class sensorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit sensorWidget(QWidget *parent = nullptr);
    ~sensorWidget();
    void setBoatList(Boats* boatlist);
public slots:
    void addWidget(QString name, int BoatID, int DeviceID);
    void info_button_pushed();
    void subscribeSensor(int BoatID, int BoardID, int PinID, char unit);

signals:

    void sendMsg(QString boatname, QString msg, int PCPort);

private:
    Ui::sensorWidget *ui;
    int connection_counter;
    int costumSensorCount;
    Boats *boatList;
    QMap<int,SensorLabel*> labelMap;

};

#endif // SENSORWIDGET_H
