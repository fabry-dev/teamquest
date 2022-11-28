#include "mainwindow.h"
#include "pigpio.h"



void MainWindow::initMCUSerial()
{
    relaysStates.resize(8,false);
    ledsStates.resize(8,false);
    voyantsStates.resize(8,false);
    MCUPort = new QSerialPort(this);
    MCUPort->setPortName("/dev/ttyS0");
    MCUPort->setBaudRate(QSerialPort::Baud9600);
    MCUPort->setDataBits(QSerialPort::Data8);
    MCUPort->setParity(QSerialPort::NoParity);
    MCUPort->setStopBits(QSerialPort::OneStop);
    connect(MCUPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(handleSerialError(QSerialPort::SerialPortError)));

    MCUConnect();


    gpioSetMode(12, PI_OUTPUT);
    gpioSetMode(13, PI_OUTPUT);
    gpioSetMode(4, PI_OUTPUT);
    gpioSetMode(5, PI_OUTPUT);


    turnAllRelaysOff();
    turnAllLedsOff();

}

void MainWindow::MCUConnect(void)
{
    MCUPort->close();
    if(MCUPort->open(QIODevice::ReadWrite))
        qDebug()<<"serial connected";
    else
        qDebug()<<"serial error";

}




void MainWindow::handleSerialError(QSerialPort::SerialPortError errorCode)
{
    if(errorCode == QSerialPort::NoError)
        return;
    qDebug()<<">>>>>>>>>>>>>>>serial port error : "<<errorCode;
    qDebug()<<">>>>>>>>>>>>>>>reconnecting in 3 seconds";

    QTimer::singleShot(3000,this,SLOT(MCUConnect()));

}




void MainWindow::sendToMCU(uint8_t code,uint8_t value)
{
    char data[4];
    data[0]=code;
    data[1]=value;
    uint16_t cs = code + value;
    data[2] = (cs>>8);
    data[3] = (cs & 0xff);
    MCUPort->write(data,4);
    char in=0;
    MCUPort->waitForReadyRead(30);
    uint n = MCUPort->read(&in,1);
    uint msg = (uint)in;
    //qDebug()<<msg;
    if(msg != 35)
    {
        if(msg == 45)
            qDebug()<<">>>>>> MCU recovered from timeout";
        else
        {
            qDebug()<<">>>>>> MCU Com lost, reconnecting";
            MCUConnect();
        }
    }


}

void MainWindow::turnRelayOn(uint relayId)
{
    if(relayId>7)
    {
        qDebug()<<"relay id error";
        return;
    }
    relaysStates[relayId] = true;
    uint8_t buf = 0;
    for(uint i = 0;i<8;i++)
        buf+=(1<<i)*relaysStates[i];
    sendToMCU(MCU_RELAY,buf);

}
void MainWindow::turnRelayOff(uint relayId)
{
    if(relayId>7)
    {
        qDebug()<<"relay id error";
        return;
    }
    relaysStates[relayId] = false;
    char buf = 0;
    for(uint i = 0;i<8;i++)
        buf+=(1<<i)*relaysStates[i];
    sendToMCU(MCU_RELAY,buf);

}
void MainWindow::turnAllRelaysOn()
{
    char buf = 255;
    for(uint i = 0;i<8;i++)
        relaysStates[i] = true;
    sendToMCU(MCU_RELAY,buf);


}
void MainWindow::turnAllRelaysOff()
{

    char buf = 0;
    for(uint i = 0;i<8;i++)
        relaysStates[i] = false;
    sendToMCU(MCU_RELAY,buf);

}


