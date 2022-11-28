#include "rfidreader.h"
#include "pigpio.h"
#include "CMFRC522/CMFRC522.h"


rfidReader::rfidReader()
{
    rfidGpios = {22,23,24,25,26};
    rfidCodes = {"d3a2e7a9","53a9afa9","53f7d8a9","b3b9eca3","53e0ad94"};
    rfidVector.resize(5,0);
    for(auto i:rfidGpios)
        gpioSetMode(i, PI_OUTPUT);
    for(auto i:rfidGpios)
        gpioWrite(i, PI_LOW);
}


void rfidReader::readProcess(void)
{

    CMFRC522 s;
    QString code;
    uint previousValue;
    while(1)
    {
        for(uint i =0;i<rfidGpios.size();i++)
        {
            QThread::usleep(5000);
            for(auto i:rfidGpios)
                gpioWrite(i, PI_LOW);
            QThread::usleep(5000);
            code = s.read(rfidGpios[i]);

            previousValue = rfidVector[i];
            rfidVector[i] = 0;
            if(code!="")
            {
                for(uint j = 0;j<rfidGpios.size();j++)
                    if(rfidCodes[j]==code)
                    {
                        rfidVector[i] = j+1;
                        break;
                    }

            }
            if(rfidVector[i]!=previousValue)
            {
                qDebug()<<"rfid "<<i<<rfidVector[i];
                if(rfidVector[i]==i+1)
                    cardOK(i+1);
                else if(rfidVector[i]>0)
                    emit disquetteError();
                emit rfidChanged(rfidVector);
            }

        }
    }


}


void rfidReader::cardOK(uint i)
{
    if(i==1)
        emit disquette1();
    if(i==2)
        emit disquette2();
    if(i==3)
        emit disquette3();
    if(i==4)
        emit disquette4();
    if(i==5)
        emit disquette5();
}
