#pragma once

#include "lib/hal/include/adc.hpp"
#include "gpio.hpp"

namespace adc_scratch
{
    uint8_t const LED_PIN = 13;        // PC.13
    uint8_t const USER_SWITCH_PIN = 0; // PA.0

    const bool OFF = false;
    const bool ON  = true;

    template <typename T>
    requires IsGpioPort<T>
    void SetLed(T&gpio, bool on)
    {
        // if we want to turn the LED on, write
        // a 0 to the output register since it's
        // an open drain output
        gpio.write_pin(LED_PIN, on ? 0 : 1);
    }

    template <typename T>
    requires IsGpioPort<T>
    bool isSwitchPressed(T &PA, uint8_t pinIndex)
    {
        return (!PA.read_pin(USER_SWITCH_PIN)) ? ON : OFF;
    }

    uint32_t pollAdc(Adc &a)
    {
        a.start_conversion();
        while (!a.is_conversion_done()) { }
        return a.read();
    }

    void run()
    {
        Gpio PC(GpioPort::C);
        Gpio PA(GpioPort::A);
        Adc  adc;
        adc.configure_pin(PA, 1);

        PC.set_mode(LED_PIN, GpioMode::Output);
        PC.set_output_mode(LED_PIN, GpioOutputMode::OpenDrain);

        PA.set_mode(USER_SWITCH_PIN, GpioMode::Input);

        uint32_t rawData = 0;
        bool switchPressed = isSwitchPressed(PA, USER_SWITCH_PIN);

        while (true)
        {
            switchPressed = isSwitchPressed(PA, USER_SWITCH_PIN);

            if (switchPressed)
            {
                // rawData = adc.Read();
                rawData = pollAdc(adc);
            }

            if (rawData > 500)
            {
                SetLed(PC, ON);
            }
            else
            {
                SetLed(PC, OFF);
            }
        }
    }
}
