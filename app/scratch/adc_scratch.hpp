#pragma once

#include "lib/hal/include/adc.hpp"
#include "lib/hal/include/gpio.hpp"

namespace adc_scratch
{
    uint8_t const LED_PIN = 13;        // PC.13
    uint8_t const USER_SWITCH_PIN = 0; // PA.0

    const bool OFF = false;
    const bool ON  = true;

    void SetLed(Gpio &gpio, bool on)
    {
        // if we want to turn the LED on, write
        // a 0 to the output register since it's
        // an open drain output
        gpio.WriteOutput(LED_PIN, on ? 0 : 1);
    }

    bool isSwitchPressed(Gpio &PA, uint8_t pinIndex)
    {
        return (!PA.ReadInput(USER_SWITCH_PIN)) ? ON : OFF;
    }

    uint32_t pollAdc(Adc &a)
    {
        a.BeginConversion();
        while (!a.IsConversionDone()) { }
        return a.Read();
    }

    void run()
    {
        Gpio PC(GpioPort::C);
        Gpio PA(GpioPort::A);
        Adc  adc;
        adc.ConfigurePin(PA, 1);

        PC.SetMode(LED_PIN, GpioMode::Output);
        PC.SetOutputType(LED_PIN, GpioOutputType::OpenDrain);

        PA.SetMode(USER_SWITCH_PIN, GpioMode::Input);

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
