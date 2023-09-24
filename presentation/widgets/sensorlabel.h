#ifndef SENSORLABEL_H
#define SENSORLABEL_H

#include <QWidget>

namespace Ui {
class SensorLabel;
}

class SensorLabel : public QWidget
{
    Q_OBJECT

public:
    explicit SensorLabel(QWidget *parent = nullptr);
    ~SensorLabel();
    void setSensorName(QString name);
    int uid;
    char unit;
    void setValue(QString value);
protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::SensorLabel *ui;
};

#endif // SENSORLABEL_H
