# Build environment:
#  = Windows 11
#  = Qt 5.15.2
#  = mingw64
#  = GStreamer-1.0-mingw64 1.20.3

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
#CONFIG += with_cuda

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += DEBUG_MODE
SOURCES += \
    presentation/widgets/adddevicedialog.cpp \
    source/dto/basedto.cpp \
    source/dto/boats.cpp \
    boatsetting.cpp \
    presentation/widgets/comboboxdelegate.cpp \
    presentation/widgets/configdialog.cpp \
    presentation/widgets/controlwidget.cpp \
    presentation/widgets/createwindowdialog.cpp \
    source/dto/device.cpp \
    main.cpp \
    presentation/widgets/mainwindow.cpp \
    networksettings.cpp \
    presentation/common/gui_style/gui_style_manager.cpp \
    presentation/widgets/sensorlabel.cpp \
    presentation/widgets/sensorwidget.cpp \
    presentation/widgets/setpindialog.cpp \
    source/settings_provider.cpp \
    presentation/widgets/videosettingsdialog.cpp \
    presentation/widgets/videowindow.cpp \

HEADERS += \
    presentation/widgets/adddevicedialog.h \
    source/dto/basedto.h \
    source/dto/boats.h \
    boatsetting.h \
    presentation/widgets/comboboxdelegate.h \
    presentation/widgets/configdialog.h \
    presentation/widgets/controlwidget.h \
    presentation/widgets/createwindowdialog.h \
    source/dto/device.h \
    presentation/widgets/mainwindow.h \
    networksettings.h \
    presentation/common/gui_style/gui_style_manager.h \
    presentation/widgets/sensorlabel.h \
    presentation/widgets/sensorwidget.h \
    presentation/widgets/setpindialog.h \
    source/settings.h \
    source/settings_provider.h \
    presentation/widgets/videosettingsdialog.h \
    presentation/widgets/videowindow.h \

FORMS += \
    presentation/widgets/adddevicedialog.ui \
    presentation/widgets/boatsetting.ui \
    presentation/widgets/configdialog.ui \
    presentation/widgets/controlwidget.ui \
    presentation/widgets/createwindowdialog.ui \
    presentation/widgets/mainwindow.ui \
    presentation/widgets/networksettings.ui \
    presentation/widgets/sensorlabel.ui \
    presentation/widgets/sensorwidget.ui \
    presentation/widgets/setpindialog.ui \
    presentation/widgets/videosettingsdialog.ui \
    presentation/widgets/videowindow.ui



INCLUDEPATH += "source"
INCLUDEPATH += "presentation\common\gui_style"
INCLUDEPATH += "presentation\widgets"
INCLUDEPATH += "resource\icon"
INCLUDEPATH += "source\dto"

INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\include"
INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\include\gstreamer-1.0"
INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\include\glib-2.0"
INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\lib\glib-2.0\include"
INCLUDEPATH += "C:\gstreamer_MSVC\1.0\msvc_x86_64\lib\gstreamer-1.0\include"

CONFIG += link_pkgconfig
PKGCONFIG += gstreamer-1.0

LIBS += -LC:\gstreamer_MSVC\1.0\msvc_x86_64/lib/ -lgstvideo-1.0

with_cuda{
    #INCLUDEPATH += "C:\opencv-4.6\build_with_cuda\install\include"
    #LIBS += -LC:/opencv-4.6/build_with_cuda/install/x64/lib\ -lopencv_world460
} else{
    #INCLUDEPATH += "C:\opencv_MSVC_no_cuda\include"
    #LIBS += -LC:/opencv_MSVC_no_cuda/x64/lib\ -lopencv_world460
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource\image.qrc

DISTFILES += \
    resource\logo.rc
RC_FILE += \
    resource\logo.rc
