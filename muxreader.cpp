#include "muxreader.h"
#include "pigpio.h"


muxReader::muxReader()
{
    muxIndex = 0;
    muxValues.resize(3, std::vector<bool>(8, 0));
    gpioSetMode(16, PI_OUTPUT);
    gpioSetMode(17, PI_OUTPUT);
    gpioSetMode(18, PI_OUTPUT);
    gpioSetMode(19, PI_INPUT);
    gpioSetMode(20, PI_INPUT);
    gpioSetMode(21, PI_INPUT);

    registerButtonInput("bouton1",0,0);
    registerButtonInput("bouton2",0,1);
    registerButtonInput("bouton3",0,2);
    registerButtonInput("bouton4",0,3);
    registerButtonInput("bouton5",0,4);
    registerButtonInput("bouton6",0,5);
    registerButtonInput("bouton7",0,6);
    registerButtonInput("bouton8",0,7);

    registerButtonInput("bouton9",1,0);
    registerButtonInput("bouton10",1,1);
    registerButtonInput("bouton_erreur",1,2);
    registerButtonInput("interrupteur1",1,3);
    registerWireInput("cable_ok",1,4);
    registerWireInput("cable_erreur",1,5);
    registerButtonInput("anagramme1",1,6);
    registerButtonInput("anagramme2",1,7);

    registerJoyInput("joystick_haut",2,0);
    registerJoyInput("joystick_droite",2,1);
    registerJoyInput("joystick_gauche",2,2);
    registerJoyInput("joystick_bas",2,3);
    registerJoyInput("bouton11",2,4);
}





void muxReader::readProcess(void)
{
    while(1)
    {
        gpioWrite(16,muxIndex&0b1);
        gpioWrite(17,(muxIndex&0b10)>>1);
        gpioWrite(18,(muxIndex&0b100)>>2);
        QThread::usleep(1000);
        bool input[3] = {gpioRead(19),gpioRead(20),gpioRead(21)};

        for(uint i = 0;i<3;i++)
        {
            for(auto in:buttonInputs)
            {
                if((in->mux == i)&&(in->index==muxIndex))
                {
                    in->setState(input[i]);
                    break;
                }
            }
            for(auto in:joyInputs)
            {
                if((in->mux == i)&&(in->index==muxIndex))
                {
                    in->setState(input[i]);
                    break;
                }
            }

            for(auto in:wireInputs)
            {
                if((in->mux == i)&&(in->index==muxIndex))
                {
                    in->setState(input[i]);
                    break;
                }
            }

        }

        muxIndex++;
        if(muxIndex>7) muxIndex = 0;

        QThread::usleep(1000);
    }

}

input_c* muxReader::getButtonByName(QString name)
{
    for(auto in:buttonInputs)
    {
        if (in->name == name)
            return in;
    }
    return NULL;
}
input_c* muxReader::getWireByName(QString name)
{
    for(auto in:wireInputs)
    {
        if (in->name == name)
            return in;
    }
    return NULL;
}

input_c* muxReader::getJoyInputByName(QString name)
{
    for(auto in:joyInputs)
    {
        if (in->name == name)
            return in;
    }
    return NULL;
}



std::vector<input_c*> muxReader::getButtonByNotName(QString name)//provide pointer to all elements which are NOT called name
{
    std::vector<input_c *> inputs;
    for(auto in:buttonInputs)
    {
        if (in->name != name)
            inputs.push_back(in);
    }

    return inputs;
}

std::vector<input_c*> muxReader::getWireByNotName(QString name)//provide pointer to all elements which are NOT called name
{
    std::vector<input_c *> inputs;
    for(auto in:wireInputs)
    {
        if (in->name != name)
            inputs.push_back(in);
    }

    return inputs;
}

void input_c::setState(bool nuState)
{
    if((!state) && (nuState))
     {
        qDebug()<<name<<" triggered";
        emit triggered();
    }

    if((state) && (!nuState))
        emit disconnected();

    if(state!=nuState)
        emit changed();

    state = nuState;
}

void muxReader::registerButtonInput(QString name,uint mux,uint index)
{
    for(auto in:buttonInputs)
    {
        if (in->name == name)
        {
            qDebug()<<"error, button name already registered";
            return;
        }
    }
    buttonInputs.push_back(new input_c(name,mux,index));
}


void muxReader::registerWireInput(QString name,uint mux,uint index)
{
    for(auto in:wireInputs)
    {
        if (in->name == name)
        {
            qDebug()<<"error, wire name already registered";
            return;
        }
    }
    wireInputs.push_back(new input_c(name,mux,index));
}






void muxReader::registerJoyInput(QString name,uint mux,uint index)
{
    for(auto in:joyInputs)
    {
        if (in->name == name)
        {
            qDebug()<<"error, button name already registered";
            return;
        }
    }
    joyInputs.push_back(new input_c(name,mux,index));
}
