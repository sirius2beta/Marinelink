#include "boatsetting.h"
#include "ui_boatsetting.h"
#include "settings_provider.h"
#include <QComboBox>
#include <QMessageBox>

#define SENSOR 0x50

BoatSetting::BoatSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoatSetting),
    initialized(false)
{
    ui->setupUi(this);
    ui->BoatTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->BoatTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->BoatlineEdit, &QLineEdit::editingFinished, this, &BoatSetting::onBoatNameChange);
    connect(ui->BoatTableView, &QTableView::doubleClicked, this, &BoatSetting::onBoatDoubleClicked);
    connect(ui->PIPlineEdit, &QLineEdit::editingFinished, this, &BoatSetting::onChangeIP);
    connect(ui->SIPlineEdit, &QLineEdit::editingFinished, this, &BoatSetting::onChangeIP);

    settings = new QSettings("Ezosirius", "GPlayer_v1", this);

    ui->BoatTableView->verticalHeader()->setDefaultSectionSize(10);
    ui->DeviceTableView->verticalHeader()->setDefaultSectionSize(10);
    //setup boat tableview
    boatItemModel = new QStandardItemModel();
    QStringList label = {"name", "Primary", "Secondary"};
    boatItemModel->setHorizontalHeaderLabels(label);
    ui->BoatTableView->setModel(boatItemModel);
    //ui->BoatTableView->setColumnWidth(0,100);
    ui->BoatTableView->setColumnWidth(1,100);
    ui->BoatTableView->setColumnWidth(2,100);
    ui->BoatTableView->verticalHeader()->setVisible(false);


    deviceItemModel = new QStandardItemModel();
    QStringList label2 = {"ID", QStringLiteral("USB裝置"), QStringLiteral("腳位"),QStringLiteral("類別"),QStringLiteral("狀態")};
    deviceItemModel->setHorizontalHeaderLabels(label2);
    ui->DeviceTableView->setModel(deviceItemModel);
    ui->DeviceTableView->setColumnWidth(0,30);
    ui->DeviceTableView->setColumnWidth(1,100);
    ui->DeviceTableView->setColumnWidth(2,100);
    ui->DeviceTableView->setColumnWidth(3,80);
    ui->DeviceTableView->verticalHeader()->setVisible(false);


    peripheralItemModel = new QStandardItemModel();
    /*
    QStringList label3 = {"ID", QStringLiteral("USB裝置"), QStringLiteral("腳位"),QStringLiteral("類別"),QStringLiteral("狀態")};
    peripheralItemModel->setHorizontalHeaderLabels(label3);
    ui->PeriTableView->setModel(peripheralItemModel);
    ui->PeriTableView->setColumnWidth(0,50);
    ui->PeriTableView->setColumnWidth(1,100);
    ui->PeriTableView->setColumnWidth(2,100);
    ui->PeriTableView->setColumnWidth(3,50);
    ui->PeriTableView->setStyleSheet("QTableView::indicator:checked { background-color: #55ee55;} QTableView::indicator:unchecked { background-color: #ee5555;}");
    ui->PeriTableView->verticalHeader()->setVisible(false);
    */

    QStringList label3 = {"ID", QStringLiteral("USB裝置"), QStringLiteral("類別"),QStringLiteral("狀態")};
    peripheralItemModel->setHorizontalHeaderLabels(label3);
    ui->PeriTableView->setModel(peripheralItemModel);
    ui->PeriTableView->setColumnWidth(0,50);
    ui->PeriTableView->setColumnWidth(2,100);
    ui->PeriTableView->setColumnWidth(3,50);
    ui->PeriTableView->setStyleSheet("QTableView::item{border:0;}"
                                     "QTableView::indicator:checked { background-color: #55ee55;}"
                                     "QTableView::indicator:unchecked { background-color: #ee5555;}");

    connect(ui->addBoatPushButton, &QPushButton::clicked, this, &BoatSetting::onAddBoat);
    connect(ui->deleteBoatPushButton, &QPushButton::clicked, this, &BoatSetting::onDeleteBoat);
    connect(ui->BoatcomboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this, &BoatSetting::onBoatSelected);
    connect(ui->addDeviceButton, &QPushButton::clicked, this, &BoatSetting::onAddDeviceButtonClicked);

}

