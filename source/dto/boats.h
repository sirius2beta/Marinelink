#ifndef BOATS_H
#define BOATS_H

#include <QWidget>
#include <QDebug>
#include "device.h"
#include "basedto.h"



class Boat{
public:
    QString name;
    int id;
    QString PIP;
    QString SIP;
    QString CurrentIP;
    int OS;
    int addDevice(Device& d);
    Peripheral getPeriperalbyID(int ID);
    QList<Peripheral> peripherals;
    QList<Device> devices;
    //Device& getDevbyID(int ID);
};

class Boats
{
public:
    Boats();
    ~Boats();
    Boat* addBoat(int ID);
    void deleteBoat(int ID);
    Boat* getBoatbyName(QString boatname);
    Boat* getBoatbyIndex(int index);
    Boat* getBoatbyID(int ID);

    QString CurrentIP(QString boatname);
    int size();

private:
    QVector<Boat*> boatList;
};

#endif // BOATS_H
