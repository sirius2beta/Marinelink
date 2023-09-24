#include "videowindow.h"
#include "ui_videowindow.h"

#define HEARTBEAT 0x10
#define FORMAT 0x20
#define COMMAND 0x30
#define QUIT 0x40

VideoWindow::VideoWindow(QWidget *parent, QString config) :
    QWidget(parent),
    _config(config),
    PCPort(0),
    title(QString("VideoWindow")),
    isPlaying(false),
    ui(new Ui::VideoWindow)
{
    ui->setupUi(this);
    ui->playButton->setIcon(QIcon(":/icon/icon/playbutton.png"));
    ui->stopButton->setIcon(QIcon(":/icon/icon/stopbutton.png"));
    ui->settingsButton->setIcon(QIcon(":/icon/icon/settingbutton-02.png"));

    //QCoreApplication::setOrganizationName("Ezosirius");
    //QCoreApplication::setApplicationName("GPlayer_v1");
    settings = new QSettings;

    connect(ui->settingsButton,&QPushButton::clicked,this,&VideoWindow::onSettings);
    connect(ui->playButton, &QPushButton::clicked, this, &VideoWindow::onPlay);
    connect(ui->stopButton, &QPushButton::clicked, this, &VideoWindow::onStop);


    connect(ui->videoportComboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this,&VideoWindow::setVideoNo);
    connect(ui->videoFormatcomboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this,&VideoWindow::onSetFormatNo);
    connect(ui->boatcomboBox, (void (QComboBox::*)(int))&QComboBox::currentIndexChanged, this,&VideoWindow::onSetBoatNo);

}

VideoWindow::~VideoWindow()
{
    emit stopworker();
    if(isPlaying == false){

    }else{
        gst_element_set_state (pipeline, GST_STATE_NULL);
        gst_object_unref (pipeline);
    }

    delete ui;
    workThread.quit();
    workThread.wait();
    workThread.destroyed();
}

void VideoWindow::init()
{
    /*
    QHostAddress addr = QHostAddress(boatIP);
    QString cmd = QString("qformat 1");
    serverSocket->writeDatagram(cmd.toUtf8().data(),cmd.toUtf8().size(), addr, 50007);
    qDebug()<<"format "+boatIP;
    */
}

void VideoWindow::setFormat()
{
    ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(title, QString::number(PCPort),ui->videoportComboBox->currentText()));
}

void VideoWindow::setPCPort(int p)
{
    PCPort = p;
    ui->screen_text->setText(QString("%1 (port : %2)").arg(title, QString::number(PCPort)));
}


void VideoWindow::setTitle(QString t)
{
    title = t;
    ui->screen_text->setText(QString("%1 (port : %2)").arg(title, QString::number(PCPort)));
}

void VideoWindow::setIndex(int i)
{
    index = i;
}

void VideoWindow::setVideoInfo(bool i)
{
    isVideoInfo = i;
}

void VideoWindow::onSettings()
{
    VideoSettingsDialog* dialog = new VideoSettingsDialog(this);
    dialog->setInfo(title, ui->boatcomboBox->currentText(), PCPort, videoNo, formatNo,isVideoInfo);
    dialog->setFormat(ui->videoFormatcomboBox->model(), ui->videoFormatcomboBox->currentIndex());
    dialog->setVideo(ui->videoportComboBox->model(),ui->videoportComboBox->currentIndex());
    connect(dialog, &VideoSettingsDialog::comit, this, &VideoWindow::changeSettings);
    connect(dialog, &VideoSettingsDialog::comit, this, &VideoWindow::onStop);
    dialog->exec();
}

