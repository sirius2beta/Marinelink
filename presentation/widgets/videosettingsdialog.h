#ifndef VIDEOSETTINGSDIALOG_H
#define VIDEOSETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QStringList>
#include <QDebug>

//#include "mainwindow.h"

namespace Ui {
class VideoSettingsDialog;
}

class VideoSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VideoSettingsDialog(QWidget *parent = nullptr);
    ~VideoSettingsDialog();
    void setInfo(QString _title, QString _boatname, int _PCPort, int videono, int formatno, bool video_info);
    void accept();
    QString title();
    void setFormat(QAbstractItemModel *model, int index);
    void setVideo(QAbstractItemModel *model, int index);

signals:
    void comit(QString title, QString boatname, int _boatPort, int videoNo, int formatNo, bool video_info);
protected slots:
    void onTitleChanged(QString title);
    void onOptionsChanged(QString options);
    void onPCPortChanged(QString text);
    void onVideoInfoChanged(int i);


private:
    Ui::VideoSettingsDialog *ui;
    QString __boatname;
    bool __video_info;
    QString __PCIP;
    int __PCPort;
    QString __title;

    //struct mosquitto *mosq;
};

#endif // VIDEOSETTINGSDIALOG_H
