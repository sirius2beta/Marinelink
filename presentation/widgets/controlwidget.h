#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include "boats.h"

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget();

    void setBoatList(Boats* bs);
signals:
    void sendMsg(QString boatname, char topic, QByteArray command);
public slots:
    void update();
    void onStart();

private:
    Ui::ControlWidget *ui;
    Boats* boatList;
    QStandardItemModel* model;
};

#endif // CONTROLWIDGET_H