void BoatSetting::initSettings(Boats* BoatList)
{
    boatList = BoatList;
    settings->beginGroup(QString("%1").arg(config));
    int size = settings->beginReadArray("boat");

    if(size!=0){
        for(int i = 0; i < size; i++){            
            settings->setArrayIndex(i);
            QString boatname = settings->value("name").toString();
            int ID = settings->value("ID").toInt();
            QString boatPIP = settings->value("/PIP").toString();
            QString boatSIP = settings->value("/SIP").toString();
            appendBoat(boatname, ID, boatPIP, boatSIP);
            emit AddBoat(boatname, ID, boatPIP, boatSIP);
            emit vAddBoat(boatname);

            //initialize board
            int size = settings->beginReadArray(QString("board"));
            qDebug()<<"Boatsetting: init boat: "<<boatname<<"board num: "<<size;

            if(size!=0){
                for(int i = 0; i < size; i++){

                    settings->setArrayIndex(i);

                    int ID = settings->value("ID").toInt();
                    QString boardName = settings->value("boardName").toString();
                    QString boardType = settings->value("boardType").toString();
                    if(boardType == QString("FTDI")){
                        boardType = QString("RS485_hub");
                    }else if(boardType == QString("Silicon")){
                        boardType = QString("Gyro");
                    }
                    addBoard(ID, boardName, boardType, false);
                    Peripheral p;
                    p.ID = ID;
                    p.boardName = boardName;
                    p.boardType = boardType;
                    currentBoat()->peripherals.append(p);
                    qDebug()<<"----------------- "<<i<<","<<ID<<","<<boardName;
                }
            }

            //initialized = true;
            //emit sendMsg(ui->BoatcomboBox->currentText(), char(SENSOR), QString("d").toLocal8Bit());
            settings->endArray();
        }
    }else{
        ui->infoBox->setVisible(false);
    }
    settings->endArray();
    settings->endGroup();
    initialized = true;
}

void BoatSetting::addBoard(int ID, QString boardName, QString boardType, bool connected)
{
    int row = peripheralItemModel->rowCount();
    QStandardItem* item1 = new QStandardItem(QString::number(ID));
    item1->setEditable(false);
    item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QStandardItem* item2 = new QStandardItem(boardName);
    QStandardItem* item3 = new QStandardItem(boardType);
    item3->setEditable(false);
    QStandardItem* item4 = new QStandardItem(true);
    item4->setEditable(false);
    item4->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    item4->setCheckable(false);
    if(connected){
        item4->setCheckState(Qt::Checked);
    }else{
        item4->setCheckState(Qt::Unchecked);
    }

    peripheralItemModel->setItem(row,0,item1);
    peripheralItemModel->setItem(row,1,item2);
    peripheralItemModel->setItem(row,2,item3);
    peripheralItemModel->setItem(row,3,item4);
    ui->PeriTableView->setRowHeight(row,5);

}

Boat* BoatSetting::currentBoat(){
    return boatList->getBoatbyIndex(ui->BoatcomboBox->currentIndex());
}

 void BoatSetting::upDateDeviceTableView()
 {
     for(int i = 0; i < currentBoat()->devices.size(); i++){
         QStandardItem* item1 = new QStandardItem(QString::number(currentBoat()->devices[i].PeriID));
         QStandardItem* item2 = new QStandardItem(currentBoat()->devices[i].deviceName);
         QString line;

         for(int j = 0; j< currentBoat()->devices[i].pinIDList.size(); j++){
             line+=QString::number(currentBoat()->devices[i].PeriID);
             line+=":";
             line+=QString::number(currentBoat()->devices[i].pinIDList[j]);
             line+=", ";
         }
         QStandardItem* item3 = new QStandardItem(line);

         QStandardItem* item4 = new QStandardItem(currentBoat()->devices[i].typeName());
         deviceItemModel->setItem(i,0, item1);
         deviceItemModel->setItem(i,1, item2);
         deviceItemModel->setItem(i,2, item3);
         deviceItemModel->setItem(i,3, item4);
     }
 }

