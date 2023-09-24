#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
#include <QStandardItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "boats.h"
#include "setpindialog.h"

namespace Ui {
class AddDeviceDialog;
}

class AddDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDeviceDialog(QWidget *parent = nullptr);
    ~AddDeviceDialog();
    void setPeripheralModel(QStandardItemModel* pm, Boat* boat);
    Device getDevice();
    QString getDevName();
    QWidget *addDevNameField();
    QLineEdit *addOptionField(const QString& optname, const QString &defaultvalue);
    QWidget *addPinSelectField(QString name);

signals:
    void addDev();
protected slots:
    void onAddSensor();
    void onAddServo();
    void onAddStepper();
    void onSelectPins();
    void onDevNameChanged(const QString& n);

private:
    Ui::AddDeviceDialog *ui;
    Boat* thisBoat;
    QStandardItemModel* peripheralItemModel;
    QList<Peripheral> PeripheralCache;
    QWidget* backgrounWidget;
    int currentMode;
    Device newDev;

    QString devName;
    QLineEdit* selectedPinLineEdit;
    QList<QLineEdit*> pinLineEditList;
    QList<QPushButton*> pinPushButton;
    QList<QList<int>> pinCurrentSelected;
    QLineEdit* opt1_lineEdit;
    QLineEdit* opt2_lineEdit;


};

#endif // ADDDEVICEDIALOG_H
