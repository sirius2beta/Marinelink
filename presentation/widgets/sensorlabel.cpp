#include "sensorlabel.h"
#include "ui_sensorlabel.h"
#include <QStyleOption>
#include <QPainter>

SensorLabel::SensorLabel(QWidget *parent) :
    QWidget(parent),
    unit('i'),
    ui(new Ui::SensorLabel)
{
    ui->setupUi(this);
}

SensorLabel::~SensorLabel()
{
    delete ui;
}

void SensorLabel::setSensorName(QString name)
{
    ui->sensor_name->setText(name);
}

void SensorLabel::setValue(QString value)
{
    ui->value->setText(value);
}

void SensorLabel::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
