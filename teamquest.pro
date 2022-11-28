QT       += core gui
QT += widgets serialport network
CONFIG += c++14




# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS



# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0






SOURCES += \
    CMFRC522/CMFRC522.cpp \
    MCU.cpp \
    customstate.cpp \
    keyboardgrabber.cpp \
    main.cpp \
    mainwindow.cpp \
    muxreader.cpp \
    rfidreader.cpp \
    segmentstimer.cpp \
    videoplayer.cpp

HEADERS += \
    CMFRC522/CMFRC522.h \
    customstate.h \
    keyboardgrabber.h \
    mainwindow.h \
    muxreader.h \
    rfidreader.h \
    segmentstimer.h

# Default rules for deployment.
target.path = /home/pi
INSTALLS += target
LIBS += -lpigpio