void MainWindow::turnLedOn(uint ledId)
{
    if(ledId>5)
    {
        if(ledId==6)
            gpioWrite(12, true);
        else if(ledId==7)
            gpioWrite(13, true);
        else if(ledId==8)
            gpioWrite(4, true);
        else if(ledId==9)
            gpioWrite(5, true);
        else
        {

            qDebug()<<"led id error";
            return;
        }
    }
    ledsStates[ledId] = true;
    uint8_t buf = 0;
    for(uint i = 0;i<8;i++)
        buf+=(1<<i)*ledsStates[i];
    sendToMCU(MCU_LED,buf);

}
void MainWindow::turnLedOff(uint ledId)
{
    if(ledId>5)
    {
        if(ledId==6)
            gpioWrite(12, true);
        else if(ledId==7)
            gpioWrite(13, true);
        else if(ledId==8)
            gpioWrite(4, true);
        else if(ledId==9)
            gpioWrite(5, true);
        else
        {

            qDebug()<<"led id error";
            return;
        }
    }
    ledsStates[ledId] = false;
    char buf = 0;
    for(uint i = 0;i<8;i++)
        buf+=(1<<i)*ledsStates[i];
    sendToMCU(MCU_LED,buf);

}
void MainWindow::turnAllLedsOn()
{
    turnLedOn(6);
    turnLedOn(7);
    turnLedOn(8);
    turnLedOn(9);
    char buf = 255;
    for(uint i = 0;i<8;i++)
        ledsStates[i] = true;
    sendToMCU(MCU_LED,buf);


}
void MainWindow::turnAllLedsOff()
{
    turnLedOff(6);
    turnLedOff(7);
    turnLedOff(8);
    turnLedOff(9);
    char buf = 0;
    for(uint i = 0;i<8;i++)
        ledsStates[i] = false;
    sendToMCU(MCU_LED,buf);

}


void MainWindow::turnVoyantOn(uint voyantId)
{
    if(voyantId>5)
    {

        qDebug()<<"led id error";
        return;

    }
    voyantsStates[voyantId] = true;
    uint8_t buf = 0;
    for(uint i = 0;i<8;i++)
        buf+=(1<<i)*voyantsStates[i];
    sendToMCU(MCU_VOYANT,buf);

}
void MainWindow::turnVoyantOff(uint voyantId)
{
    if(voyantId>5)
    {
        qDebug()<<"voyant id error";
        return;
    }
    voyantsStates[voyantId] = false;
    char buf = 0;
    for(uint i = 0;i<8;i++)
        buf+=(1<<i)*voyantsStates[i];
    sendToMCU(MCU_VOYANT,buf);

}
void MainWindow::turnAllVoyantsOn()
{
    char buf = 255;
    for(uint i = 0;i<8;i++)
        voyantsStates[i] = true;
    sendToMCU(MCU_VOYANT,buf);


}
void MainWindow::turnAllVoyantsOff()
{
    char buf = 0;
    for(uint i = 0;i<8;i++)
        voyantsStates[i] = false;
    sendToMCU(MCU_VOYANT,buf);
}








void MainWindow::initRelayStates()
{
    char buf = 1      //electro aimant on
            + (1 <<1)  //solenoide1 on
            + (1 <<2)  //solenoide2 on
            + (1 <<3)  //solenoide3 on
            + (1 <<4)  //solenoide4 on
            + (0 <<5)  //digicode off
            + (0 <<6)  //gyrophare off
            + (1 <<7); //ventilateur on
    for(uint i = 0;i<8;i++)
        relaysStates[i] = (buf>>i)&1;
    sendToMCU(MCU_RELAY,buf);

    qDebug()<<"relay init";

}


void MainWindow::initLedStates(void)
{

    char buf = 1      //led1 on
            + (0 <<1)  //led2 off
            + (0 <<2)  //led3 off
            + (0 <<3)  //led4 off
            + (1 <<4)  //led5 on
            + (0 <<5);  //led6 off
    for(uint i = 0;i<8;i++)
        ledsStates[i] = (buf>>i)&1;
    sendToMCU(MCU_LED,buf);

    qDebug()<<"led init";

}

void MainWindow::initVoyantStates(void)
{

    char buf = 1      //v1 on
            + (0 <<1)  // off
            + (0 <<2)  // off
            + (0 <<3)  // off
            + (0 <<4)  // off
            + (0 <<5);  // off
    for(uint i = 0;i<8;i++)
        voyantsStates[i] = (buf>>i)&1;
    sendToMCU(MCU_VOYANT,buf);

    qDebug()<<"voyant init";

}

