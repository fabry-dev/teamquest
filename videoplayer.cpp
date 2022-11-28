#include "mainwindow.h"
#include "qeventloop.h"




void MainWindow::sendIpc(QString msg,QString server)
{
    QLocalSocket *socket = new QLocalSocket(this);
    socket->connectToServer(server);
    if (!socket->waitForConnected()) {
        qDebug() << "could not connect to MPV server: " << socket->errorString();
        socket->close();
        socket->deleteLater();
        return;
    }

    QByteArray inUtf8 = msg.toUtf8();
    const char *data = inUtf8.constData();
    socket->readAll();
    socket->write(data);
    socket->waitForBytesWritten(1000);
    socket->waitForReadyRead(3000);
    //qDebug()<<"sent to mpv: "<<msg;
    //qDebug() <<"mpv response:"<< socket->readAll();
    socket->close();
    socket->deleteLater();
}




void MainWindow::createSoundPlayer()
{
    soundProcess.start("mpv", QStringList() <<"--quiet"
                       <<"--no-input-default-bindings"
                       <<"--keep-open"<<"yes"
                       <<"--input-ipc-server=/tmp/mpvsocket1"
                       <<"--idle");
}
void MainWindow::playSound(QString filename)
{
    QString server = "/tmp/mpvsocket1";
    QString data;
    data = "{\"command\":[\"loadfile\",\"/home/pi/multimedia/"+filename+"\"]}\n";
    sendIpc(data,server);
    QThread::msleep(200);
    data = "{\"command\":[\"set_property\", \"pause\", false]}\n";
    sendIpc(data,server);
}
void MainWindow::stopSound(void)
{
    QString server = "/tmp/mpvsocket1";
    QString data = "{ \"command\": [\"stop\"] }\n";
    sendIpc(data,server);
    //soundProcess.close();
    qDebug()<<"stop sound";
}



void MainWindow::createJoySoundPlayer()
{
    joySoundProcess.start("mpv", QStringList() <<"--quiet"
                          <<"--no-input-default-bindings"
                          <<"--keep-open"<<"yes"
                          <<"--input-ipc-server=/tmp/mpvsocket3"
                          <<"--idle");
}
void MainWindow::playJoySound(QString filename)
{
    QString server = "/tmp/mpvsocket3";
    QString data;
    data = "{\"command\":[\"loadfile\",\"/home/pi/multimedia/"+filename+"\"]}\n";
    sendIpc(data,server);
    QThread::msleep(200);
    data = "{\"command\":[\"set_property\", \"pause\", false]}\n";
    sendIpc(data,server);
}
void MainWindow::stopJoySound(void)
{
    QString server = "/tmp/mpvsocket3";
    QString data = "{ \"command\": [\"stop\"] }\n";
    sendIpc(data,server);
    //soundProcess.close();
    qDebug()<<"stop joy sound";
}










void MainWindow::createVideoPlayer()
{
    videoProcess.start("mpv", QStringList() <<"--quiet"
                       <<"--no-input-default-bindings"
                       <<"--force-window=yes"
                       <<"--keep-open=yes"
                       <<"--input-ipc-server=/tmp/mpvsocket1"
                       <<"--idle");
}









void MainWindow::playVideo(QString filename)
{

    filename = "/home/pi/multimedia/"+filename;
    connect(&videoProcess,SIGNAL(finished(int)),this,SIGNAL(videoFinished()));
    connect(&videoProcess,SIGNAL(started()),this,SIGNAL(videoStarted()));

    if(videoProcess.state()==QProcess::Running)
    {
        qDebug()<<"loading new video file";
        //videoProcess.close();
        QString server = "/tmp/mpvsocket2";
        QString data;
        data = "{\"command\":[\"loadfile\",\""+filename+"\"]}\n";
        sendIpc(data,server);
        QThread::msleep(200);
        data = "{\"command\":[\"set_property\", \"pause\", false]}\n";
        sendIpc(data,server);
    }
    else
    {
        qDebug()<<"creating mpv video player";
        videoProcess.start("mpv", QStringList() <<"--cursor-autohide=always"<<"--no-osc"<<"--no-input-default-bindings"<<"--input-ipc-server=/tmp/mpvsocket2"<< filename );
    }
    imgLbl->hide();
}
void MainWindow::stopVideo(void)
{
    if(videoProcess.state()!=QProcess::NotRunning)
    {
        videoProcess.close();
        qDebug()<<"stopping video";
    }

    imgLbl->show();
}




void MainWindow::playSoundLoop()
{
    if(soundLoopProcess.state() == QProcess::Running)
        return;

    QString filename = "/home/pi/multimedia/SON_1.mp3";
    uint volume = 75;
    soundLoopProcess.start("mpv", QStringList() << filename <<"--quiet"<<(QString)"--volume="+QString::number(volume)<<"--loop=inf");
}


void MainWindow::stopSoundLoop()
{
    if(soundLoopProcess.state()!=QProcess::NotRunning)
    {
        soundLoopProcess.close();
        qDebug()<<"stopping loop sound";
    }
}






void MainWindow::joyPlayAlertSound()
{
    qDebug()<<"joy play alert sound";
    if(joyAlertProcess.state() == QProcess::Running)
        return;

    QString filename = "/home/pi/multimedia/SON_15.mp3";
    uint volume = 75;
    joyAlertProcess.start("mpv", QStringList() << filename <<"--quiet"<<(QString)"--volume="+QString::number(volume));

}



void MainWindow::joyStopAlertSound()
{
    if(joyAlertProcess.state()!=QProcess::NotRunning)
    {
        joyAlertProcess.close();
        qDebug()<<"stopping alert joy sound";
    }
}



