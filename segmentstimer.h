#ifndef SEGMENTSTIMER_H
#define SEGMENTSTIMER_H

#include <QObject>
#include <qtimer.h>
#include <qprocess.h>

class segmentsTimer : public QObject
{
    Q_OBJECT
public:
    explicit segmentsTimer(QObject *parent = nullptr, uint lengthS=10);


private :
    QTimer *countTimer;
    uint count;
    void displayTimer(uint time);
    uint lengthS;

signals:
    void finished(void);
private slots:
    void top(void);
    void stop(void);
    void pause(void);
    void unpause(void);
    void start();
};

#endif // SEGMENTSTIMER_H
