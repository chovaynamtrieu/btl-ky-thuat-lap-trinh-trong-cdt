QT       += core gui serialport multimedia network concurrent texttospeech widgets

requires(qtHaveModule(gui))
qtHaveModule(gui): requires(qtConfig(combobox))

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    comserial.cpp \
    connect_function.cpp \
    data_analysis.cpp \
    main.cpp \
    mainwindow.cpp \
    menu_action.cpp \
    speech_function.cpp

HEADERS += \
    comserial.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon-resource.qrc
