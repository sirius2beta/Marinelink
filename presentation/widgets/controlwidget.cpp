#include "controlwidget.h"
#include "ui_controlwidget.h"
#define SENSOR 0x50

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);
    connect(ui->start_pushButton, &QPushButton::clicked, this, &ControlWidget::onStart);
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::setBoatList(Boats* bs)
{
    boatList = bs;

    QStringList label3 = {"devID", "boat", QStringLiteral("board"), QStringLiteral("ID")};
    model->setHorizontalHeaderLabels(label3);

    int row = 0;
    for(int i = 0; i<boatList->size(); i++){
        for(int j = 0; j< boatList->getBoatbyIndex(i)->devices.size(); j++){
            if(boatList->getBoatbyIndex(i)->devices[j].type == 2){
                QStandardItem* item1 = new QStandardItem(QString::number(boatList->getBoatbyIndex(i)->devices[j].ID));

                QStandardItem* item2 = new QStandardItem(QString::number(boatList->getBoatbyIndex(i)->id));
                QStandardItem* item3 = new QStandardItem(QString::number(boatList->getBoatbyIndex(i)->devices[j].PeriID));
                QStandardItem* item4 = new QStandardItem(QString::number(boatList->getBoatbyIndex(i)->devices[j].pinIDList[0]));
                model->setItem(row, 0, item1);
                model->setItem(row, 1, item2);
                model->setItem(row, 2, item3);
                model->setItem(row, 3, item4);
            }
        }
    }

    ui->stepper_tableView->setModel(model);
    ui->stepper_tableView->setColumnWidth(0,50);
    ui->stepper_tableView->setColumnWidth(1,50);
    ui->stepper_tableView->setColumnWidth(2,50);
    ui->stepper_tableView->setColumnWidth(3,50);
}

void ControlWidget::update()
{
    model->clear();
    QStringList label3 = {"devID", "boat", QStringLiteral("board"), QStringLiteral("ID")};
    model->setHorizontalHeaderLabels(label3);

    int row = 0;
    for(int i = 0; i<boatList->size(); i++){
        for(int j = 0; j< boatList->getBoatbyIndex(i)->devices.size(); j++){
            if(boatList->getBoatbyIndex(i)->devices[j].type == 2){
                QStandardItem* item1 = new QStandardItem(QString::number(boatList->getBoatbyIndex(i)->devices[j].ID));
                QStandardItem* item2 = new QStandardItem(QString::number(boatList->getBoatbyIndex(i)->id));
                QStandardItem* item3 = new QStandardItem(QString::number(boatList->getBoatbyIndex(i)->devices[j].PeriID));
                QStandardItem* item4 = new QStandardItem(QString::number(boatList->getBoatbyIndex(i)->devices[j].pinIDList[0]));
                model->setItem(row, 0, item1);
                model->setItem(row, 1, item2);
                model->setItem(row, 2, item3);
                model->setItem(row, 3, item4);
            }
        }
    }

    ui->stepper_tableView->setModel(model);
    ui->stepper_tableView->setColumnWidth(0,50);
    ui->stepper_tableView->setColumnWidth(1,50);
    ui->stepper_tableView->setColumnWidth(2,50);
    ui->stepper_tableView->setColumnWidth(3,50);
}

void ControlWidget::onStart()
{
    if(ui->step_lineEdit->text() != QString()){

        int row = ui->stepper_tableView->currentIndex().row();
        if(row == -1){
            return;
        }
        int devID = model->item(row,0)->text().toInt();
        int boatID = model->item(row,1)->text().toInt();

        Device dev;
        for(int i = 0; i<boatList->getBoatbyID(boatID)->devices.size(); i++){
            if(boatList->getBoatbyID(boatID)->devices[i].ID == devID){
                dev = boatList->getBoatbyID(boatID)->devices[i];
            }
        }

        QString step = ui->step_lineEdit->text();
        QString cmd = "cd";
        cmd += QString::number(dev.ID);
        cmd+= ",";
        cmd += QString::number(dev.PeriID);
        cmd+= ",";
        cmd += QString::number(dev.pinIDList[0]);
        cmd+= " ";
        cmd += QString::number(dev.pinIDList[1]);
        cmd+= ",";
        cmd += QString::number(dev.type);
        cmd+= ",";
        cmd += dev.settings.join(" ");
        cmd+= " ";
        cmd+= step;
        qDebug()<<cmd;
        emit sendMsg(boatList->getBoatbyID(boatID)->name, char(SENSOR), cmd.toLocal8Bit());

    }
}
