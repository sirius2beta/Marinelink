#include "adddevicedialog.h"
#include "setpindialog.h"
#include "ui_adddevicedialog.h"

AddDeviceDialog::AddDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDeviceDialog),
    backgrounWidget(0),
    currentMode(-1),
    devName(QString("unknown"))
{
    ui->setupUi(this);

    connect(ui->SensorButton, &QPushButton::clicked, this, &AddDeviceDialog::onAddSensor);
    connect(ui->ServoButton, &QPushButton::clicked, this, &AddDeviceDialog::onAddServo);
    connect(ui->StepperButton, &QPushButton::clicked, this, &AddDeviceDialog::onAddStepper);


}

AddDeviceDialog::~AddDeviceDialog()
{
    delete ui;
}

void AddDeviceDialog::setPeripheralModel(QStandardItemModel* pm, Boat* boat){
    thisBoat = boat;
    this->peripheralItemModel = pm;
    PeripheralCache = boat->peripherals;

}

Device AddDeviceDialog::getDevice()
{
    newDev.deviceName = devName;

    for(int i = 0; i < pinCurrentSelected.size(); i++){
        if(pinCurrentSelected[i].size() == 0){
            return Device();
        }
        newDev.pinIDList.append(pinCurrentSelected[i][0]);
    }


    if(newDev.type == Device::SENSOR){
        newDev.settings.append(opt1_lineEdit->text());
    }else if(newDev.type == Device::SERVO){

    }else if(newDev.type == Device::STEPPER){
        if(newDev.pinIDList.size() != 2){
            return Device();
        }
        newDev.settings.append(opt1_lineEdit->text());
        newDev.settings.append(opt1_lineEdit->text());
    }
    return newDev;
}

QString AddDeviceDialog::getDevName(){
    return devName;
}

QWidget* AddDeviceDialog::addDevNameField()
{
    QWidget* holderwidget = new QWidget(backgrounWidget);
    QHBoxLayout* layout = new QHBoxLayout(holderwidget);
    layout->setContentsMargins(11,2,11,2);
    QLineEdit* lineEdit = new QLineEdit(holderwidget);
    lineEdit->setText(devName);
    QLabel* label = new QLabel(holderwidget);
    label->setText("Device name");
    layout->addWidget(label);

    layout->addWidget(lineEdit);
    layout->addStretch();
    backgrounWidget->layout()->addWidget(holderwidget);
    connect(lineEdit, &QLineEdit::textChanged, this, &AddDeviceDialog::onDevNameChanged);
    return holderwidget;
}

QLineEdit* AddDeviceDialog::addOptionField(const QString& optname, const QString& defaultvalue)
{
    QWidget* holderwidget = new QWidget(backgrounWidget);
    QHBoxLayout* layout = new QHBoxLayout(holderwidget);
    layout->setContentsMargins(11,2,11,2);
    QLineEdit* lineEdit = new QLineEdit(holderwidget);
    lineEdit->setText(defaultvalue);
    QLabel* label = new QLabel(holderwidget);
    label->setText(optname);
    layout->addWidget(label);

    layout->addWidget(lineEdit);
    layout->addStretch();
    backgrounWidget->layout()->addWidget(holderwidget);
    return lineEdit;
}

QWidget* AddDeviceDialog::addPinSelectField(QString name)
{
    QWidget* holderwidget = new QWidget(backgrounWidget);
    QHBoxLayout* layout = new QHBoxLayout(holderwidget);
    QLineEdit* lineEdit = new QLineEdit(holderwidget);
    selectedPinLineEdit = lineEdit;

    QPushButton* button = new QPushButton("...", holderwidget);
    QLabel* label = new QLabel(holderwidget);
    pinLineEditList.append(lineEdit);
    pinPushButton.append(button);
    pinCurrentSelected.append(QList<int> {});
    label->setText(name);
    layout->addWidget(label);

    layout->addWidget(lineEdit);
    layout->addWidget(button);
    layout->addStretch();
    backgrounWidget->layout()->addWidget(holderwidget);
    connect(button, &QPushButton::clicked, this, &AddDeviceDialog::onSelectPins);
    return holderwidget;
}

