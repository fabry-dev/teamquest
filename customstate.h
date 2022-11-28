#ifndef CUSTOMSTATE_H
#define CUSTOMSTATE_H

#include <QState>
#include <QObject>
#include "qdebug.h"
#include "qstatemachine.h"
#include "qhistorystate.h"
class customState : public QState
{
    Q_OBJECT
public:
    customState(QState *parent=NULL, QString name="", std::vector<customState*> *liste=NULL, customState **lastState=NULL, QState *errorState=NULL,
                QString sound="", QString video="", QImage *pic=NULL);

    const QString &getName() const;
    void addErrorSignal(QObject* parent, const char *signal);
private:
    QString name;
    customState *nextState;
    customState **lastState;
    QState *errorState;
    QString sound,video;
    QImage *pic;

private slots:
    void didEnter();

signals:
    void stopSound();
    void playSound(QString);
    void playVideo(QString);
    void errorDetected(void);
    void setPicture(QImage*);
};

#endif // CUSTOMSTATE_H
