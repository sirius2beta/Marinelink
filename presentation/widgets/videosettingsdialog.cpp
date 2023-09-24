#include "videosettingsdialog.h"
#include "ui_videosettingsdialog.h"


VideoSettingsDialog::VideoSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VideoSettingsDialog)
{
    ui->setupUi(this);
    ui->PC_port_edit->setInputMask(QString("99999"));
    setWindowTitle("Streaming Settings");

    ui->videonoComboBox->addItem("video0", "video0");
    ui->videonoComboBox->addItem("video1", "video1");
    ui->videonoComboBox->addItem("video2", "video2");
    ui->videonoComboBox->addItem("video3", "video3");
    ui->videonoComboBox->addItem("video4", "video4");
    ui->videonoComboBox->addItem("video5", "video5");
    ui->videonoComboBox->addItem("video6", "video6");

    //ui->formatComboBox->addItem("320 x 240 15fps", "320-240-15");
    ui->formatComboBox->addItem("320 x 240 30fps", "320-240-30");
    //ui->formatComboBox->addItem("640 x 480 15fps", "640-480-15");
    ui->formatComboBox->addItem("640 x 480 30fps", "640-480-30");
    ui->formatComboBox->addItem("800 x 600 20fps", "800-600-20");
    ui->formatComboBox->addItem("800 x 600 30fps", "800-600-30");


    connect(ui->boat_name_edit,SIGNAL(textChanged(QString)),this, SLOT(onTitleChanged(QString)));
    connect(ui->videonoComboBox,SIGNAL(activated(int)),this, SLOT(onVideonoChanged(int)));
    connect(ui->formatComboBox, SIGNAL(activated(int)), this, SLOT(selectFormat(int)));
    connect(ui->optionEdit,SIGNAL(textChanged(QString)),this, SLOT(onOptionsChanged(QString)));
    connect(ui->qualityEdit,SIGNAL(textChanged(QString)),this, SLOT(onQualityChanged(QString)));
    connect(ui->PC_port_edit, SIGNAL(textChanged(QString)),this,SLOT(onPCPortChanged(QString)));
    connect(ui->videoinfoCheckBox, &QCheckBox::stateChanged,this,&VideoSettingsDialog::onVideoInfoChanged);

}

void VideoSettingsDialog::setInfo(QString _title, QString _boatname, int _PCPort, int videono, int formatno, bool video_info)
{
    __boatname = _boatname;
    __video_info = video_info;

    ui->titleLineEdit->setText(_title);
    ui->boat_name_edit->setText(__boatname);
    ui->videonoComboBox->setCurrentIndex(videono);
    ui->formatComboBox->setCurrentIndex(formatno);
    ui->PC_port_edit->setText(QString::number(_PCPort));

    if(video_info){
        ui->videoinfoCheckBox->setChecked(true);
    }else{
        ui->videoinfoCheckBox->setChecked(false);
    }

    qDebug()<<"setInfo";

}

void VideoSettingsDialog::setFormat(QAbstractItemModel* model, int index)
{
    ui->formatComboBox->setModel(model);
    ui->formatComboBox->setCurrentIndex(index);

}

void VideoSettingsDialog::setVideo(QAbstractItemModel *model, int index)
{
    ui->videonoComboBox->setModel(model);
    ui->videonoComboBox->setCurrentIndex(index);
}


VideoSettingsDialog::~VideoSettingsDialog()
{
    delete ui;
}

void VideoSettingsDialog::accept()
{
    emit comit(ui->titleLineEdit->text(), __boatname, __PCPort, ui->videonoComboBox->currentIndex(), ui->formatComboBox->currentIndex(), __video_info);

    QDialog::accept();
}

void VideoSettingsDialog::onTitleChanged(QString title)
{
    __boatname = title;
}






void VideoSettingsDialog::onOptionsChanged(QString options)
{
}



void VideoSettingsDialog::onPCPortChanged(QString text)
{
    __PCPort = text.toInt();
}


void VideoSettingsDialog::onVideoInfoChanged(int i)
{
    __video_info = ui->videoinfoCheckBox->isChecked();
}

QString VideoSettingsDialog::title()
{
    return __boatname;
}