void VideoWindow::onPlay()
{

    QString gstcmd;
    QString encoder = "jpegenc";
    if(isVideoInfo){
        if(ui->H264checkBox->isChecked()){
            gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtph264depay ! avdec_h264 ! videoconvert  !\
             textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 14\" !\
             glimagesink name=mySink2").arg(QString::number(PCPort),title,ui->videoportComboBox->currentText());
            encoder = "h264";
        }else{
             gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
             textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 14\" !\
             glimagesink name=mySink2").arg(QString::number(PCPort),title,ui->videoportComboBox->currentText());
        }

    }else{
        if(ui->H264checkBox->isChecked()){
            gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtph264depay ! avdec_h264 ! videoconvert  !\
             glimagesink name=mySink2").arg(QString::number(PCPort));
            encoder = "h264";
        }else{
             gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
             glimagesink name=mySink2").arg(QString::number(PCPort));
        }
    }



    ui->playButton->setEnabled(false);
    emit sendCommand(ui->boatcomboBox->currentText(), ui->videoportComboBox->currentText()+" "+ui->videoFormatcomboBox->currentText()+" "+encoder+" nan"+" 90", PCPort);



    ui->playButton->setEnabled(false);
    QTimer::singleShot(100,[=]{
        ui->playButton->setEnabled(true);
    });


    if(isPlaying == false){

    }else{
        gst_element_set_state (pipeline, GST_STATE_NULL);
    }
    qDebug()<<"play";

    pipeline= gst_parse_launch(gstcmd.toLocal8Bit(), NULL);
    sink = gst_bin_get_by_name((GstBin*)pipeline,"mySink2");
    WId xwinid = ui->screen_text->winId();
    gst_video_overlay_set_window_handle (GST_VIDEO_OVERLAY (sink), xwinid);
    gst_element_set_state (pipeline,
        GST_STATE_PLAYING);
    isPlaying = true;

}

void VideoWindow::onStop()
{
    std::string s = QString(ui->videoportComboBox->currentText()).toStdString();
    emit sendMsg(ui->boatcomboBox->currentText(), char(QUIT), QByteArray(s.c_str()));

    if(isPlaying == false){

    }else{
        gst_element_set_state (pipeline, GST_STATE_NULL);
        isPlaying = false;
    }
}


void VideoWindow::setVideoFormat(int ID, QStringList videoformat)
{   

    if(ui->boatcomboBox->currentText() == boatList->getBoatbyID(ID)->name){


        int preVideoNo = ui->videoportComboBox->currentIndex();
        int preVideoFormat = ui->videoFormatcomboBox->currentIndex();
        ui->videoportComboBox->clear();
        ui->videoFormatcomboBox->clear();

        QStringList videoFormatList;
        QString thisvideoNo; //videox
        int index = -1;
        for(const auto &vf:videoformat){
            QString videoNo = vf.split(' ')[0];
            if(videoNo == thisvideoNo){ //the same videox
                QStringList vfl = vf.split(' ');
                vfl.pop_front();
                if(vfl[1].split('=')[1].toInt()<= 1920){  //limit with<1920
                    QString vfstring = vfl.join(' ');
                    videoFormatList<<vfstring; //save videoformat of videox to videoFormatList
                }

            }else{ //next video port(ex. video1 to video2)
                ui->videoportComboBox->setItemData(index, videoFormatList);
                ui->videoportComboBox->addItem(videoNo, 0);
                thisvideoNo = videoNo;
                videoFormatList.clear();
                index++;
            }
        }
        ui->videoportComboBox->setItemData(index, videoFormatList);
        qDebug()<<"VideoWindow "<<this->index<<", pre-index count: "<< ui->videoportComboBox->count()<<" , videoNo: "<<videoNo;
        videoFormatList = ui->videoportComboBox->currentData().toStringList();
        for(int i = 0;i<videoFormatList.size(); i++){
            ui->videoFormatcomboBox->addItem(videoFormatList[i],0);

        }
        if(ui->videoportComboBox->count() > preVideoNo){
            if(preVideoNo == -1){
                ui->videoportComboBox->setCurrentIndex(videoNo);
            }else{
                ui->videoportComboBox->setCurrentIndex(preVideoNo);
                setVideoNo(preVideoNo);
            }

        }

        if(ui->videoFormatcomboBox->count() > formatNo){
            ui->videoFormatcomboBox->setCurrentIndex(formatNo);
        }

    }

}

