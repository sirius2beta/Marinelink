#include "setpindialog.h"
#include "qmessagebox.h"
#include "ui_setpindialog.h"
#include <QDebug>

SetPinDialog::SetPinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetPinDialog),
    itemReady(false),
    devType(0),
    devID(0),
    maxNum(0)
{
    ui->setupUi(this);

    pinItemModel = new QStandardItemModel(this);



    ArduinoPinMap = QList<QString> {"0", "0", "03", "03", "", "", "0", "02", "01", "02",
                                    "012", "0", "01", "012", "012", "02", "01", "02", "", "",
                                    "01", "01", "01", "01", "01", "01", "", "", "", "",
                                    "", "", "0", "0"};
    ArduinoPinInfo = QList<QString> {"   D16/COPI", "   D17", "   D1/TX", "   D0/RX", "RESET", "GND", "   D2", "~D3", "   D4/A6", "~D5",
                                    "~D6/A7", "   D7", "   D8/A8", "~D9/A9", "~D10/A10", "~D11", "   D12/A11", "~D13", "+3V", "AREF",
                                    "   D18/A0", "   D19/A1", "   D20/A2", "   D21/A3", "   D22/A4", "   D23/A5", "NC", "NC", "+5V", "RESET",
                                    "GND", "VIN", "D14", "D15"};
    IOMap = QList<QString> {"01", "1", "0", "01"};


    ui->pinTableView->verticalHeader()->setDefaultSectionSize(10);
    connect(ui->boardTableView,&QTableView::clicked, this, &SetPinDialog::onBoardSelected);
    connect(pinItemModel, &QStandardItemModel::itemChanged, this, &SetPinDialog::onDeviceItemChanged);
}

SetPinDialog::~SetPinDialog()
{
    delete ui;
}

void SetPinDialog::setPeripheralModel(QStandardItemModel* pm, Boat* boat){
    thisBoat = boat;
    this->peripheralItemModel = pm;
    QStringList label = {"ID", QStringLiteral("裝置名稱"), QStringLiteral("USB裝置"), QStringLiteral("狀態")};
    //peripheralItemModel->setHorizontalHeaderLabels(label);
    ui->boardTableView->setModel(peripheralItemModel);
    ui->boardTableView->setColumnWidth(0,50);

    ui->boardTableView->setColumnWidth(2,100);
    ui->boardTableView->setColumnWidth(3,50);
    ui->boardTableView->verticalHeader()->setVisible(false);
    PeripheralCache = boat->peripherals;

}

void SetPinDialog::setType(int type)
{
    devType = type;
}

void SetPinDialog::setDev(const Device& Dev)
{
    newDev = Dev;
}

void SetPinDialog::setPeripherals(const QList<Peripheral>& peripherals)
{

    PeripheralCache = peripherals;
}

void SetPinDialog::setMaxNum(int num)
{
    maxNum = num;
}

void SetPinDialog::setIndex(int i)
{
    index = i;
}

int SetPinDialog::getIndex()
{
    return index;
}

QList<int> SetPinDialog::getpinCurrentSelected()
{
    return pinCurrentSelected;
}

QList<Peripheral> SetPinDialog::getPeripherals()
{
    for(int i = 0; i <pinCurrentSelected.size(); i++){
        PeripheralCache[currentRow].pinSelected.append(pinCurrentSelected[i]);
    }
    return PeripheralCache;
}

void SetPinDialog::onBoardSelected(const QModelIndex &index){
    itemReady = false;
    currentRow = index.row();
    pinCurrentSelected.clear();


    if(peripheralItemModel->item(index.row(),1)->text() == "Arduino LLC"){
        pinItemModel->clear();
        QStringList label = {QStringLiteral("啟用"), "ID", "Pin",QStringLiteral("裝置名稱")};
        pinItemModel->setHorizontalHeaderLabels(label);
        ui->pinTableView->setModel(pinItemModel);
        ui->pinTableView->setColumnWidth(0,40);
        ui->pinTableView->setColumnWidth(1,40);
        ui->pinTableView->setColumnWidth(2,120);

        ui->pinTableView->verticalHeader()->setVisible(false);
        ui->pinTableView->setStyleSheet("QTableView::item{border:0;}"
                                        "QTableView::indicator:checked { background-color: #55ee55;}"
                                        "QTableView::indicator:unchecked { background-color: #ee5555;}");

        for(int i = 0; i<34; i++){
            QStandardItem* item1 = new QStandardItem();
            item1->setCheckable(true);
            item1->setCheckState(Qt::Unchecked);
            QStandardItem* item2 = new QStandardItem(QString::number(i+1));
            QStandardItem* item3 = new QStandardItem("unknown");
            QStandardItem* item4 = new QStandardItem(ArduinoPinInfo[i]);

            pinItemModel->setItem(i,0,item1);
            pinItemModel->setItem(i,1,item2);
            pinItemModel->setItem(i,2,item4);
            pinItemModel->setItem(i,3,item3);
            ui->pinTableView->setRowHeight(i,1);

        }

        for(int i = 0; i<PeripheralCache[index.row()].pinSelected.size(); i++){
                bool cont = false;
                for(int j = 0; j < pinItemModel->rowCount(); j++){
                    for(int i = 0; i < PeripheralCache[currentRow].pinSelected.size(); i++){
                        if(PeripheralCache[currentRow].pinSelected[i] == pinItemModel->item(j, 1)->text().toInt()){
                            pinItemModel->setItem(j,0,0);
                            break;
                        }
                    }
                }
        }
    }else{
        pinItemModel->clear();
    }
    itemReady = true;
}

void SetPinDialog::onDeviceItemChanged(QStandardItem *item)
{
    if(itemReady){
        if(item->column() == 0){ // Activate欄，切換開啟關閉
           if(item->checkState()){ // 開啟時
               if(maxNum<=pinCurrentSelected.size()){ //超過最多能選擇個數
                   QMessageBox* msgbox = new QMessageBox(this);
                   msgbox->setText("exceed maximum selected pin number!");
                   msgbox->exec();
                   item->setCheckState(Qt::Unchecked);
                   return;
               }
               int pin = pinItemModel->item(item->row(),1)->text().toInt();
               pinCurrentSelected.append(pin);


               /*
               qDebug()<<"ID:"<<newDev.ID
               qDebug()<<"uid:"<<uid;
               qDebug()<<"name:"<<newDev.deviceName;
               qDebug()<<"type:"<<newDev.type;
               */
               //新增至Peripheral pinselected中
           }else{
               int pinID = pinItemModel->item(item->row(),1)->text().toInt();

               for(int i = 0; i<pinCurrentSelected.size(); i++){
                   if(pinCurrentSelected[i] == pinID){
                       pinCurrentSelected.removeAt(i);
                   }
               }

           }

        }

    }

}


int SetPinDialog::getPeriID()
{
    return PeripheralCache[currentRow].ID;
}

QList<int> SetPinDialog::getPinID()
{
    return pinCurrentSelected;
}
