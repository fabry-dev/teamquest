#include "mainwindow.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "qprocess.h"
#include "QDebug"
#include "pigpio.h"
#include "qthread.h"
#include "QTimer"
#include <QTime>
#include "customstate.h"
#include <QSet>
#include <QHistoryState>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800,480);
    setStyleSheet("background-color:black;");
    qsrand(static_cast<quint64>(QTime::currentTime().msecsSinceStartOfDay()));

    system("amixer set Speaker 50%");
    setFocusPolicy(Qt::StrongFocus);
    gpioInitialise();
    initMCUSerial();

    st = new segmentsTimer(this,30*60);//30 min


    QThread* muxThread = new QThread;
    inputReader = new muxReader();
    inputReader->moveToThread(muxThread);
    connect(muxThread,SIGNAL(started()),inputReader,SLOT(readProcess()));
    muxThread->start();

    QThread* rfidThread = new QThread;
    rfid = new rfidReader();
    //connect(rfid,SIGNAL(rfidChanged(std::vector<uint>)),this,SLOT(rfidChanged(std::vector<uint>)));
    rfid->moveToThread(rfidThread);
    connect(rfidThread,SIGNAL(started()),rfid,SLOT(readProcess()));
    rfidThread->start();


    timerLed7 = new QTimer(this);
    timerLed7->setInterval(50);
    connect(timerLed7,SIGNAL(timeout()),this,SLOT(updateLed7State()));

    timerLed8 = new QTimer(this);
    timerLed8->setInterval(60);
    connect(timerLed8,SIGNAL(timeout()),this,SLOT(updateLed8State()));

    timerLed9 = new QTimer(this);
    timerLed9->setInterval(70);
    connect(timerLed9,SIGNAL(timeout()),this,SLOT(updateLed9State()));

    timerLed10 = new QTimer(this);
    timerLed10->setInterval(80);
    connect(timerLed10,SIGNAL(timeout()),this,SLOT(updateLed10State()));


    voyantsTimer = new QTimer(this);
    voyantsTimer->setInterval(60*1000);
    connect(voyantsTimer,SIGNAL(timeout()),this,SLOT(topVoyants(void)));

    QThread* keyThread = new QThread;
    keyboard = new keyGrabber();
    keyboard->moveToThread(keyThread);
    connect(keyThread,SIGNAL(started()),keyboard,SLOT(readKeys()));
    keyThread->start();



    imgLbl = new QLabel(this);
    imgLbl->resize(size());
    //imgLbl->setPixmap(QPixmap::fromImage(QImage("/home/pi/multimedia/IMAGE_1.png")).scaledToWidth(width()));
    imgLbl->hide();

    createSoundPlayer();
    createJoySoundPlayer();
    pic_image1.load("/home/pi/multimedia/IMAGE_1.png");
    pic_image2.load("/home/pi/multimedia/IMAGE_2.png");
    pic_image3.load("/home/pi/multimedia/IMAGE_3.png");
    pic_image4.load("/home/pi/multimedia/IMAGE_4.png");
    pic_image5.load("/home/pi/multimedia/IMAGE_5.png");
    pic_image6.load("/home/pi/multimedia/IMAGE_6.png");
    pic_image7.load("/home/pi/multimedia/IMAGE_7.png");
    pic_image8.load("/home/pi/multimedia/IMAGE_8.png");
    pic_image9.load("/home/pi/multimedia/IMAGE_9.png");
    pic_image10.load("/home/pi/multimedia/IMAGE_10.png");
    pic_image11.load("/home/pi/multimedia/IMAGE_11.png");
    pic_image12.load("/home/pi/multimedia/IMAGE_12.png");
    pic_image13.load("/home/pi/multimedia/IMAGE_13.png");
    pic_image14.load("/home/pi/multimedia/IMAGE_14.png");


    gameProcess();


    qDebug()<<"game started";
}


void MainWindow::test()
{

}



