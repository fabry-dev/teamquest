#ifndef MUXREADER_H
#define MUXREADER_H

#include <qobject.h>
#include <QTimer>
#include <qthread.h>
#include <qdebug.h>

class input_c:public QObject
{

        Q_OBJECT
public :
    input_c(QString name,uint mux,uint index):name(name),mux(mux),index(index){state = false;}
public:
    QString name;
    uint mux;
    uint index;
    void setState(bool nuState);
private:
    bool state;
signals:
    void triggered(void);
    void disconnected(void);
    void changed(void);


};


class muxReader:public QObject
{
    Q_OBJECT
public:
    muxReader();
    input_c* getButtonByName(QString name);
    input_c* getJoyInputByName(QString name);
    input_c* getWireByName(QString name);

    std::vector<input_c*> getButtonByNotName(QString name);
    std::vector<input_c*> getWireByNotName(QString name);
    void registerButtonInput(QString name,uint mux,uint index);
    void registerJoyInput(QString name,uint mux,uint index);
    void registerWireInput(QString name,uint mux,uint index);

private:
    std::vector<input_c*> buttonInputs,joyInputs,wireInputs;
    uint muxIndex;
    std::vector<std::vector<bool>> muxValues;
private slots:
    void readProcess(void);
};

#endif // MUXREADER_H
