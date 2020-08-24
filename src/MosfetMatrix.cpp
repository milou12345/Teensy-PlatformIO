#include "MosfetMatrix.h"

MosfetMatrix::MosfetMatrix(byte pin1, byte pin2, byte pin3, byte pin4)
{

    mos1 = Mosfet(pin1);
    mos2 = Mosfet(pin2);
    mos2 = Mosfet(pin3);
    mos2 = Mosfet(pin4);
    switch (mos1.getState())
    {
    case 0:
        switchPosition = 0;
        break;
    case 1:
        switchPosition = 1;
        break;
    default:
        break;
    }
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
            mos1.switchOn();

            mos2.switchOff();
            switchPosition = 1; //Standart PS
            break;

        case 1:
            mos1.switchOff();
            Serial.println("Mos2 switched on");
            mos2.switchOn();
            switchPosition = 0; //HotSwap PS
            break;

        default:
            break;
        }
    }
}

void MosfetMatrix::switchToPS1()
{
    mos1.switchOn();
    mos2.switchOff();
    switchPosition = 1;
}

void MosfetMatrix::switchToPS2()
{
    mos1.switchOff();
    mos2.switchOn();
    switchPosition = 0;
}

void MosfetMatrix::switchOff()
{
    mos1.switchOff();
    mos2.switchOff();
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