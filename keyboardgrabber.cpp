#include "keyboardgrabber.h"
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <qdebug.h>
#define UNGRAB    0
#define GRAB      1


keyGrabber::keyGrabber(QObject *parent)
    : QObject{parent}
{

}



void keyGrabber::readKeys(void)
{

    int fd = 0;
    if( (fd = open("/dev/input/event0", O_RDONLY)) > 0 )
    {
        if(ioctl(fd, EVIOCGRAB, GRAB)==0)
            qDebug()<<"Keyboard grabbed";
        else qDebug()<<"failed to grab keyboard";

        struct input_event event;
        while(1){

            read(fd, &event, sizeof(event));
            if ((event.type == EV_KEY) &&(event.value==1)){

                switch(event.code)
                {
               case 28: emit keyStart();qDebug()<<"key start";break;
               case 105: emit keyPrev();break;
               case 106: emit keyNext();break;
                case 108: emit keyStop();break;
                default: qDebug()<<"key"<<event.code;

                }



            }
            QThread::usleep(100);
        }



        qDebug()<<"Key grabber closed";


    }
    deleteLater();

}
