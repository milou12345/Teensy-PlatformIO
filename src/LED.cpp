#include "LED.h"

LED::LED(byte pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
    status = 0;
}

void LED::switchOn()
{
    if (status == 0)
    {
        status = 1;
        digitalWrite(pin, HIGH);
    }
}

void LED::switchOFf()
{
    if (status == 1)
    {
        status = 0;
        digitalWrite(pin, LOW);
    }
}

void LED::changeState()
{
    switch (status)
    {
    case 1:
        switchOFf();

        break;

    default:
        switchOn();
        break;
    }
}

bool LED::getStatus()
{
    return status;
}