void VideoWindow::setBoatList(Boats* boatlist)
{
    boatList = boatlist;
    for(int i = 0; i < boatlist->size(); i++){
        QString boat_name = boatlist->getBoatbyIndex(i)->name;
        ui->boatcomboBox->addItem(boat_name,boat_name);
    }
}

void VideoWindow::changeSettings(QString _title, QString boatname,int PCPort, int videono, int formatno, bool video_info)
{
    setPCPort(PCPort);
    setTitle(_title);
    setVideoInfo(video_info);
    ui->videoportComboBox->setCurrentIndex(videono);
    ui->videoFormatcomboBox->setCurrentIndex(formatno);

    settings->setValue(QString("%1/w%2/title").arg(_config,QString::number(index)),PCPort);
    settings->setValue(QString("%1/w%2/boat_name").arg(_config,QString::number(index)),boatname);
    settings->setValue(QString("%1/w%2/in_port").arg(_config,QString::number(index)),PCPort);
    settings->setValue(QString("%1/w%2/videono").arg(_config,QString::number(index)), videono);
    settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(index)), formatno);
    settings->setValue(QString("%1/w%2/title").arg(_config,QString::number(index)), title);
    if(isVideoInfo){
        settings->setValue(QString("%1/w%2/videoinfo").arg(_config,QString::number(index)), 1);
    }else{
        settings->setValue(QString("%1/w%2/videoinfo").arg(_config,QString::number(index)), 0);
    }
    qDebug()<<"start changesettings3";

    QString gstcmd;
    if(isVideoInfo){
        gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
     textoverlay text=\"%2\n%3\nPort:%1\" valignment=top halignment=right font-desc=\"Sans, 18\" !\
     glimagesink name=mySink2").arg(QString::number(PCPort),title,ui->videoportComboBox->currentText());
    }else{
        gstcmd = QString("udpsrc port=%1 ! application/x-rtp, media=video, clock-rate=90000, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert  !\
     glimagesink name=mySink2").arg(QString::number(PCPort),title,ui->videoportComboBox->currentText());
    }
    worker->setGstcmd(gstcmd);


    QDockWidget* dockwidget = (QDockWidget*)parent();
    dockwidget->setWindowTitle(title);
    //ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(title, QString::number(port),ui->videoportComboBox->currentText()));

}



void VideoWindow::handleResult(const QPixmap &result)
{
   ;
   ui->screen_text->setPixmap(result.scaledToHeight(ui->screen_text->height()));

}

void VideoWindow::clearScreen(){
    ui->screen_text->clear();
    ui->screen_text->setText(QString("%1\n%3\n(port : %2)").arg(title, QString::number(PCPort),ui->videoportComboBox->currentText()));
}


void VideoWindow::onUDPMsg()
{
    while(clientSocket->hasPendingDatagrams()){
        QByteArray data;
        QHostAddress addr;
        QString ip;
        data.resize(clientSocket->pendingDatagramSize());
        clientSocket->readDatagram(data.data(),data.size(),&addr);

        QString topic = data.split(' ')[0];
        QString message;
        if(data.split(' ').size() >1){
            message = data.split(' ')[1];
        }
        if(topic == QString("alive")){

        }
        const QString content = QLatin1String(" Received Topic: ")
                    + topic
                    + QLatin1String(" Message: ")
                    + message
                    + QLatin1Char('\n');
        qDebug() << content;
    }
}

void Worker::setGstcmd(const QString cmd)
{
    gstcmd = cmd;
}

Worker::Worker(QObject *parent) : QObject(parent)
{
    /*
    yolov5 = new YOLOV5();
    #ifdef DEBUG_MODE
    yolov5->initConfig("D:\\OneDrive\\Source\\Qt\\resource\\yolov5s.onnx", 640, 640, 0.25f);
    #else
    yolov5->initConfig("yolov5s.onnx", 640, 640, 0.25f);
    #endif
    initiated = false;
    */
}

