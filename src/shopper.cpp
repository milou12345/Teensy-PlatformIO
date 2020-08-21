#include "shopper.h"

shopper::shopper(byte pwmPin, int freq)
{
    this->pwmPin = pwmPin;
    this->freq = freq;
    analogWriteFrequency(this->pwmPin, this->freq);
    init();
}

void shopper::init()
{
    Kp = 0;
    Ki = 0;
    Kd = 0;
    Setpoint = 0;
    Output = 0;
    dutyCyle = 0;
}

void shopper::setDutyCyle(byte dutyCyle)
{
    this->dutyCyle = dutyCyle;
    dutyCyleMap = map(this->dutyCyle, 0, 100, 1, 255);
    analogWrite(this->pwmPin, dutyCyleMap);
}

void shopper::PidCompute()
{

    currentPID.Compute();

    if (currentPID.GetMode() == 1)
    {

        analogWrite(this->pwmPin, this->Output);
    }
}

void shopper::PidInit(double *const Input, double Setpoint,
                      double Kp, double Ki, double Kd)
{
    //this->Input = &Input;
    this->Setpoint = Setpoint;
    this->Kp = Kp;
    this->Kd = Kd;
    this->Ki = Ki;
    //  this->Input = Input;

    currentPID.Init(Input, &this->Output, &this->Setpoint, this->Kp, this->Ki, this->Kd, DIRECT);
    currentPID.SetMode(MANUAL); // PID is Off !
    //currentPID.SetResolution(MICROS);
    currentPID.SetSampleTime(SAMPLE_TIME);
}

void shopper::setSetpoint(double Setpoint)
{
    this->Setpoint = Setpoint;
}

void shopper::setKp(double Kp)
{
    this->Kp = Kp;
}

void shopper::setKi(double Ki)
{
    this->Ki = Ki;
}

void shopper::setKd(double Kd)
{
    this->Kd = Kd;
}

void shopper::setOutput(double Output)
{
    this->Output = Output;
}

double shopper::getOutput()
{
    return this->Output;
}

void shopper::writePwm()
{
    //Serial.println(currentPID.GetMode());
    /*  switch (currentPID.GetMode())
    {
    case 0:
        analogWrite(pwmPin, dutyCyleMap);
        Serial.println("Wirte DutyCyle oitput");
        break;
    case 1:
        Serial.println("Wirte PID oitput");
        analogWrite(pwmPin, Output);

    default:
        analogWrite(pwmPin, 0);
        break;
    } */
}
