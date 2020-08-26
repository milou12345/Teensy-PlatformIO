#ifndef __MOSFETMATRIX_H__
#define __MOSFETMATRIX_H__
#include <Arduino.h>
#include <mosfet.h>
#include "PID_v1.h"
#include "LED.h"
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
    LED ledPS1;
    LED ledPS2;

    MosfetMatrix(byte mosPin1, byte mosPin2, byte mosPin3, byte mosPin4, byte ledPin1, byte ledPin2);
    void switchPS();    // Switch to other PS when only one is active
    void switchToPS1(); //Switch to PS1 and switch off PS2
    void switchToPS2(); //Switch to PS2 and switch off PS1

    void switchOnPS1();  // Switch on PS1
    void switchOnPS2();  // Switch on PS2
    void switchOffPS1(); // Switch off PS1
    void switchOffPS2(); // Switch off PS2

    void switchOff();
    byte getSwitchPos();
    bool getHotSwapEnable();
    void setHotSwapEnable();
};

#endif // __MOSFETMATRIX_H__