Worker::~Worker()
{
    /*
    capture->release();
    delete capture;
    delete yolov5;
    if(initiated){
        delete timer;
    }
    */
}

Worker::Worker(QString name , QObject *parent )
{
    /*
    this->name = name;
    yolov5 = new YOLOV5();
    #ifdef DEBUG_MODE
    yolov5->initConfig("D:\\OneDrive\\Source\\Qt\\resource\\yolov5s.onnx", 640, 640, 0.25f);
    #else
    yolov5->initConfig("yolov5s.onnx", 640, 640, 0.25f);
    #endif
    initiated = false;
*/

}



void Worker::doWork(const QString cmd)
{
    /*
    if(cmd != QString()){
        gstcmd = cmd;
    }
    if(!initiated){
        capture = new cv::VideoCapture(gstcmd.toStdString(), cv::CAP_GSTREAMER);
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &Worker::update);
        timer->start(33);
        initiated = true;
    }else{
        delete capture;
        capture = new cv::VideoCapture(gstcmd.toStdString(), cv::CAP_GSTREAMER);
        timer->start(33);
    }
*/

}

void Worker::restart(const QString cmd)
{
    /*
    timer->stop();
    delete capture;
    capture = new cv::VideoCapture(gstcmd.toStdString(), cv::CAP_GSTREAMER);
    timer->start(33);
    */
}

void Worker::update(){
    /*
    cv::Mat frame;
    capture->read(frame);
    if(frame.empty()) return;
    yolov5->detect2(frame);
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);


   QImage rawImage = QImage(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
   emit resultReady(QPixmap::fromImage(rawImage));
   */
}


void Worker::stop()
{
    /*
    timer->stop();
    capture->release();
    */
}


void Worker::exit(){
    /*
    capture->release();
    delete capture;
    delete yolov5;
    this->deleteLater();
    */
}

void VideoWindow::resetBoatName(QString boatname,QString newboatname)
{
    for(int i = 0; i < ui->boatcomboBox->count();i++){
        if(ui->videoportComboBox->itemText(i) == boatname){
            ui->videoportComboBox->setItemText(i, newboatname);
        }
    }

}



void VideoWindow::AddBoat(QString boatname)
{
    qDebug()<<"Videowindow: add boat: "<<boatname;
    ui->boatcomboBox->addItem(boatname);
}

void VideoWindow::onBoatNameChange(QString oldname, QString newname)
{
    for(int i = 0; i < ui->boatcomboBox->count();i++){
        if(ui->boatcomboBox->itemText(i) == oldname){
            ui->boatcomboBox->setItemText(i, newname);
        }
    }
}

void VideoWindow::onDeleteBoat(QString boatname)
{

    for(int i = 0; i < ui->boatcomboBox->count(); i++){
        if(ui->boatcomboBox->itemText(i) == boatname){
            ui->boatcomboBox->removeItem(i);
        }
    }

}

void VideoWindow::setVideoNo(int i)
{
    videoNo = i;
    ui->videoFormatcomboBox->clear();
    qDebug()<<"VideoWindow "<<this->index<<" set videoNo : "<<videoNo;
    QStringList formatList = ui->videoportComboBox->itemData(i).toStringList();
    for(int j = 0; j<formatList.size(); j++){
        ui->videoFormatcomboBox->addItem(formatList[j],0);
    }
    setFormat();

}

void VideoWindow::setFormatNo(int i)
{
    formatNo = i;

}

void VideoWindow::onSetFormatNo(int i)
{
    formatNo = i;
    settings->setValue(QString("%1/w%2/formatno").arg(_config,QString::number(this->index)), i);
    qDebug()<<"setformatno 2: "<<i;
}

void VideoWindow::onSetBoatNo(int i)
{
    emit sendMsg(ui->boatcomboBox->itemText(i),char(FORMAT),"qformat");
    ui->videoFormatcomboBox->clear();
    ui->videoportComboBox->clear();
}
