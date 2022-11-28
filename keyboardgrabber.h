#ifndef KEYBOARDGRABBER_H
#define KEYBOARDGRABBER_H

#include <QObject>
#include <qthread.h>
class keyGrabber : public QObject
{
    Q_OBJECT
public:
    explicit keyGrabber(QObject *parent = nullptr);

signals:
    void gotKey(uint key);
    void keyStart(void);
    void keyNext(void);
    void keyPrev(void);
    void keyStop(void);
private slots:
    void readKeys(void);

};

#endif // KEYBOARDGRABBER_H
