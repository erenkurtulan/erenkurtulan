#include "stdint.h"
#include "stdbool.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "driverlib/sysctl.h"
#include "driverlib/sysctl.c"
#include "driverlib/pin_map.h"
#include "math.h"
#include "inc/hw_gpio.h"


#define RED_LED   GPIO_PIN_1
#define SW1       GPIO_PIN_4
#define SW2       GPIO_PIN_0

void delayMs(int delayTime);

int main(void) {

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);


    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}


    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= SW2;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;


    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1 | SW2);


    GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    int delayTime = 500;
    bool sw1Pressed = false;
    bool sw2Pressed = false;

    while(1) {
        if (GPIOPinRead(GPIO_PORTF_BASE, SW1) == 0) {
            if (!sw1Pressed) {
                delayTime = 100;
                sw1Pressed = true;
            }
        } else {
            sw1Pressed = false;
        }


        if (GPIOPinRead(GPIO_PORTF_BASE, SW2) == 0) {
            if (!sw2Pressed) {
                delayTime = 1000;
                sw2Pressed = true;
            }
        } else {
            sw2Pressed = false;
        }


        GPIOPinWrite(GPIO_PORTF_BASE, RED_LED, ~GPIOPinRead(GPIO_PORTF_BASE, RED_LED) & RED_LED);


        delayMs(delayTime);
    }
}

void delayMs(int delayTime) {
    SysCtlDelay((SysCtlClockGet() / 3) * (delayTime / 1000.0));
}
