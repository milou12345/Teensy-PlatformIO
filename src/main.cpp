#include "main.h"
#include "Arduino.h"

double Input, Output; //MEasured Current
bool manualControl = false, hotSwapControl = false, swap = false;
shopper shopperMain(PWM_PIN_1, PWM_FREQ);
shopper shopperBackup(PWM_PIN_2, PWM_FREQ);

LED ledManualControl(LED_PIN_MAN_EN);
LED ledHotSwapEnable(LED_PIN_HS_EN);
MosfetMatrix matrix(MOSFET_PIN_1, MOSFET_PIN_2, MOSFET_PIN_3, MOSFET_PIN_4, LED_PIN_MOS_1, LED_PIN_MOS_2);
Sensor currentSensor(PIN_INPUT, 12);
Button switchPS1 = Button();
Button switchPS2 = Button();
Button buttonSwap = Button();
Button buttonHSEnable = Button();
Button buttonManualControl = Button();

void setup()
{

    /*************************************
    * Set up for die Switches and Buttons
    *************************************/

    switchPS1.attach(SWITCH_PIN_MOS1, INPUT_PULLUP);
    switchPS2.attach(SWITCH_PIN_MOS2, INPUT_PULLUP);
    buttonSwap.attach(BUTTON_PIN_SWAP, INPUT_PULLUP);
    buttonHSEnable.attach(BUTTON_PIN_HS_Enable, INPUT_PULLUP);
    buttonManualControl.attach(BUTTON_PIN_MANUAL_CONTROL, INPUT_PULLUP);

    buttonSwap.interval(5);
    switchPS2.interval(5);
    switchPS1.interval(5);
    buttonHSEnable.interval(5);
    buttonManualControl.interval(5);

    buttonSwap.setPressedState(LOW);
    switchPS1.setPressedState(LOW);
    switchPS2.setPressedState(LOW);
    buttonHSEnable.setPressedState(LOW);
    buttonManualControl.setPressedState(LOW);

    //initialize the variables we're linked to for PID
    Input = currentSensor.getCurrentSensed();

    //Initialize PID for shopper
    shopperMain.PidInit(&Input, SETPOINT, KP, KI, KD);
    shopperBackup.PidInit(&Input, SETPOINT, KP, KI, KD);

    Serial.begin(115200);

    pinMode(13, OUTPUT);    //Onboard led
    digitalWrite(13, HIGH); //Switch on Led to see board working
}

