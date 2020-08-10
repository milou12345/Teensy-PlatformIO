#include "main.h"
#include "Arduino.h"

double Input; //MEasured Current
//byte buttonPins[] = {SWITCH_PIN_MOS1, SWITCH_PIN_MOS2, BUTTON_PIN_SWAP};
shopper shopperMain(PWM_PIN_1, PWM_FREQ);
shopper shopperBackup(PWM_PIN_2, PWM_FREQ);
LED ledMos1(LED_PIN_MOS_1);
LED ledMos2(LED_PIN_MOS_2);
LED ledSwap(LED_PIN_EN);
MosfetMatrix matrix(mosfet1Pin, mosfet2Pin);
Sensor currentSensor(PIN_INPUT, 8);
Button switchMos1 = Button();
Button switchMos2 = Button();
Button button = Button();

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

  switchMos1.attach(SWITCH_PIN_MOS1, INPUT_PULLUP);
  switchMos2.attach(SWITCH_PIN_MOS2, INPUT_PULLUP);
  button.attach(BUTTON_PIN_SWAP, INPUT_PULLUP);

  button.interval(5);
  switchMos2.interval(5);
  switchMos1.interval(5);

  button.setPressedState(LOW);
  switchMos1.setPressedState(LOW);
  switchMos2.setPressedState(LOW);

  //initialize the variables we're linked to for PID
  Input = currentSensor.getCurrentSensed();

  //Initialize PID for shopper
  //shopperMain.PidInit(&Input, SETPOINT, KP, KI, KD);
  //shopperBackup.PidInit(&Input, SETPOINT, KP, KI, KD);
  Serial.begin(115200);
  pinMode(12, OUTPUT);
}

void loop()
{
  button.update();
  switchMos1.update();
  switchMos2.update();

  //Aktivate HotSwap with switch
  /* if (digitalRead(hotSwapEnablePin) == true)
    hotSwapEnable = true;
  if (hotSwapEnable == true)
    digitalWrite(onboardLed, HIGH); */

  if (switchMos1.pressed())
  {
    Serial.println("Switch1 pressed");
    ledMos1.switchOn();
  }
  else if (switchMos1.released())
  {
    Serial.println("Switch1 released");
    ledMos1.switchOFf();
  }

  if (switchMos2.pressed())
  {
    Serial.println("Switch2 pressed");
    ledMos2.switchOn();
  }
  else if (switchMos2.released())
  {
    Serial.println("Switch2 released");
    ledMos2.switchOFf();
  }

  if (button.pressed())
  {
    Serial.println("button pressed");
    ledSwap.switchOn();
  }
  else if (button.released())
  {
    Serial.println("button released");
    ledSwap.switchOFf();
  }

  ledMos1.switchOn();
  //ledMos2.switchOn();
  //ledSwap.switchOn();
  
  digitalWrite(13, HIGH);
  //Serial.println("main loop ");
  //Input = currentSensor.getCurrentSensed();

  //shopperMain.PidCompute();
}
