// Defines
#define PIN_INPUT 24 //Current Mesaurment
#define PWM_PIN_1 19 //PWM Output 1
#define PWM_PIN_2 18 //PWM Output 1

#define PWM_RESOLUTION 8
#define maxPwm pow(2, RESOLUTION)

#define MOSFET_PIN_1 22
#define MOSFET_PIN_2 23
#define MOSFET_PIN_3 33
#define MOSFET_PIN_4 34

#define LED_PIN_MOS_1 2
#define LED_PIN_MOS_2 1
#define LED_PIN_MAN_EN 3
#define LED_PIN_HS_EN 6

#define SWITCH_PIN_MOS1 4
#define SWITCH_PIN_MOS2 28
#define BUTTON_PIN_SWAP 10
#define BUTTON_PIN_HS_Enable 5
#define BUTTON_PIN_MANUAL_CONTROL 30

#define PWM_FREQ 100000
#define hotSwapEnablePin 9

#define VDD 3.3
#define HOT_SWAP_LIM 40
#define SAMPLE_TIME 1
//Shopper
#define KP 0.11466
#define KI 1.7371
#define KD 0
#define SETPOINT 50

//#define DUTY_CYLE_STATIC 1
#define PID_MODE 1
#define DUTY_CYLE 50
#define SAFTEY_CURRENT 80