void MainWindow::gameProcess(void)
{
    std::vector<customState*> gameSates,errorStates;
    std::vector<customState*> joyStates;
    QTimer *t5Timer = new QTimer(this);
    t5Timer->setInterval(5000);
    t5Timer->setSingleShot(true);




    QTimer *joyTimer = new QTimer(this);
    joyTimer->setInterval(60*1000);//1 min avant de perdre
    joyTimer->setSingleShot(true);

    stateBeforeError = NULL;

    led7State=led8State=led9State=led10State=false;



    game = new customState(NULL,"Game launched",NULL,NULL,NULL);
    gameh = new QHistoryState(game);
    error = new customState(NULL,"Erreur",NULL,NULL,NULL);
    errorh = new QHistoryState(error);



    gameMachine.addState(game);
    gameMachine.addState(error);

    game_idle = new customState(game,"Game idle",&gameSates,&stateBeforeError,NULL,"","",NULL);
    video1 = new customState(game,"Video1",&gameSates,&stateBeforeError,(QState*)errorh,"","VIDEO_1.mp4",NULL);
    image1 = new customState(game,"Image 1",&gameSates,&stateBeforeError,(QState*)errorh,"SON_2.mp3","",&pic_image1);
    image2 = new customState(game,"Image 2",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image2);
    image2_2 = new customState(game,"Image 2_2",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image2);
    image2_3 = new customState(game,"Image 2_3",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image2);
    image2_4 = new customState(game,"Image 2_4",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image2);
    image2_5 = new customState(game,"Image 2_5",&gameSates,&stateBeforeError,(QState*)errorh,"SON_3.mp3","",&pic_image2);
    video2 = new customState(game,"Video 2",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","VIDEO_2.mp4");
    video2_2 = new customState(game,"Video 2_2",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3");
    video2_3 = new customState(game,"Video 2_3",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3");
    image3 = new customState(game,"Image 3",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image3);
    video3 = new customState(game,"Video 3",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","VIDEO_3.mp4");
    image4 = new customState(game,"Image 4",&gameSates,&stateBeforeError,(QState*)errorh,"","",&pic_image4);
    image5 = new customState(game,"Image 5",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image5);
    image6 = new customState(game,"Image 6",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image6);
    image7 = new customState(game,"Image 7",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image7);
    video4 = new customState(game,"Video 4",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","VIDEO_4.mp4",NULL);
    image8 = new customState(game,"Image 8",&gameSates,&stateBeforeError,(QState*)errorh,"","",&pic_image8);
    image9 = new customState(game,"Image 9",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image9);
    image10 = new customState(game,"Image 10",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image10);
    image11 = new customState(game,"Image 11",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","",&pic_image11);
    image11_2 = new customState(game,"Image 11_2",&gameSates,&stateBeforeError,(QState*)errorh,"SON_11.mp3","",NULL);
    anagramme1 = new customState(game,"Anagramme 1",&gameSates,&stateBeforeError,(QState*)errorh,"","",NULL);
    anagramme2 = new customState(game,"Anagramme 2",&gameSates,&stateBeforeError,(QState*)errorh,"","",NULL);
    anagramme12 = new customState(game,"Anagramme 1&2",&gameSates,&stateBeforeError,(QState*)errorh,"","",&pic_image11);
    video5 = new customState(game,"Video 5",&gameSates,&stateBeforeError,(QState*)errorh,"SON_17.mp3","VIDEO_5.mp4",NULL);
    video6 = new customState(game,"Video 6",&gameSates,&stateBeforeError,(QState*)errorh,"","VIDEO_6.mp4",NULL);
    final = new customState(game,"Video 6",&gameSates,&stateBeforeError,(QState*)errorh,"","",&pic_image14);



    error0 = new customState(error,"Erreur0",&errorStates,NULL,(QState*)errorh,"","",NULL);


    error3 = new customState(error,"Erreur3",&errorStates,NULL,(QState*)errorh,"","",NULL);
    error2 = new customState(error,"Erreur2",&errorStates,NULL,error3,"SON_14.mp3","",&pic_image13);
    error1 = new customState(error,"Erreur1",&errorStates,NULL,error2,"SON_13.mp3","",&pic_image12);


    error->setInitialState(error1);
    error0->addTransition(error,SIGNAL(entered()),error1);
    error1->addTransition(inputReader->getButtonByName("bouton9"),SIGNAL(triggered()),gameh);
    setStateWhiteList(error1,"bouton9","");
    error1->addTransition(error,SIGNAL(entered()),error2);
    error2->addTransition(inputReader->getButtonByName("bouton10"),SIGNAL(triggered()),gameh);
    setStateWhiteList(error2,"bouton10","");
    error2->addTransition(error,SIGNAL(entered()),error3);
    error3->addTransition(error0);
    error->addTransition(error3,SIGNAL(exited()),video6);
    errorh->setDefaultState(error1);




    game->setInitialState(game_idle);



    //joystick subroutine




    customState *joy_idle = new customState(NULL,"Joystick idle",&joyStates);
    customState *joy_start = new customState(NULL,"Joystick start",&joyStates);
    customState *joy_H = new customState(NULL,"Joystick H",&joyStates,NULL,NULL,"SON_17.mp3");
    customState *joy_HD = new customState(NULL,"Joystick H D",&joyStates,NULL,NULL,"SON_17.mp3");
    customState *joy_HDG = new customState(NULL,"Joystick H D G",&joyStates,NULL,NULL,"SON_17.mp3");
    customState *joy_HDGB = new customState(NULL,"Joystick H D G B",&joyStates,NULL,NULL,"SON_17.mp3");
    customState *joy_HDGB11 = new customState(NULL,"Joystick Done",&joyStates,NULL,NULL,"SON_17.mp3");


    connect(keyboard,SIGNAL(keyStop()),&gameMachine,SLOT(start()));



    connect(game_idle,SIGNAL(entered()),this,SLOT(stopSoundLoop()));
    connect(game_idle,SIGNAL(entered()),this,SLOT(stopSound()));
    connect(game_idle,SIGNAL(entered()),this,SLOT(stopVideo()));
    connect(game_idle,SIGNAL(entered()),st,SLOT(stop()));
    connect(game_idle,SIGNAL(entered()),this,SLOT(initRelayStates()));
    connect(game_idle,SIGNAL(entered()),imgLbl,SLOT(hide()));
    connect(game_idle,SIGNAL(entered()),this,SLOT(initLedStates()));
    connect(game_idle,SIGNAL(entered()),this,SLOT(start78910Led()));
    connect(game_idle,SIGNAL(entered()),this,SLOT(initVoyantStates()));
    game_idle->addTransition(keyboard,SIGNAL(keyStart()),video1);




    connect(video1,SIGNAL(entered()),this,SLOT(initGame()));
    connect(video1,SIGNAL(entered()),imgLbl,SLOT(hide()));
    connect(video1,SIGNAL(entered()),this,SLOT(stopSoundLoop()));
    connect(video1,SIGNAL(entered()),st,SLOT(stop()));
    connect(video1,SIGNAL(entered()),this,SLOT(initRelayStates()));
    connect(video1,SIGNAL(entered()),this,SLOT(initLedStates()));
    connect(video1,SIGNAL(entered()),this,SLOT(start78910Led()));
    connect(video1,SIGNAL(entered()),this,SLOT(initVoyantStates()));
    video1->addTransition(this,SIGNAL(videoFinished()),image1);




    connect(image1,SIGNAL(entered()),st,SLOT(start()));
    connect(image1,SIGNAL(entered()),this,SLOT(playSoundLoop()));
    connect(image1,SIGNAL(exited()),imgLbl,SLOT(hide()));
    connect(image1,SIGNAL(exited()),this,SLOT(stopSound()));
    image1->addTransition(inputReader->getButtonByName("bouton1"),SIGNAL(triggered()),image2);
    setStateWhiteList(image1,"bouton1","");


    /*
     *
     * Joystick subroutine
     *
    */




    connect(joy_idle,SIGNAL(entered()),this,SLOT(joyStopAlertSound()));
    connect(joy_idle,SIGNAL(entered()),this,SLOT(stopVoyants()));
    connect(joy_idle,SIGNAL(entered()),joyTimer,SLOT(stop()));

    connect(joy_HDGB11,SIGNAL(entered()),this,SLOT(joyStopAlertSound()));
    connect(joy_HDGB11,SIGNAL(entered()),this,SLOT(stopVoyants()));
    connect(joy_HDGB11,SIGNAL(entered()),joyTimer,SLOT(stop()));


    connect(joy_start,SIGNAL(entered()),this,SLOT(startVoyants()));
    connect(this,SIGNAL(voyantsOver()),joyTimer,SLOT(start()));
    connect(this,SIGNAL(voyantsOver()),this,SLOT(joyPlayAlertSound()));
    joy_start->addTransition(inputReader->getJoyInputByName("joystick_haut"),SIGNAL(triggered()),joy_H);

    joy_H->addTransition(inputReader->getJoyInputByName("joystick_droite"),SIGNAL(triggered()),joy_HD);
    joy_H->addTransition(inputReader->getJoyInputByName("joystick_haut"),SIGNAL(triggered()),joy_start);
    joy_H->addTransition(inputReader->getJoyInputByName("joystick_gauche"),SIGNAL(triggered()),joy_start);
    joy_H->addTransition(inputReader->getJoyInputByName("joystick_bas"),SIGNAL(triggered()),joy_start);
    joy_H->addTransition(inputReader->getJoyInputByName("bouton11"),SIGNAL(triggered()),joy_start);

    joy_HD->addTransition(inputReader->getJoyInputByName("joystick_gauche"),SIGNAL(triggered()),joy_HDG);
    joy_HD->addTransition(inputReader->getJoyInputByName("joystick_haut"),SIGNAL(triggered()),joy_start);
    joy_HD->addTransition(inputReader->getJoyInputByName("joystick_droite"),SIGNAL(triggered()),joy_start);
    joy_HD->addTransition(inputReader->getJoyInputByName("joystick_bas"),SIGNAL(triggered()),joy_start);
    joy_HD->addTransition(inputReader->getJoyInputByName("bouton11"),SIGNAL(triggered()),joy_start);

    joy_HDG->addTransition(inputReader->getJoyInputByName("joystick_bas"),SIGNAL(triggered()),joy_HDGB);
    joy_HDG->addTransition(inputReader->getJoyInputByName("joystick_haut"),SIGNAL(triggered()),joy_start);
    joy_HDG->addTransition(inputReader->getJoyInputByName("joystick_droite"),SIGNAL(triggered()),joy_start);
    joy_HDG->addTransition(inputReader->getJoyInputByName("joystick_gauche"),SIGNAL(triggered()),joy_start);
    joy_HDG->addTransition(inputReader->getJoyInputByName("bouton11"),SIGNAL(triggered()),joy_start);

    joy_HDGB->addTransition(inputReader->getJoyInputByName("joystick_bas"),SIGNAL(triggered()),joy_start);
    joy_HDGB->addTransition(inputReader->getJoyInputByName("joystick_haut"),SIGNAL(triggered()),joy_start);
    joy_HDGB->addTransition(inputReader->getJoyInputByName("joystick_droite"),SIGNAL(triggered()),joy_start);
    joy_HDGB->addTransition(inputReader->getJoyInputByName("joystick_gauche"),SIGNAL(triggered()),joy_start);
    joy_HDGB->addTransition(inputReader->getJoyInputByName("bouton11"),SIGNAL(triggered()),joy_HDGB11);




    image2->addTransition(inputReader->getButtonByName("bouton2"),SIGNAL(triggered()),image2_2);
    setStateWhiteList(image2,"bouton2","");



    image2_2->addTransition(inputReader->getButtonByName("bouton3"),SIGNAL(triggered()),image2_3);
    setStateWhiteList(image2_2,"bouton3","");



    image2_3->addTransition(inputReader->getButtonByName("bouton4"),SIGNAL(triggered()),image2_4);
    setStateWhiteList(image2_3,"bouton4","");


    connect(image2_4,SIGNAL(entered()),this,SLOT(closeElectroAimant()));
    image2_4->addTransition(inputReader->getButtonByName("bouton5"),SIGNAL(triggered()),image2_5);
    setStateWhiteList(image2_4,"bouton5","");


    connect(image2_5,SIGNAL(entered()),this,SLOT(openElectroAimant()));
    connect(image2_5,SIGNAL(entered()),this,SLOT(closeSolenoide1()));
    connect(image2_5,SIGNAL(entered()),this,SLOT(stopVideo()));
    image2_5->addTransition(inputReader->getButtonByName("bouton6"),SIGNAL(triggered()),video2);
    setStateWhiteList(image2_5,"bouton6","");



    connect(video2,SIGNAL(entered()),this,SLOT(openSolenoide1()));
    connect(video2,SIGNAL(entered()),this,SLOT(closeSolenoide2()));
    connect(video2,SIGNAL(entered()),this,SLOT(turnOnGyro()));
    video2->addTransition(inputReader->getButtonByName("bouton7"),SIGNAL(triggered()),video2_2);
    video2->addTransition(this,SIGNAL(videoFinished()),video6);//échec si fin vidéo
    setStateWhiteList(video2,"bouton7","");




    connect(video2_2,SIGNAL(entered()),this,SLOT(openSolenoide2()));
    video2_2->addTransition(inputReader->getButtonByName("bouton8"),SIGNAL(triggered()),video2_3);
    video2_2->addTransition(this,SIGNAL(videoFinished()),video6);//échec si fin vidéo
    setStateWhiteList(video2_2,"bouton8","");


    connect(image3,SIGNAL(entered()),this,SLOT(turnOffGyro()));
    video2_3->addTransition(inputReader->getWireByName("cable_ok"),SIGNAL(disconnected()),image3);
    video2_3->addTransition(this,SIGNAL(videoFinished()),video6);//échec si fin vidéo
    setStateWhiteList(video2_3,"","cable_ok");


    image3->addTransition(rfid,SIGNAL(disquette1()),video3);
    setStateWhiteList(image3,"","",1);


    connect(video3,SIGNAL(entered()),this,SLOT(turnOnGyro()));
    video3->addTransition(this,SIGNAL(videoFinished()),image4);
    setStateWhiteList(video3,"","");


    image4->addTransition(this,SIGNAL(answerC()),image5);
    image4->addErrorSignal(this,SIGNAL(answerA()));
    image4->addErrorSignal(this,SIGNAL(answerB()));
    image4->addErrorSignal(this,SIGNAL(answerC()));
    setStateWhiteList(image4,"","");

    image5->addTransition(this,SIGNAL(answerA()),image6);
    image5->addErrorSignal(this,SIGNAL(answerB()));
    image5->addErrorSignal(this,SIGNAL(answerC()));
    image5->addErrorSignal(this,SIGNAL(answerD()));
    setStateWhiteList(image5,"","");

    image6->addTransition(this,SIGNAL(answerC()),image7);
    image6->addErrorSignal(this,SIGNAL(answerA()));
    image6->addErrorSignal(this,SIGNAL(answerB()));
    image6->addErrorSignal(this,SIGNAL(answerD()));
    setStateWhiteList(image6,"","");

    image7->addTransition(this,SIGNAL(answerC()),video4);
    image7->addErrorSignal(this,SIGNAL(answerA()));
    image7->addErrorSignal(this,SIGNAL(answerB()));
    image7->addErrorSignal(this,SIGNAL(answerD()));
    setStateWhiteList(image7,"","");


    connect(video4,SIGNAL(entered()),this,SLOT(turnOffGyro()));
    video4->addTransition(this,SIGNAL(videoFinished()),image8);
    setStateWhiteList(video4,"","");

    image8->addTransition(rfid,SIGNAL(disquette2()),image9);
    setStateWhiteList(image8,"","",2);

    image9->addTransition(rfid,SIGNAL(disquette3()),image10);
    setStateWhiteList(image9,"","",3);

    image10->addTransition(rfid,SIGNAL(disquette4()),image11);
    setStateWhiteList(image10,"","",4);

    connect(image11,SIGNAL(entered()),this,SLOT(turnOnLed1()));
    connect(image11,SIGNAL(entered()),this,SLOT(turnOffLed2()));
    connect(image11,SIGNAL(entered()),this,SLOT(closeSolenoide4()));
    connect(image11,SIGNAL(entered()),this,SLOT(turnOffDigicode()));
    connect(image11,SIGNAL(entered()),t5Timer,SLOT(start()));
    image11->addTransition(t5Timer,SIGNAL(timeout()),image11_2);
    setStateWhiteList(image11,"","");

    connect(image11_2,SIGNAL(entered()),this,SLOT(openSolenoide4()));
    connect(image11_2,SIGNAL(entered()),this,SLOT(turnOffLed1()));
    connect(image11_2,SIGNAL(entered()),this,SLOT(turnOnLed2()));
    connect(image11_2,SIGNAL(entered()),this,SLOT(turnOnDigicode()));
    image11_2->addTransition(inputReader->getButtonByName("anagramme1"),SIGNAL(triggered()),anagramme1);
    image11_2->addTransition(inputReader->getButtonByName("anagramme2"),SIGNAL(triggered()),anagramme2);
    connect(image11_2,SIGNAL(entered()),this,SLOT(turnOffLed3()));
    connect(image11_2,SIGNAL(entered()),this,SLOT(turnOffLed4()));
    setStateWhiteLists(image11_2,{"anagramme1","anagramme2"},"");

    anagramme1->addTransition(inputReader->getButtonByName("anagramme2"),SIGNAL(triggered()),anagramme12);
    connect(anagramme1,SIGNAL(entered()),this,SLOT(turnOnLed3()));
    connect(anagramme1,SIGNAL(entered()),this,SLOT(turnOffLed4()));
    connect(anagramme1,SIGNAL(entered()),this,SLOT(turnOnLed5()));
    connect(anagramme1,SIGNAL(entered()),this,SLOT(turnOffLed6()));
    setStateWhiteList(anagramme1,"anagramme2","");
    anagramme2->addTransition(inputReader->getButtonByName("anagramme1"),SIGNAL(triggered()),anagramme12);
    connect(anagramme2,SIGNAL(entered()),this,SLOT(turnOnLed4()));
    connect(anagramme2,SIGNAL(entered()),this,SLOT(turnOffLed3()));
    connect(anagramme2,SIGNAL(entered()),this,SLOT(turnOnLed5()));
    connect(anagramme2,SIGNAL(entered()),this,SLOT(turnOffLed6()));
    setStateWhiteList(anagramme2,"anagramme1","");


    connect(anagramme12,SIGNAL(entered()),this,SLOT(playSoundLoop()));
    connect(anagramme12,SIGNAL(entered()),this,SLOT(turnOnVentil()));
    connect(anagramme12,SIGNAL(entered()),this,SLOT(turnOnLed3()));
    connect(anagramme12,SIGNAL(entered()),this,SLOT(turnOnLed4()));
    connect(anagramme12,SIGNAL(entered()),this,SLOT(turnOffLed5()));
    connect(anagramme12,SIGNAL(entered()),this,SLOT(turnOnLed6()));

    anagramme12->addTransition(inputReader->getButtonByName("interrupteur1"),SIGNAL(triggered()),video5);
    setStateWhiteList(anagramme12,"interrupteur1","");
    connect(anagramme12,SIGNAL(entered()),this,SLOT(start78910Led()));

    //Fin échec
    connect(video6,SIGNAL(entered()),this,SLOT(stopSound()));
    connect(video6,SIGNAL(entered()),this,SLOT(stopSoundLoop()));
    connect(video6,SIGNAL(entered()),this,SLOT(stopVoyants()));
    connect(video6,SIGNAL(entered()),joyTimer,SLOT(stop()));
    connect(video6,SIGNAL(entered()),st,SLOT(pause()));
    video6->addTransition(this,SIGNAL(videoFinished()),final);


    //Fin victoire
    connect(video5,SIGNAL(entered()),this,SLOT(stopSoundLoop()));
    connect(video5,SIGNAL(entered()),this,SLOT(turnOffVentil()));
    connect(video5,SIGNAL(entered()),st,SLOT(pause()));
    connect(video5,SIGNAL(entered()),this,SLOT(stopVoyants()));
    connect(video5,SIGNAL(entered()),joyTimer,SLOT(stop()));
    connect(video5,SIGNAL(entered()),this,SLOT(stop78910Led()));
    video5->addTransition(this,SIGNAL(videoFinished()),final);

    //dernière image


    connect(error,SIGNAL(entered()),this,SLOT(gotError()));
    connect(error,SIGNAL(exited()),this,SLOT(turnOffGyro()));
    connect(error,SIGNAL(entered()),t5Timer,SLOT(stop()));


    for(int i = 0;i<gameSates.size();i++)
    {
        if(i>0)
            if((gameSates[i]!=video6))
                gameSates[i]->addTransition(keyboard,SIGNAL(keyPrev()),gameSates[i-1]);//previous state
        if(i<gameSates.size()-1)
            if((gameSates[i+1]!=video6))
                gameSates[i]->addTransition(keyboard,SIGNAL(keyNext()),gameSates[i+1]);//next state

        gameSates[i]->addTransition(keyboard,SIGNAL(keyStart()),video1);
        gameSates[i]->addTransition(keyboard,SIGNAL(keyStop()),game_idle);
        gameSates[i]->addTransition(st,SIGNAL(finished()),video6);//défaite sur fin de décompte
        gameSates[i]->addTransition(joyTimer,SIGNAL(timeout()),video6);//défaite sur fin de décompte joystick


        connect(gameSates[i],SIGNAL(stopSound()),this,SLOT(stopSound()));
        connect(gameSates[i],SIGNAL(playVideo(QString)),this,SLOT(playVideo(QString)));
        connect(gameSates[i],SIGNAL(playSound(QString)),this,SLOT(playSound(QString)));
        connect(gameSates[i],SIGNAL(setPicture(QImage *)),this,SLOT(setPicture(QImage *)));
    }


    for(auto s:errorStates)
    {
        connect(s,SIGNAL(stopSound()),this,SLOT(stopSound()));
        connect(s,SIGNAL(playVideo(QString)),this,SLOT(playVideo(QString)));
        connect(s,SIGNAL(playSound(QString)),this,SLOT(playSound(QString)));
        connect(s,SIGNAL(setPicture(QImage *)),this,SLOT(setPicture(QImage *)));
    }

    gameMachine.setInitialState(game);

    for(auto s:joyStates)
    {
        connect(s,SIGNAL(stopSound()),this,SLOT(stopJoySound()));
        connect(s,SIGNAL(playSound(QString)),this,SLOT(playJoySound(QString)));
        s->addTransition(image1,SIGNAL(entered()),joy_start);
        s->addTransition(game_idle,SIGNAL(entered()),joy_idle);
        joystickMachine.addState(s);
    }
    joystickMachine.setInitialState(joy_idle);
    joystickMachine.start();
    gameMachine.start();


}






void MainWindow::startVoyants(void)
{
    voyantsTimer->start();
    voyantIndex = 0;
    initVoyantStates();
    qDebug()<<"voyant started";
}



void MainWindow::stopVoyants(void)
{
    voyantsTimer->stop();
    initVoyantStates();
    qDebug()<<"voyant stopped";
}


void MainWindow::topVoyants()
{
    voyantIndex++;
    turnVoyantOn(voyantIndex);
    qDebug()<<"top voyant "<<voyantIndex;
    if(voyantIndex>=5)
    {
        voyantsTimer->stop();
        emit voyantsOver();
    }
}



void MainWindow::setStateWhiteList(customState *state, QString button,QString wire,uint disquette)
{
    for(auto in:inputReader->getButtonByNotName(button)) state->addErrorSignal(in,SIGNAL(triggered()));
    for(auto in:inputReader->getWireByNotName(wire)) state->addErrorSignal(in,SIGNAL(disconnected()));
    if(disquette!=1) state->addErrorSignal(rfid,SIGNAL(disquette1()));
    if(disquette!=2) state->addErrorSignal(rfid,SIGNAL(disquette2()));
    if(disquette!=3) state->addErrorSignal(rfid,SIGNAL(disquette3()));
    if(disquette!=4) state->addErrorSignal(rfid,SIGNAL(disquette4()));
    state->addErrorSignal(rfid,SIGNAL(disquette5()));
    state->addErrorSignal(rfid,SIGNAL(disquetteError()));
}


void MainWindow::setStateWhiteLists(customState *state, QStringList buttons,QString wire,uint disquette)
{
    for(auto button:buttons)
        for(auto in:inputReader->getButtonByNotName(button))
            state->addErrorSignal(in,SIGNAL(triggered()));


    for(auto in:inputReader->getWireByNotName(wire)) state->addErrorSignal(in,SIGNAL(disconnected()));
    if(disquette!=1) state->addErrorSignal(rfid,SIGNAL(disquette1()));
    if(disquette!=2) state->addErrorSignal(rfid,SIGNAL(disquette2()));
    if(disquette!=3) state->addErrorSignal(rfid,SIGNAL(disquette3()));
    if(disquette!=4) state->addErrorSignal(rfid,SIGNAL(disquette4()));
    state->addErrorSignal(rfid,SIGNAL(disquette5()));
    state->addErrorSignal(rfid,SIGNAL(disquetteError()));
}



void MainWindow::gotError()
{
    qDebug()<<"got error #"<<errorCount;
    qDebug()<<"from state: "<<stateBeforeError->getName();
    //error->addTransition(inputReader->getButtonByName("bouton9"),SIGNAL(triggered()),gameh);

    return;
    if(errorCount == 0)
    {
        for(auto t:error->transitions())
            error->removeTransition(t);
        errorCount=1;
        imgLbl->setPixmap(QPixmap::fromImage(pic_image12).scaledToWidth(width()));
        imgLbl->show();
        playSound("SON_13.mp3");
        error->addTransition(inputReader->getButtonByName("bouton9"),SIGNAL(triggered()),gameh);
        for(auto in:inputReader->getButtonByNotName("bouton9"))
            connect(in,SIGNAL(triggered()),this,SLOT(gotError()));
        turnOnGyro();
    }
    else if(errorCount == 1)
    {
        for(auto t:error->transitions())
            error->removeTransition(t);
        errorCount=2;
        imgLbl->setPixmap(QPixmap::fromImage(pic_image13).scaledToWidth(width()));
        imgLbl->show();
        playSound("SON_14.mp3");
        error->addTransition(inputReader->getButtonByName("bouton10"),SIGNAL(triggered()),gameh);
        turnOnGyro();

        for(auto in:inputReader->getButtonByNotName("bouton9"))
            connect(in,SIGNAL(triggered()),this,SLOT(gotError()));

        error->addTransition(error,SIGNAL(entered()),video6);//next time we lose
    }
    /* else
    {
        error->addTransition(this,SIGNAL(forceGo()),video6);//next time we lose
        emit forceGo();
    }*/



}

void MainWindow::errorFilter(void)
{
    qDebug()<<"error detected";
    qDebug()<<"state is: "<<((customState*)(gameMachine.configuration().toList()[0]))->getName();


}

void MainWindow::initGame()
{
    errorCount = 0;
}




void MainWindow::setPicture(QImage *pic)
{
    imgLbl->setPixmap(QPixmap::fromImage(*pic).scaledToWidth(width()));
    imgLbl->show();
    stopVideo();
}




void MainWindow::updateLed7State()
{
    if((qrand() % 4)==0)
    {
        led7State=!led7State;
        gpioWrite(12, led7State);
    }
}

void MainWindow::updateLed8State()
{
    if((qrand() % 4)==0)
    {
        led8State=!led8State;
        gpioWrite(13, led8State);
    }
}

void MainWindow::updateLed9State()
{
    if((qrand() % 4)==0)
    {
        led9State=!led9State;
        gpioWrite(4, led9State);
    }
}

void MainWindow::updateLed10State()
{
    if((qrand() % 4)==0)
    {
        led10State=!led10State;
        gpioWrite(5, led10State);
    }
}


void MainWindow::start78910Led(void)
{
    timerLed7->start();
    timerLed8->start();
    timerLed9->start();
    timerLed10->start();
}


void MainWindow::stop78910Led(void)
{
    timerLed7->stop();
    timerLed8->stop();
    timerLed9->stop();
    timerLed10->stop();
    turnLedOff(6);
    turnLedOff(7);
    turnLedOff(8);
    turnLedOff(9);
}


void MainWindow::mousePressEvent(QMouseEvent *eventPress)
{

    // qDebug()<<eventPress->pos();
    uint x = eventPress->pos().x();
    uint y = eventPress->pos().y();

    if((x>105)&&(x<382)&&(y>210)&&(y<314))
        emit answerA();
    else if((x>428)&&(x<708)&&(y>206)&&(y<306))
        emit answerB();
    else if((x>114)&&(x<377)&&(y>344)&&(y<444))
        emit answerC();
    else if((x>424)&&(x<708)&&(y>351)&&(y<441))
        emit answerD();

}















MainWindow::~MainWindow()
{


}