void BoatSetting::appendBoat(QString boatname, int ID, QString PIP, QString SIP)
{

    int current = boatItemModel->rowCount();
    QStandardItem* item1 = new QStandardItem(boatname);
    QStandardItem* item2 = new QStandardItem(QString("SB"));
    item2->setData(PIP);
    item2->setBackground(QBrush(QColor(120,0,0)));
    QStandardItem* item3 = new QStandardItem(QString("SB"));
    item3->setData(SIP);
    item3->setBackground(QBrush(QColor(120,0,0)));
    boatItemModel->setItem(current,0,item1);
    boatItemModel->setItem(current,1,item2);
    boatItemModel->setItem(current,2,item3);
    ui->BoatTableView->setRowHeight(current,30);
    ui->BoatcomboBox->addItem(boatname,boatname);
    ui->BoatcomboBox->setCurrentIndex(ui->BoatcomboBox->count()-1);
    ui->BoatlineEdit->setText(boatname);
    ui->PIPlineEdit->setText(PIP);
    ui->SIPlineEdit->setText(SIP);

    Boat* boat = boatList->addBoat(ID);
    boat->name = boatname;
    boat->PIP = PIP;
    boat->SIP = SIP;
    //boat->ID = ID;

    boat->CurrentIP = QString();



}

void BoatSetting::setconfig(QString conf)
{
    config = conf;
}

BoatSetting::~BoatSetting()
{
    delete ui;
}



void BoatSetting::deleteDevice(int ID)
{

}

void BoatSetting::onConnected(int ID, bool isprimary)
{
    for(int i = 0; i < boatItemModel->rowCount();i++){
        qDebug()<<"boatItem: "<<boatItemModel->item(i,0)->text()<<"boatName: "<<boatList->getBoatbyID(ID)->name;
        if(boatItemModel->item(i,0)->text() == boatList->getBoatbyID(ID)->name){

            if(isprimary){
                boatItemModel->item(i,1)->setText("Active");
                boatItemModel->item(i,1)->setBackground(QBrush(QColor(0,120,0)));
            }else{
                boatItemModel->item(i,2)->setText("Active");
                boatItemModel->item(i,2)->setBackground(QBrush(QColor(0,120,0)));
            }
        }
    }

}

void BoatSetting::onDisonnected(int ID, bool isprimary)
{
    for(int i = 0; i < boatItemModel->rowCount();i++){
        if(boatItemModel->item(i,0)->text() == boatList->getBoatbyID(ID)->name){
            if(isprimary){
                boatItemModel->item(i,1)->setText("SB");
                boatItemModel->item(i,1)->setBackground(QBrush(QColor(120,0,0)));
            }else{
                boatItemModel->item(i,2)->setText("SB");
                boatItemModel->item(i,2)->setBackground(QBrush(QColor(120,0,0)));
            }
        }
    }
}

