#include "device.h"

Device::Device(): PeriID(-1),type(0)
{
    pinIDList = {};
}

Device::Device(const Device &d)
{
    this->ID = d.ID;
    this->deviceName = d.deviceName;
    this->PeriID = d.PeriID;
    this->settings = d.settings;
    this->type = d.type;
    this->pinIDList = d.pinIDList;
}

int Device::addPin(int boardID, int pinID)
{
    //return UID
    pinIDList.append(pinID);
    return boardID*256+pinID;
}
/*
int Device::addPin(int UID)
{
    //return UID
    pinUIDList.append(UID);
    return UID;
}


bool Device::operator== (const Device& lDev)
{
    if(this->PeriID == lDev.PeriID){
        return true;
    }
    return false;
}
*/
Device::~Device()
{
}


QString Device::typeName()
{
    if(this->type == DEV::SENSOR){
        return "sensor";
    }else if(this->type == DEV::SERVO){
        return "servo";
    }else if(this->type == DEV::STEPPER){
        return "stepper";
    }

    return "unknown";

}

Peripheral::Peripheral(): ID(0), boardName(QString()), boardType(QString())
{
}

Peripheral::Peripheral(const Peripheral &p)
{
    this->ID = p.ID;
    this->boardName = p.boardName;
    this->boardType = p.boardType;
    this->pinSelected = p.pinSelected;
}


