#ifndef __MOSFETMATRIX_H__
#define __MOSFETMATRIX_H__
#include <Arduino.h>
#include <mosfet.h>
#include "PID_v1.h"
class MosfetMatrix
{
private:
    bool hotSwapEnable;

    byte switchPosition; // Which switch combination is conduction

public:
    Mosfet mos1;
    Mosfet mos2;
    Mosfet mos3;
    Mosfet mos4;
    LED ledMos1(LED_PIN_MOS_1);
    LED ledMos2(LED_PIN_MOS_2);

    MosfetMatrix(byte pin1, byte pin2, byte pin3, byte pin4);
    void switchPS();
    void switchToPS1();
    void switchToPS2();

    byte getSwitchPos();
    bool getHotSwapEnable();
    void setHotSwapEnable();
};

#endif // __MOSFETMATRIX_H__