void BoatSetting::onMsg(QByteArray data)
{
    int ID = int(data[0]);
    char mode = char(data[1]);
    data.remove(0,1);
    QStringList devlistline = QString(data).split('\n');

    if(mode == 'r'){
        if(ID != boatList->getBoatbyIndex(ui->BoatcomboBox->currentIndex())->id){
            return;
        }
        if(!initialized){
            return;
        }
        qDebug()<<devlistline.size();
        for(int i = 0; i < devlistline.size(); i++){
            if(devlistline[i].size() == 0){
                break;
            }
            qDebug()<<devlistline[i];

            QStringList devinfo = devlistline[i].split(',');
            int devID = devinfo[4].toInt();
            bool cont = false;
            for(int i = 0; i<peripheralItemModel->rowCount(); i++){
                if(devID ==peripheralItemModel->item(i,0)->text().toInt()){
                    cont = true;
                    peripheralItemModel->item(i,3)->setCheckState(Qt::Checked);
                }else{

                }
                qDebug()<<"---------"<<peripheralItemModel->item(i,0)->text().toInt()<<", "<<devID;
            }

            if(!cont){
                Peripheral newperiperal;
                Boat* boat = boatList->getBoatbyIndex(ui->BoatcomboBox->currentIndex());
                newperiperal.ID = devinfo[4].toInt();

                newperiperal.boardName = devinfo[2];

                QString dev_type = (newperiperal.boardName).split(" ")[0];
                newperiperal.boardType = dev_type;
                if(dev_type == QString("FTDI")){
                    dev_type = QString("RS485_hub");
                }else if(dev_type == QString("Silicon")){
                    dev_type = QString("Gyro");
                }
                boat->peripherals.append(newperiperal);
                qDebug()<<"--------Add periperal-----------";
                qDebug()<<"boat:"<<boat->name;
                qDebug()<<"DevID"<<newperiperal.ID;
                qDebug()<<"DevName"<<newperiperal.boardName;
                qDebug()<<"DevType"<<newperiperal.boardType;


                addBoard(newperiperal.ID, newperiperal.boardName, dev_type, true);
                currentBoat()->peripherals.append(newperiperal);

                settings->beginGroup(QString("%1").arg(config));
                int size = settings->beginReadArray(QString("boat/%1/board").arg(ui->BoatcomboBox->currentIndex()+1));

                settings->endArray();
                settings->beginWriteArray(QString("boat/%1/board").arg(ui->BoatcomboBox->currentIndex()+1));
                settings->setArrayIndex(size);
                settings->setValue(QString("ID"), newperiperal.ID);
                settings->setValue(QString("boardName"), newperiperal.boardName);
                settings->setValue(QString("boardType"), dev_type);
                settings->endArray();
                settings->endGroup();
            }

        }
        // send mapping
        for(int i = 0; i< boatList->getBoatbyID(ID)->devices.size(); i++){
            Device dev = boatList->getBoatbyID(ID)->devices[i];
            QString cmd = "ma";
            cmd += QString::number(dev.ID);
            cmd+= ",";
            cmd += QString::number(dev.PeriID);
            cmd+= ",";
            cmd += QString::number(dev.pinIDList[0]);
            if(dev.type == 0){
                cmd+= ",";
                cmd += QString::number(0);
                cmd+= ",";
                cmd += dev.settings.join(" ");
                qDebug()<<cmd;
                emit sendMsg(currentBoat()->name, char(SENSOR), cmd.toLocal8Bit());
            }else if(dev.type == Device::SERVO){

            }else if(dev.type == Device::STEPPER){

            }
        }

    }
    /*
    int sensor_num = data.mid(1,1).toHex().toInt(&ok);
    int start = 1;
    for(int i = 0; i<sensor_num; i++){
        QString unit = data.mid(start,1);
        int BoatID = data.mid(start+1,1).toHex().toInt(&ok);
        int DeviceID = data.mid(start+2,1).toHex().toInt(&ok);
        int uid = 255*BoatID + DeviceID;
        qDebug()<<"Sensor uid:"<<uid;
        SensorLabel* label = labelMap[uid];
        if(label == 0){
            return;
        }
        if(label->unit == 'i'){
            bool ok;
            int value = data.mid(start+3,4).toHex().toInt(&ok, 16);
            label->setValue(QString::number(value));
            start+=7;
        }
    }*/


}

