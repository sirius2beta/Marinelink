#include "mainwindow.h"

#include <QApplication>
#include <QTimer>
#include <QSplashScreen>
#include <QStyleFactory>
#include <QPalette>
#include <QDebug>

#include "gui_style_manager.h"
#include "settings.h"

int main(int argc, char *argv[])
{

    //initialize gstreamer
    gst_init (&argc, &argv);
    QApplication a(argc, argv);

    a.setOrganizationName("Ezosirius");
    a.setApplicationName("GPlayer_v1");


    a.setStyle(QStyleFactory::create("Fusion"));
    /*
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    //darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(200, 200, 200));

    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Disabled, QPalette::Button,QColor(50,50,50));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText,QColor(150,150,150));

    darkPalette.setColor(QPalette::Base,QColor(45,45,45));
    */
    presentation::GuiStyleManager guiStyleManager;
    guiStyleManager.setPaletteStyle(presentation::GuiStyleManager::Night);
    a.setPalette(guiStyleManager.getPallete());
    a.setFont(QFont("Microsoft JhengHei UI", 9, QFont::Normal));
    a.setStyleSheet("QDockWidget::title { background-color:#202020; };");


    //create splash screen
    QPixmap pixmap(":/splash/icon/mssplash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();

    MainWindow w(0,QString("config1"));
    w.setWindowState(w.windowState() | Qt::WindowMaximized);

    w.show();
    splash.finish(&w);
    return a.exec();
}
