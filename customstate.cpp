#include "customstate.h"

customState::customState(QState *parent, QString name, std::vector<customState *> *liste,
                         customState **lastState, QState *errorState, QString sound, QString video, QImage *pic)
    :QState(parent),name(name),lastState(lastState),errorState(errorState),sound(sound),video(video),pic(pic)
{    
    if(liste!=NULL)
        liste->push_back(this);
    connect(this,SIGNAL(entered()),this,SLOT(didEnter()));

}

const QString &customState::getName() const
{
    return name;
}



void customState::addErrorSignal(QObject* parent,const char* signal)
{

    if(errorState!=NULL)
        this->addTransition(parent,signal,errorState);
    else
        qDebug()<<"state "<<name<<": no error state defined";
}

void customState::didEnter()
{
    if(lastState!=NULL)
        (*lastState)=this;
    qDebug()<<"State is now: "<<name;

    emit stopSound();
    if(pic)
        emit setPicture(pic);
    if(sound!="")
        emit playSound(sound);
    if(video!="")
        emit playVideo(video);


}
