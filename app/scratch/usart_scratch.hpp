#pragma once

#include "lib/hal/include/gpio.hpp"
#include "lib/hal/include/usart.hpp"

extern uint32_t SystemCoreClock;

namespace usart_scratch
{
    uint8_t const LED_PIN = 13;        // PC.13
    uint8_t const USER_SWITCH_PIN = 0; // PA.0
    Gpio PC(GpioPort::C);

    const bool OFF = false;
    const bool ON  = true;

    enum Operations
    {
        LED_OFF,
        LED_ON,
        HELLO
    };

    void Transmit(Usart &u, uint8_t *c, uint32_t len)
    {
        for (uint8_t i = 0; i < len; i++)
        {
            if (!u.Tx(c[i]))
            {
                break;
            }
        }
    }

    void Transmit(Usart &u, const uint8_t *c)
    {
        uint32_t i = 0;
        for (uint32_t i = 0; c[i]; i++)
        {
            if (!u.Tx(c[i]))
            {
                break;
            }
        }
    }

    void Hello(Usart &u)
    {
        uint8_t MSG[] = "HELLO\r\n";
        const uint8_t MSG_SIZE = sizeof(MSG);
        Transmit(u, MSG, MSG_SIZE);
    }


    void SetLed(Gpio &gpio, bool on)
    {
        // if we want to turn the LED on, write
        // a 0 to the output register since it's
        // an open drain output
        gpio.WriteOutput(LED_PIN, on ? 0 : 1);
    }

    uint32_t MIN(uint32_t a, uint32_t b)
    {
        return a < b ? a : b;
    }

    bool isEqualTo(const char *c0, uint32_t len0, const char *cmd)
    {
        bool match = true;
        for (uint32_t i = 0; i < len0; i++)
        {
            if ((cmd[i] == 0) || (c0[i] != cmd[i]))
            {
                match = false;
                break;
            }
        }

        return match;
    }

    Operations Process(Usart &u, char *c, uint8_t len)
    {
        static char *cmds[] =
        {
            "LED_OFF",
            "LED_ON",
            "HELP",
            "HELLO"
        };

        if (isEqualTo(c, len, "LED_OFF"))
        {
            PC.WriteOutput(LED_PIN, OFF);
            SetLed(PC, OFF);
        }
        else if (isEqualTo(c, len, "LED_ON"))
        {
            SetLed(PC, ON);
        }
        else if (isEqualTo(c, len, "HELLO"))
        {
            Hello(u);
        }
        else if (isEqualTo(c, len, "HELP"))
        {
            for (uint8_t i = 0; i < sizeof(cmds) / sizeof(char*); i++)
            {
                Transmit(u, reinterpret_cast<uint8_t*>(cmds[i]));
                u.Tx('\r');
                u.Tx('\n');
            }
        }
    }

    void Receive(Usart &u)
    {
        constexpr uint32_t INPUT_LENGTH = 512;
        static char buffer[INPUT_LENGTH] = { 0 };
        static uint32_t i = 0;

        if (i > INPUT_LENGTH)
        {
            i = 0;
        }

        uint32_t data = 0;
        bool received = u.Rx(data);
        if (received)
        {
            if (data == '\n' || data == '\r')
            {
                uint8_t msg[] = "\r\Processing... \r\n";
                Transmit(u, msg, sizeof(msg));

                // process buffer
                const Operations opcode = Process(u, buffer, i);
                i = 0;
            }
            else
            {
                // local echo
                u.Tx(data);
                buffer[i] = static_cast<uint8_t>(data);
                i++;
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

        // Gpio PC(GpioPort::C);
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

            // if (!switchPressed)
            // {
            //     Hello(usart);
            //     // rawData = adc.Read();
            //     // rawData = pollAdc(adc);
            // }

            Receive(usart);

            // count++;
            // uint32_t d = 0;
            // if (count >= LIMIT || usart.Rx(d))
            // {
            //     count = 0;
            //     ledState = !ledState;
            //     PC.WriteOutput(LED_PIN, ledState);
            // }
        }
    }
}