QString BoatSetting::getIP(QString boatname){
    for(int i = 0; i < boatItemModel->rowCount();i++){
        if(boatItemModel->item(i,0)->text() == boatname){
            if(boatItemModel->item(i,1)->text() == QString("Active")){
                return boatItemModel->item(i,1)->data().toString();
                qDebug()<<"Using Primary IP";
            }else{
                return boatItemModel->item(i,2)->data().toString();
                qDebug()<<"Using Secondary IP";
            }

        }
    }
    return "0.0.0.0";

}

bool BoatSetting::isPrimary(QString boatname){
    for(int i = 0; i < boatItemModel->rowCount();i++){
        if(boatItemModel->item(i,0)->text() == boatname){
            if(boatItemModel->item(i,1)->text() == QString("Active")){
                return true;
            }else{
                return false;
            }

        }
    }
    return "192.168.0.1";

}

void BoatSetting::onBoatNameChange()
{
    QString oldname = ui->BoatcomboBox->currentText();
    QString newname = ui->BoatlineEdit->text();
    for(int i = 0; i < boatItemModel->rowCount();i++){
        if(boatItemModel->item(i,0)->text() == oldname){
            boatItemModel->item(i,0)->setText(newname);
        }
    }
    emit changeBoatName(oldname, newname);
    ui->BoatcomboBox->setItemText(ui->BoatcomboBox->currentIndex(),ui->BoatlineEdit->text());
    settings->beginGroup(QString("%1").arg(config));
    int size = settings->beginReadArray("boat");
    int index = 0;
    for(int i = 0; i<size; i++){

        settings->setArrayIndex(i);
        ;
        if(settings->value("name").toString() == oldname){
            settings->setValue("boatname",newname);
            index = i;
            boatList->getBoatbyIndex(i)->name = newname;
            break;
        }
    }

    settings->endArray();
    settings->endGroup();

}

void BoatSetting::onAddBoat()
{
    int count = 0;
    QVector<bool> indexfree(256, true);
    int index = 0;
    for(int i = 0; i < boatItemModel->rowCount();i++){
        if(boatItemModel->item(i,0)->text().contains("unknown")){
            count++;
            qDebug()<<boatItemModel->item(i,0)->text();
            qDebug()<<boatList->getBoatbyName(boatItemModel->item(i,0)->text())->id;
            indexfree[boatList->getBoatbyName(boatItemModel->item(i,0)->text())->id] = false;
            qDebug()<<"done";
        }
    }

    for(int i =0; i<256; i++){
        if(indexfree[i] == true){
            index = i;
            break;
        }
    }
    QString newboatname = "unknown"+QString::number(index);

    appendBoat(newboatname, index,"", "");

    settings->beginGroup(QString("%1").arg(config));
    int size = settings->beginReadArray("boat");

    settings->endArray();
    settings->beginWriteArray("boat");
    settings->setArrayIndex(size);
    settings->setValue(QString("boatname"), newboatname);
    settings->setValue(QString("ID"), index);
    settings->setValue(QString("PIP"), "192.168.0.1");
    settings->setValue(QString("SIP"), "100.100.100.100");
    settings->endArray();
    settings->endGroup();

    ui->BoatcomboBox->setCurrentIndex(ui->BoatcomboBox->count()-1);
    emit AddBoat(newboatname, index, "192.168.0.1", "100.100.100.100");
    emit vAddBoat(newboatname);
}