void loop()
{

    buttonSwap.update();
    switchPS1.update();
    switchPS2.update();
    buttonHSEnable.update();
    buttonManualControl.update();

    Input = currentSensor.getCurrentSensed(); // Measure Current

    /***************************************************************
    * Register Button presses and Switches ans set flags
    *************************************************************/
    if (buttonManualControl.risingEdge() && (manualControl == false)) // Manual control on
    {
        manualControl = true;
        hotSwapControl = false;
        Serial.println("Manual Control Activated!");
    }
    else if (buttonManualControl.risingEdge() && (manualControl == true)) // Manual control off
    {

        manualControl = false;
        Serial.println("Manual Control Deactivated!!");
    }

    if (buttonHSEnable.risingEdge() && (hotSwapControl == false)) // Hot-Swap on
    {
        hotSwapControl = true;

        Serial.println("HS Controll activated!");
    }
    else if (buttonHSEnable.risingEdge() && (hotSwapControl == false)) // Hot-Swap off
    {
        hotSwapControl = false;
        ledHotSwapEnable.switchOff();
        Serial.println("HS Controll deactivated!");
    }

    /*****************************************************
     * Manual control over IGBT/MOSFET switch status.
     * Can be acivated with Manual control switch.
     * Performs every action needed in case of button/switch
     * presses.
     * ***************************************************/

    if (manualControl == true)
    {
        ledManualControl.switchOn();

        if (switchPS1.fallingEdge())
        {
            Serial.println("Switch1 On");
            matrix.switchOnPS1();
#ifdef DUTY_CYLE_STATIC

            shopperMain.setDutyCyle(DUTY_CYLE);
#endif

#ifdef PID_MODE
            shopperMain.currentPID.SetMode(AUTOMATIC);
#endif
        }
        else if (switchPS1.risingEdge())
        {
            Serial.println("Switch1 Off");

            matrix.switchOffPS1();
            shopperMain.currentPID.SetMode(MANUAL);
            shopperMain.setDutyCyle(0);
        }

        if (switchPS2.fallingEdge())
        {
            Serial.println("Switch2 On");

            matrix.switchOnPS2();
#if (DUTY_CYLE_STATIC)
            shopperBackup.setDutyCyle(DUTY_CYLE);
#endif
#if !(DUTY_CYLE_STATIC)
            shopperBackup.currentPID.SetMode(AUTOMATIC);
#endif
        }
        else if (switchPS2.risingEdge())
        {
            Serial.println("Switch2 Off");
            matrix.switchOffPS2();
            shopperBackup.currentPID.SetMode(MANUAL);
            shopperBackup.setDutyCyle(0);
        }

        if (buttonSwap.risingEdge())
        {
            Serial.println("Manual Swap!");
            swap = true;
            matrix.switchPS();
            if (matrix.getSwitchPos() == 0)
            {
                Serial.println("Swaped Pid to Backup");
                shopperMain.currentPID.SetMode(MANUAL);
                shopperMain.setDutyCyle(0);
#if (DUTY_CYLE_STATIC)
                shopperBackup.setDutyCyle(DUTY_CYLE);
#endif
#if !(DUTY_CYLE_STATIC)
                shopperBackup.currentPID.SetMode(AUTOMATIC);
#endif
            }
            else if (matrix.getSwitchPos() == 1)
            {
                Serial.println("Swaped Pid to main");
                shopperBackup.currentPID.SetMode(MANUAL);
                shopperBackup.setDutyCyle(0);

#ifdef DUTY_CYLE_STATIC

                shopperMain.setDutyCyle(DUTY_CYLE);
#endif

#ifdef PID_MODE
                shopperMain.currentPID.SetMode(AUTOMATIC);
#endif
            }
        }
    }
    else if (manualControl == false)
    {
        ledManualControl.switchOff();
        matrix.switchOff();
        shopperMain.currentPID.SetMode(MANUAL);
        shopperBackup.currentPID.SetMode(MANUAL);

        shopperMain.setDutyCyle(0);
        shopperBackup.setDutyCyle(0);
    }

    /****************************************
     * Automatic current watch, with swap of
     * power supplys if conditions are met
     * *************************************/
    if (hotSwapControl == true)
    {
        Output = shopperMain.getOutput();
        ledHotSwapEnable.switchOn();
        if (matrix.getSwitchPos() != 1)
        {
            matrix.switchToPS1();
        }

        if ((currentSensor.getCurrentSensed() <= HOT_SWAP_LIM))
        {
            Serial.println("Hot Swap event!");
            Serial.println(currentSensor.getCurrentSensed());

            // Switch Mosfets for Hotswap
            matrix.switchToPS2();

            // PWM to 100% till current rises to wanted value
            do
            {
                shopperBackup.setDutyCyle(DUTY_CYLE);
                if (currentSensor.getCurrentSensed() >= SAFTEY_CURRENT)

                {
                    shopperMain.setDutyCyle(0);
                    shopperBackup.setDutyCyle(0);
                    matrix.switchOff();
                    break;
                }
            } while (currentSensor.getCurrentSensed() <= 48);
            //  shopperBackup.setOutput(Output);
            shopperBackup.currentPID.SetMode(AUTOMATIC);
            hotSwapControl = false;
            ledHotSwapEnable.switchOff();
        }
    }

    /*************************************************
     *  PID.Compute should be called every iteration
     ************************************************/

    shopperMain.PidCompute();
    shopperBackup.PidCompute();

    if (currentSensor.getCurrentSensed() >= SAFTEY_CURRENT)

    {
        shopperMain.setDutyCyle(0);
        shopperBackup.setDutyCyle(0);
        matrix.switchOff();
    }

    /* Serial.print("Output: ");
    Serial.println(shopperMain.Output); */
    Serial.print("Input: ");
    Serial.println(Input);
    // shopperMain.setDutyCyle(15);
    //shopperBackup.setDutyCyle(15);
}
