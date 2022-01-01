#include <cstdint>
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "lib/hal/gpio/gpio.hpp"
#include "lib/hal/gpio/stm32f411xe_gpio.hpp"

// This can be considered your "application" interface here
void set_led(GpioInterface &gpio, bool on)
{
    // if we want to turn the LED on, write
    // a 0 to the output register since it's 
    // an open drain output
    gpio.Write(13, on ? 0 : 1);
}

int main()
{
    SystemInit();

    Gpio<GpioPort::C> portC;

    portC.SetMode(13, GpioMode::Output);
    portC.SetOutputType(13, GpioOutputType::OpenDrain);

    bool const ON = true;
    bool const OFF = false;
    while(1)
    {
        set_led(portC, ON);
        for (int i = 0; i < 500000; i++); // arbitrary delay
        set_led(portC, OFF);
        for (int i = 0; i < 500000; i++); // arbitrary delay
    }

    return 0;
}
