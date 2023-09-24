#include "sensorwidget.h"
#include "ui_sensorwidget.h"

sensorWidget::sensorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sensorWidget),
    connection_counter(0),
    costumSensorCount(0)
{
    ui->setupUi(this);
    connect(ui->info_Button, &QPushButton::clicked, this, &sensorWidget::info_button_pushed);
    addWidget("temp", 1, 0);
    addWidget("dist",1 ,1);
    addWidget("dist2",1,2);
    addWidget("dir",1,3);

}

sensorWidget::~sensorWidget()
{
    delete ui;
}

void sensorWidget::setBoatList(Boats* boatlist){
    this->boatList = boatlist;
}



void sensorWidget::addWidget(QString name, int BoatID, int DeviceID)
{

    int H = costumSensorCount/3;
    int W = costumSensorCount%3;
    SensorLabel* s = new SensorLabel(this);
    ui->gridLayout->addWidget(s,H,W);
    s->setSensorName(name);
    s->setValue("25");
    int uid = 255*BoatID + DeviceID;
    labelMap[uid] = s;
    costumSensorCount += 1;
}

void sensorWidget::info_button_pushed()
{
    //emit sendMsg(QString("usv1"),QString("qformat"));
}

void sensorWidget::subscribeSensor(int BoatID, int BoardID, int PinID, char unit)
{

}
