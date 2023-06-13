#pragma once

#include "lib/hal/include/gpio.hpp"
#include "lib/hal/include/usart.hpp"

extern uint32_t SystemCoreClock;

namespace usart_scratch
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

    void Transmit(Usart &u)
    {
        uint8_t MSG[] = "HELLO\n";
        const uint8_t MSG_SIZE = sizeof(MSG);

        for (uint8_t i = 0; i < MSG_SIZE; i++)
        {
            if (!u.Tx(MSG[i]))
            {
                break;
            }
        }
    }

    bool isSwitchPressed(Gpio &PA, uint8_t pinIndex)
    {
        return (!PA.ReadInput(USER_SWITCH_PIN)) ? ON : OFF;
    }

    void run()
    {
        Gpio PA(GpioPort::A);

        PA.SetMode(9,   GpioMode::Alternate); // USART1 TX
        PA.SetMode(10,  GpioMode::Alternate); // USART1 RX
        PA.SetSpeed(9,  GpioSpeed::High);
        PA.SetSpeed(10, GpioSpeed::High);
        PA.SetPullUpPullDown(9,  GpioPuPdMode::Pullup);
        PA.SetPullUpPullDown(10, GpioPuPdMode::Pullup);

        PA.SetAfMode(9, 0x7);
        PA.SetAfMode(10, 0x7);

        PA.SetMode(USER_SWITCH_PIN, GpioMode::Input);

        Gpio PC(GpioPort::C);
        PC.SetMode(LED_PIN, GpioMode::Output);

        Usart usart(UsartNum::USART_1);

        usart.Configure(115200);

        bool switchPressed = isSwitchPressed(PA, USER_SWITCH_PIN);
        const uint32_t LIMIT = SystemCoreClock / 16000;
        uint32_t count = 0;
        bool ledState = false;

        while (true)
        {
            switchPressed = isSwitchPressed(PA, USER_SWITCH_PIN);

            if (!switchPressed)
            {
                Transmit(usart);
                // USART1.Tx()
                // rawData = adc.Read();
                // rawData = pollAdc(adc);
            }

            count++;
            uint32_t d = 0;
            if (count >= LIMIT || usart.Rx(d))
            {
                count = 0;
                ledState = !ledState;
                PC.WriteOutput(LED_PIN, ledState);
            }
        }
    }
}
