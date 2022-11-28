#include "segmentstimer.h"

segmentsTimer::segmentsTimer(QObject *parent,uint lengthS)
    : QObject{parent},lengthS(lengthS)
{
    countTimer = new QTimer(this);
    connect(countTimer,SIGNAL(timeout()),this,SLOT(top()));
}


void segmentsTimer::start()
{
    count = lengthS;
    displayTimer(count);
    countTimer->start(1000);
}

void segmentsTimer::stop(void)
{
    count = 0;
    QProcess process2;
    process2.startDetached("python3", QStringList() << "setClock.py"<<"");
    countTimer->stop();

}

void segmentsTimer::pause(void)
{

    countTimer->stop();

}

void segmentsTimer::unpause()
{
    if(!countTimer->isActive())
      countTimer->start();
}


void segmentsTimer::displayTimer(uint time)
{

    uint minutes = time/60;
    uint seconds = time%60;
    QProcess process2;
    process2.startDetached("python3", QStringList() << "setClock.py"<<QString::number(minutes).rightJustified(2, '0')+":"+QString::number(seconds).rightJustified(2, '0'));

}


void segmentsTimer::top()
{
    if(count>0)
    {
        count--;
        displayTimer(count);
    }
    if(count == 0)
    {
        countTimer->stop();
        emit finished();
    }


}