void BoatSetting::onDeleteBoat()
{
    int index = ui->BoatcomboBox->currentIndex();
    int deletedID = boatList->getBoatbyIndex(index)->id;
    qDebug()<<"BoatSetting delete: " <<index;
    QMessageBox msgBox;
    msgBox.setText(QString("Are you sure you want to delete ")+ui->BoatcomboBox->currentText());
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret != QMessageBox::Ok){
        return;
    }
    settings->beginGroup(QString("%1").arg(config));
    settings->beginWriteArray("boat");
    QString lastname;
    QString lastPIP;
    QString lastSIP;
    int lastID;
    int size = boatList->size();

    for(int i = index; i<size-1; i++){
        Boat* boat = boatList->getBoatbyIndex(i+1);
        lastname = boat->name;
        lastPIP = boat->PIP;
        lastSIP = boat->SIP;
        lastID = boat->id;



        settings->setArrayIndex(i);
        settings->setValue("boatname",lastname);
        settings->setValue("PIP", lastPIP);
        settings->setValue("SIP", lastSIP);
        settings->setValue("ID", lastID);
        settings->remove(QString("boat/%1/board").arg(size));
        settings->beginWriteArray("board");
        for(int j = 0; j<boat->peripherals.size(); j++){
            settings->setArrayIndex(j);
            settings->setValue("boardName", boat->peripherals[j].boardName);
            settings->setValue("boardType", boat->peripherals[j].boardType);
            settings->setValue("ID", boat->peripherals[j].ID);
        }
        settings->endArray();

    }
    settings->endArray();
    settings->setValue("boat/size", size-1);
    settings->remove(QString("boat/%1").arg(size));
    settings->endGroup();
    boatList->deleteBoat(deletedID);
    emit deleteBoat(ui->BoatcomboBox->currentText());
    ui->BoatcomboBox->removeItem(index);
    boatItemModel->removeRows(index,1);
    if(ui->BoatcomboBox->count() == 0){
        ui->infoBox->setVisible(false);
    }
}

void BoatSetting::onBoatSelected(int index)
{
    if(index>-1){
        ui->BoatlineEdit->setText(ui->BoatcomboBox->currentText());
        ui->PIPlineEdit->setText(boatItemModel->item(index,1)->data().toString());
        ui->SIPlineEdit->setText(boatItemModel->item(index,2)->data().toString());
    }
    ui->infoBox->setVisible(true);
    peripheralItemModel->clear();
    peripheralItemModel = new QStandardItemModel();
    QStringList label3 = {"ID", QStringLiteral("USB裝置"), QStringLiteral("類別"),QStringLiteral("狀態")};
    peripheralItemModel->setHorizontalHeaderLabels(label3);
    ui->PeriTableView->setModel(peripheralItemModel);
    ui->PeriTableView->setColumnWidth(0,50);
    ui->PeriTableView->setColumnWidth(2,100);
    ui->PeriTableView->setColumnWidth(3,50);
    ui->PeriTableView->setStyleSheet("QTableView::item{border:0;}"
                                    "QTableView::indicator:checked { background-color: #55ee55;}"
                                    "QTableView::indicator:unchecked { background-color: #ee5555;}");
    ui->PeriTableView->verticalHeader()->setVisible(false);



    if(initialized){
        settings->beginGroup(QString("%1").arg(config));
        int size = settings->beginReadArray(QString("boat/%1/board").arg(index+1));

        if(size!=0){
            for(int i = 0; i < size; i++){

                settings->setArrayIndex(i);

                int ID = settings->value("ID").toInt();
                QString boardName = settings->value("boardName").toString();
                QString boardType = settings->value("boardType").toString();
                if(boardType == QString("FTDI")){
                    boardType = QString("RS485_hub");
                }else if(boardType == QString("Silicon")){
                    boardType = QString("Gyro");
                }
                addBoard(ID, boardName, boardType, false);
            }
        }


        emit sendMsg(ui->BoatcomboBox->currentText(), char(SENSOR), QString("d").toLocal8Bit());
        settings->endArray();
        settings->endGroup();
    }


}

void BoatSetting::onBoatDoubleClicked(QModelIndex index)
{
    ui->BoatlineEdit->setText(boatItemModel->item(index.row(),0)->text());
    ui->PIPlineEdit->setText(boatItemModel->item(index.row(),1)->data().toString());
    ui->SIPlineEdit->setText(boatItemModel->item(index.row(),2)->data().toString());
    ui->BoatcomboBox->setCurrentIndex(index.row());
}

