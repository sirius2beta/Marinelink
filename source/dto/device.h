#ifndef DEVICE_H
#define DEVICE_H
#include <QWidget>
#include <QDebug>
#include "basedto.h"

class Component
{
public:
    Component();
    Component(const Component& c);
    int periID;
    int comID;
    int type;
    QList<int> pinList;
    QStringList setting;

};

class Device
{
public:
    Device();
    Device(const Device &d);
    ~Device();
    int ID;
    int PeriID; //pin ID
    QString deviceName;
    int type;
    QStringList settings;

    QList<int> pinIDList;

    int addPin(int boardID, int pinID);
    QString typeName();

    //bool operator== (const Device& lDev);

    enum DEV {SENSOR = 0, SERVO = 1, STEPPER=2};

};

class Peripheral{
public:
    Peripheral();
    Peripheral(const Peripheral &p);
    QList<int> pinSelected;
    int ID;
    QString boardName;
    QString boardType;
};

#endif // DEVICE_H
