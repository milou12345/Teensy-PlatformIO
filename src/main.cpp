#include "main.h"
#include "Arduino.h"

double Input; //MEasured Current
bool manualControl;
bool hotSwapControl;
shopper shopperMain(PWM_PIN_1, PWM_FREQ);
shopper shopperBackup(PWM_PIN_2, PWM_FREQ);
LED ledMos1(LED_PIN_MOS_1);
LED ledMos2(LED_PIN_MOS_2);
LED ledManualControl(LED_PIN_EN);
MosfetMatrix matrix(MOSFET_PIN_1, MOSFET_PIN_2);
Sensor currentSensor(PIN_INPUT, 12);
Button switchMos1 = Button();
Button switchMos2 = Button();
Button buttonSwap = Button();
Button buttonHSEnable = Button();
Button buttonManualControl = Button();


// int hotSwap(double currentMeasure)
// { // Function for the HotSwap event
//   if (hotSwapEnable == true)
//   {
//     if ((currentMeasure <= hotSwapLimit))
//     {
//       // Switch Mosfets for Hotswap
//       matrix.switchToPS2();

//       // PWM to 100% till current rises to wanted value
//       do
//       {
//         ledcWrite(ledChannel, maxPwm);
//       } while (analogRead(PIN_INPUT) <= sollwert);
//       hotSwapEnable = false;
//       return 1;
//     }
//   }
//   return 0;
// }

void setup()
{
    //  analogWriteFrequency(33, 10000000);

    switchMos1.attach(SWITCH_PIN_MOS1, INPUT_PULLUP);
    switchMos2.attach(SWITCH_PIN_MOS2, INPUT_PULLUP);
    buttonSwap.attach(BUTTON_PIN_SWAP, INPUT_PULLUP);
    buttonHSEnable.attach(BUTTON_PIN_SWAP, INPUT_PULLUP);
    buttonManualControl.attach(BUTTON_PIN_MANUAL_CONTROL, INPUT_PULLUP);

    buttonSwap.interval(5);
    switchMos2.interval(5);
    switchMos1.interval(5);
    buttonHSEnable.interval(5);
    buttonManualControl.interval(5);

    buttonSwap.setPressedState(LOW);
    switchMos1.setPressedState(LOW);
    switchMos2.setPressedState(LOW);
    buttonHSEnable.setPressedState(LOW);
    buttonManualControl.setPressedState(LOW);

    //initialize the variables we're linked to for PID
    Input = currentSensor.getCurrentSensed();

    //Initialize PID for shopper
    shopperMain.PidInit(&Input, SETPOINT, KP, KI, KD);
    shopperBackup.PidInit(&Input, SETPOINT, KP, KI, KD);

    Serial.begin(115200);

    pinMode(13, OUTPUT); //Onboard led


}

void loop()
{
    //analogWrite(33, 100);
    buttonSwap.update();
    switchMos1.update();
    switchMos2.update();
    buttonHSEnable.update();


    //Aktivate HotSwap with switch
    /* if (digitalRead(hotSwapEnablePin) == true)
      hotSwapEnable = true;
    if (hotSwapEnable == true)
      digitalWrite(onboardLed, HIGH); */
    if (manualControl==true)
    {
        ledManualControl.switchOn();

        if (switchMos1.pressed())
        {
            Serial.println("Switch1 pressed");
            ledMos1.switchOn();
            matrix.mos1.switchOn();
        }
        else if (switchMos1.released())
        {

            ledMos1.switchOFf();
            matrix.mos1.switchOff();
        }

        if (switchMos2.pressed())
        {

            ledMos2.switchOn();
            matrix.mos2.switchOn();
        }
        else if (switchMos2.released())
        {

            ledMos2.switchOFf();
            matrix.mos2.switchOff();
        }

        if (buttonSwap.risingEdge())
        {

            matrix.switchPS();
        }

    }
    else if (hotSwapControl==true)
    {


    }







    Input = currentSensor.getVoltageSensed();


    delay(10);
    shopperMain.PidCompute();
    Serial.println(shopperMain.Output);
    shopperMain.setDutyCyle(50);
    shopperBackup.setDutyCyle(80);
}
