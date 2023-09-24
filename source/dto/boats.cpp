#include "boats.h"
int Boat::addDevice(Device &d)
{
    int index = 0;
    while(true){
        for(int i = 0; i<devices.size(); i++){
            if(devices[i].ID == index){
                index++;
                continue;
            }
        }
        break;
    }
    d.ID = index;
    devices.append(d);
    return index;

}

Boats::Boats()
{

}

Boats::~Boats()
{
    if(!boatList.empty()){
        for(int i = 0; i < boatList.size(); i++){
            delete boatList[i];
        }
    }

}

Boat* Boats::addBoat(int ID)
{
    Boat* boat = new Boat;
    boat->id = ID;
    boatList.append(boat);
    return boat;
}

void Boats::deleteBoat(int ID)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->id == ID){
            boatList.remove(i);
        }
    }
}
Boat* Boats::getBoatbyName(QString boatname)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->name == boatname){

            return boatList[i];
        }
    }
    return 0;
}
Boat* Boats::getBoatbyIndex(int index)
{
    if(index > boatList.size()){
        return 0;
    }
    return boatList[index];
}

Boat* Boats::getBoatbyID(int ID)
{

    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->id == ID){

            return boatList[i];
        }
    }

    return 0;
}
/*
Device& Boat::getDevbyID(int ID)
{

    for(int i = 0; i<devices.size(); i++){
        if(devices[i].ID == ID){
            return devices[i];
        }
    }
}
*/

QString Boats::CurrentIP(QString boatname)
{
    for(int i = 0; i<boatList.size(); i++){
        if(boatList[i]->name == boatname){
            return boatList[i]->CurrentIP;
        }
    }
    return QString();
}

int Boats::size()
{
    return boatList.size();
}

Peripheral Boat::getPeriperalbyID(int ID)
{

    for(int i = 0; i<peripherals.size(); i++){
        if(peripherals[i].ID == ID){
            return peripherals[i];
        }
    }

    return Peripheral();
}