void BoatSetting::onChangeIP()
{
    bool breakout = false;
    QString repeatedBoat;

    for(int i = 0; i< boatItemModel->rowCount(); i++){
        if(ui->BoatcomboBox->currentIndex() == i){
            continue;
        }
        if((boatItemModel->item(i,1)->data() == ui->PIPlineEdit->text()) && (ui->PIPlineEdit->text() != "")){
            ui->PIPlineEdit->setText("");
            repeatedBoat = boatItemModel->item(i,0)->text();
            breakout = true;
        }else if((boatItemModel->item(i,2)->data() == ui->SIPlineEdit->text()) && ui->SIPlineEdit->text() != ""){
            ui->SIPlineEdit->setText("");
            repeatedBoat = boatItemModel->item(i,0)->text();
            breakout = true;
        }

    }
    if(breakout){
        QMessageBox msgBox;
        msgBox.setText(QStringLiteral("IP與其他船衝突: ")+repeatedBoat);
        msgBox.exec();
    }
    emit ChangeIP(ui->BoatcomboBox->currentText(), ui->PIPlineEdit->text(), ui->SIPlineEdit->text());
    settings->beginGroup(QString("%1").arg(config));
    int size = settings->beginReadArray("boat");
    int index = 0;
    for(int i = 0; i<size; i++){
        settings->setArrayIndex(i);
        if(settings->value("name").toString() == ui->BoatcomboBox->currentText()){
            settings->setValue("PIP",ui->PIPlineEdit->text());
            settings->setValue("SIP",ui->SIPlineEdit->text());
        }
    }
    boatItemModel->item(ui->BoatcomboBox->currentIndex(),1)->setData(ui->PIPlineEdit->text());
    boatItemModel->item(ui->BoatcomboBox->currentIndex(),2)->setData(ui->SIPlineEdit->text());
    settings->endArray();
    settings->endGroup();
}

void BoatSetting::onAddDeviceButtonClicked()
{
    AddDeviceDialog* dialog = new AddDeviceDialog(this);
    dialog->setPeripheralModel(peripheralItemModel, boatList->getBoatbyIndex(ui->BoatcomboBox->currentIndex()));
    int ret = dialog->exec();
    if(ret==QDialog::Accepted){
        Device newdev = dialog->getDevice();

        if(newdev.pinIDList.size() != 0){
            addDevice(newdev);
        }


    }
}

void BoatSetting::addDevice(const Device& d)
{
    Device newDev = d;
    currentBoat()->addDevice(newDev);

    qDebug()<<"Add device:";
    qDebug()<<"  - ID"<<newDev.ID;
    qDebug()<<"  - Type:"<<newDev.type;
    qDebug()<<"  - PPID:"<<newDev.PeriID<<":"<<newDev.pinIDList[0];
    qDebug()<<"  - Settings"<<newDev.settings.join(" ");

    for(int j = 0; j < currentBoat()->peripherals.size(); j++){
        if(currentBoat()->peripherals[j].ID == newDev.PeriID){
            for(int i = 0; i < d.pinIDList.size(); i++){
                currentBoat()->peripherals[j].pinSelected.append(d.pinIDList[i]);
            }
            break;
        }
    }


    QString cmd = "ma";
    cmd += QString::number(newDev.ID);
    cmd+= ",";
    cmd += QString::number(newDev.PeriID);
    cmd+= ",";
    cmd += QString::number(newDev.pinIDList[0]);
    if(newDev.type == 0){
        cmd+= ",";
        cmd += QString::number(0);
        cmd+= ",";
        cmd += newDev.settings.join(" ");
        qDebug()<<cmd;
        emit sendMsg(currentBoat()->name, char(SENSOR), cmd.toLocal8Bit());
    }else if(newDev.type == Device::SERVO){

    }else if(newDev.type == Device::STEPPER){

    }
    upDateDeviceTableView();
    emit updateDevice();
}
