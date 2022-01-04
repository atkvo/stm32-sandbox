#include <cstdint>
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"
#include "lib/hal/include/gpio.hpp"

enum AppMode
{
    SwitchMode,
    BlinkyMode
};

uint8_t const LED_PIN = 13;        // PC.13
uint8_t const USER_SWITCH_PIN = 0; // PA.0

// This can be considered as an "application" using the GPIO
// interface here. Eventually we should be able to mock the
// Gpio object out 
void SetLed(Gpio &gpio, bool on)
{
    // if we want to turn the LED on, write
    // a 0 to the output register since it's 
    // an open drain output
    gpio.WriteOutput(LED_PIN, on ? 0 : 1);
}

int main()
{
    SystemInit();

    AppMode MODE = BlinkyMode;

    Gpio portC(GpioPort::C);
    portC.SetMode(LED_PIN, GpioMode::Output);
    portC.SetOutputType(LED_PIN, GpioOutputType::OpenDrain);

    Gpio portA(GpioPort::A);
    if (MODE == SwitchMode)
    {
        // PA.0 has an ACTIVE LOW button connected
        portA.SetMode(USER_SWITCH_PIN, GpioMode::Input);
    }

    bool const ON = true;
    bool const OFF = false;

    while(1)
    {
        if (!portA.ReadInput(USER_SWITCH_PIN))
        {
            MODE = SwitchMode;
        }

        if (MODE == SwitchMode)
        {
            // PA.0 is has an active low switch on it
            // so turn on the LED when it reads 0
            SetLed(portC, portA.ReadInput(USER_SWITCH_PIN) ? ON : OFF);
        }
        else
        {
            SetLed(portC, ON);
            for (volatile int i = 0; i < 500000; i++); // arbitrary delay
            SetLed(portC, OFF);
            for (volatile int i = 0; i < 500000; i++); // arbitrary delay
        }
    }

    return 0;
}
