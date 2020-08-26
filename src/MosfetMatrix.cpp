#include "MosfetMatrix.h"

MosfetMatrix::MosfetMatrix(byte mosPin1, byte mosPin2, byte mosPin3, byte mosPin4, byte ledPin1, byte ledPin2)
{

    mos1 = Mosfet(mosPin1);
    mos2 = Mosfet(mosPin2);
    mos3 = Mosfet(mosPin3);
    mos4 = Mosfet(mosPin4);
    ledPS1 = LED(ledPin1);
    ledPS2 = LED(ledPin2);

    switchPosition = 0; //SwitchPosition = 0 TO-DO
}

/************************************************************
 * Switch PS if either one of them is in a different state
 *************************************************************/
void MosfetMatrix::switchPS()
{
    if (mos1.getState() != mos2.getState())
    {

        switch (mos1.getState())
        {

        case 0:
            switchToPS1();
            break;
        case 1:
            switchToPS2();
            break;
        default:
            break;
        }
    }
}

void MosfetMatrix::switchToPS1()
{
    //Switch on PS 1
    switchOnPS1();

    //Switch off PS 2
    switchOffPS2();

    switchPosition = 1; // PS 1 is running
}

void MosfetMatrix::switchToPS2()
{
    //Switch off PS 1
    switchOffPS1();

    //Switch on PS 2
    switchOnPS2();
    switchPosition = 0;
}

void MosfetMatrix::switchOnPS1()
{
    mos1.switchOn();
    mos3.switchOn();
    ledPS1.switchOn();
}

void MosfetMatrix::switchOnPS2()
{
    mos2.switchOn();
    mos4.switchOn();
    ledPS2.switchOn();
}

void MosfetMatrix::switchOffPS1()
{
    mos1.switchOff();
    mos3.switchOff();
    ledPS1.switchOff();
}

void MosfetMatrix::switchOffPS2()
{
    mos2.switchOff();
    mos4.switchOff();
    ledPS2.switchOff();
}

void MosfetMatrix::switchOff()
{
    mos1.switchOff();
    mos2.switchOff();
    mos3.switchOff();
    mos4.switchOff();
    ledPS1.switchOff();
    ledPS2.switchOff();
}

byte MosfetMatrix::getSwitchPos()
{
    return switchPosition;
}

bool MosfetMatrix::getHotSwapEnable()
{
    return hotSwapEnable;
}

void MosfetMatrix::setHotSwapEnable()
{
    this->hotSwapEnable = true;
}