void AddDeviceDialog::onAddSensor()
{
    newDev = Device();
    newDev.type = Device::SENSOR;

    pinLineEditList.clear();
    pinPushButton.clear();
    pinCurrentSelected.clear();

    currentMode = Device::SENSOR;
    if(backgrounWidget != 0){
        backgrounWidget->deleteLater();
    }
    backgrounWidget = new QWidget(this);
    ui->OptionLayout->addWidget(backgrounWidget);


    QVBoxLayout* layout = new QVBoxLayout(backgrounWidget);
    backgrounWidget->setLayout(layout);
    addDevNameField();
    addPinSelectField("Sensor Pin");
    opt1_lineEdit = addOptionField("Acceleration", "400");


    qobject_cast<QVBoxLayout*>(backgrounWidget->layout())->addStretch();
    currentMode = Device::SENSOR;



}

void AddDeviceDialog::onAddServo()
{
    //init newDev
    newDev = Device();
    currentMode = Device::SERVO;


    if(backgrounWidget != 0){
        backgrounWidget->deleteLater();
    }
    backgrounWidget = new QWidget(this);
    ui->OptionLayout->addWidget(backgrounWidget);



    QVBoxLayout* layout = new QVBoxLayout(backgrounWidget);
    backgrounWidget->setLayout(layout);
    addDevNameField();
    addPinSelectField("Servo Pin");

    qobject_cast<QVBoxLayout*>(backgrounWidget->layout())->addStretch();

}

void AddDeviceDialog::onAddStepper()
{
    newDev = Device();
    newDev.type = Device::STEPPER;

    pinLineEditList.clear();
    pinPushButton.clear();
    pinCurrentSelected.clear();

    currentMode = Device::STEPPER;
    if(backgrounWidget != 0){
        backgrounWidget->deleteLater();
    }
    backgrounWidget = new QWidget(this);
    ui->OptionLayout->addWidget(backgrounWidget);

    QVBoxLayout* layout = new QVBoxLayout(backgrounWidget);
    backgrounWidget->setLayout(layout);
    addDevNameField();
    addPinSelectField("DirPin");
    addPinSelectField("StepPin");
    opt1_lineEdit = addOptionField("MaxSpeed", "1000");
    opt2_lineEdit = addOptionField("Acceleration", "500");


    qobject_cast<QVBoxLayout*>(backgrounWidget->layout())->addStretch();
    currentMode = Device::STEPPER;



}

void AddDeviceDialog::onSelectPins()
{
    int maxnum = 1;
    if(currentMode == -1){
        return;
    }
    int index = 0;
    if(sender() == pinPushButton[0]){
        index = 0;
    }else{
        index = 1;
    }
    SetPinDialog* dialog = new SetPinDialog(this);
    dialog->setMaxNum(maxnum);
    dialog->setPeripheralModel(peripheralItemModel, thisBoat);
    dialog->setPeripherals(PeripheralCache);
    dialog->setDev(newDev);
    dialog->setType(currentMode);
    dialog->setIndex(index);


    int ret = dialog->exec();
    if(ret==QDialog::Accepted){
        QString line;
        newDev.PeriID = dialog->getPeriID();
        if(dialog->getPinID().size() == 0){
            return;
        }
        pinCurrentSelected[index] = dialog->getPinID();
        for(int i = 0; i< pinCurrentSelected[index].size(); i++){
            line+=QString::number(newDev.PeriID);
            line+=":";
            line+=QString::number(pinCurrentSelected[index][0]);
            line+=", ";
        }
        pinLineEditList[index]->setText(line);

    }
}

void AddDeviceDialog::onDevNameChanged(const QString& n)
{
    devName = n;
}
