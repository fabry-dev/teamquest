#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qserialport.h>

#include "segmentstimer.h"
#include "muxreader.h"
#include "rfidreader.h"
#include "qstatemachine.h"
#include "qhistorystate.h"
#include "keyboardgrabber.h"
#include "qlabel.h"
#include "qstate.h"
#include "customstate.h"
#include <QMouseEvent>
#include <QLocalSocket>

#define MCU_RELAY (uint8_t)1
#define MCU_LED (uint8_t)2
#define MCU_VOYANT (uint8_t)3


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initMCUSerial(void);

    std::vector<bool> relaysStates,ledsStates,voyantsStates;
    void turnRelayOn(uint relayId);
    void turnRelayOff(uint relayId);
    void turnAllRelaysOn();
    void turnAllRelaysOff();

    void turnLedOn(uint ledId);
    void turnLedOff(uint ledId);
    void turnAllLedsOn();
    void turnAllLedsOff();








    void turnVoyantOn(uint voyantId);
    void turnVoyantOff(uint voyantId);
    void turnAllVoyantsOn();
    void turnAllVoyantsOff();

    void sendToMCU(uint8_t code,uint8_t value);

    void gameProcess(void);
    void setStateWhiteList(customState *state, QString button="", QString wire="", uint disquette=0);
    void setStateWhiteLists(customState *state, QStringList buttons={},QString wire="",uint disquette=0);
    QStateMachine gameMachine,joystickMachine;

    QSerialPort *MCUPort;
    bool led7State,led8State,led9State,led10State;

    keyGrabber *keyboard ;
    QProcess videoProcess,soundLoopProcess,soundProcess,joySoundProcess,joyAlertProcess;

    void playMPV(QString filename);
    void playMPVLoop(QString filename);

    void sendIpc(QString, QString server);
    QLabel *imgLbl;
    segmentsTimer *st;
    muxReader *inputReader;
    rfidReader *rfid;
    QImage pic_image1,pic_image2,pic_image3,pic_image4,pic_image5,pic_image6,pic_image7,pic_image8,pic_image9,pic_image10,pic_image11,pic_image12,pic_image13,pic_image14;
    customState *stateBeforeError;
    customState *error,*game;
    customState *error0,*error1,*error2,*error3;

    customState *game_idle,*video1,*image1,*image2,*image2_2,*image2_3,*image2_4,*image2_5,*video2,*video2_2,*video2_3;
    customState *image3,*video3,*image4,*image5,*image6,*image7,*video4,*image8,*image9,*image10,*image11,*image11_2;
    customState *anagramme1,*anagramme2,*anagramme12,*video5,*video6,*final;

    QHistoryState *gameh,*errorh;

    uint errorCount=0;

    QTimer *timerLed7,*timerLed8,*timerLed9,*timerLed10;

    uint voyantIndex;
    QTimer *voyantsTimer;
private slots:

    void updateLed7State(void);
    void updateLed8State(void);
    void updateLed9State(void);
    void updateLed10State(void);
    void stop78910Led(void);
    void start78910Led(void);

    void stopVideo(void);
    void stopSoundLoop(void);
    void playSoundLoop();
    void stopSound(void);
    void stopJoySound(void);

    void initGame(void);
    void gotError(void);
    void errorFilter(void);
    void createSoundPlayer(void);
    void createVideoPlayer(void);
    void createJoySoundPlayer(void);

    void openElectroAimant(){turnRelayOff(0);}
    void closeElectroAimant(){turnRelayOn(0);}
    void openSolenoide1(){turnRelayOff(1);}
    void closeSolenoide1(){turnRelayOn(1);}
    void openSolenoide2(){turnRelayOff(2);}
    void closeSolenoide2(){turnRelayOn(2);}
    void openSolenoide3(){turnRelayOff(3);}
    void closeSolenoide3(){turnRelayOn(3);}
    void openSolenoide4(){turnRelayOff(4);}
    void closeSolenoide4(){turnRelayOn(4);}
    void turnOffDigicode(){turnRelayOff(5);}
    void turnOnDigicode(){turnRelayOn(5);}
    void turnOffGyro(){turnRelayOff(6);}
    void turnOnGyro(){turnRelayOn(6);}
    void turnOffVentil(){turnRelayOff(7);}
    void turnOnVentil(){turnRelayOn(7);}

    void turnOnLed1(){turnLedOn(0);}    void turnOffLed1(){turnLedOff(0);}
    void turnOnLed2(){turnLedOn(1);}    void turnOffLed2(){turnLedOff(1);}
    void turnOnLed3(){turnLedOn(2);}    void turnOffLed3(){turnLedOff(2);}
    void turnOnLed4(){turnLedOn(3);}    void turnOffLed4(){turnLedOff(3);}
    void turnOnLed5(){turnLedOn(4);}    void turnOffLed5(){turnLedOff(4);}
    void turnOnLed6(){turnLedOn(5);}    void turnOffLed6(){turnLedOff(5);}


    void initLedStates();
    void initRelayStates();
    void initVoyantStates();
    void setPicture(QImage *pic);
    void playSound(QString filename);
    void playJoySound(QString filename);
    void playVideo(QString filename);

    void joyPlayAlertSound(void);
    void joyStopAlertSound(void);


    void MCUConnect(void);
    void handleSerialError(QSerialPort::SerialPortError errorCode);
    void test(void);

    void stopVoyants(void);
    void startVoyants(void);
    void topVoyants(void);
signals:
    void videoFinished(void);
    void videoStarted(void);
    void answerA(void);void answerB(void);void answerC(void);void answerD(void);
    void gotButtonError(void);
    void forceGo(void);
    void voyantsOver(void);

protected:
    void mousePressEvent(QMouseEvent *eventPress);

};







#endif // MAINWINDOW_H
