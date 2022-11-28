#ifndef RFIDREADER_H
#define RFIDREADER_H



#include <qobject.h>
#include <QTimer>
#include <qthread.h>
#include <qdebug.h>


class rfidReader:public QObject
{
    Q_OBJECT
public:
    rfidReader();
private:
    std::vector<uint> rfidGpios;
    std::vector<QString> rfidCodes;
    std::vector<uint> rfidVector;
    void cardOK(uint i);
private slots:
    void readProcess(void);
signals:
    void rfidChanged(std::vector<uint> vector);
    void disquette1(void);
    void disquette2(void);
    void disquette3(void);
    void disquette4(void);
    void disquette5(void);
    void disquetteError(void);
};
#endif // RFIDREADER_H
