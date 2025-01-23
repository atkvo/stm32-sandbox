#pragma once
#include "lib/hal/include/gpio_interface.hpp"
#include "gpio.hpp"

namespace gpio_scratch
{
    enum AppMode : bool
    {
        SwitchMode = false,
        BlinkyMode = true
    };

    uint8_t const LED_PIN = 13;        // PC.13
    uint8_t const USER_SWITCH_PIN = 0; // PA.0

    // This can be considered as an "application" using the GPIO
    // interface here. Eventually we should be able to mock the
    // Gpio object out
    template <typename T>
    requires IsGpioPort<T>
    void set_led(T& gpio, bool state)
    {
        // if we want to turn the LED on, write
        // a 0 to the output register since it's
        // an open drain output
        gpio.write_pin(LED_PIN, state ? 0 : 1);
    }

    // Another example of how we can use C++20 concepts feature
    // to have drivers that use interfaces (compile time)
    // without any virtual functions
    template <typename T>
    requires IsGpioPort<T>
    class LedController
    {
    public:
        LedController(T& gpio_port) : m_port(gpio_port) {}

        enum LedState : uint8_t
        {
            ON = 0,
            OFF = 1
        };
        void set(uint8_t state)
        {
            m_port.write_pin(LED_PIN, state);
        }

    private:
        T& m_port;
    };

    inline
    void _delay(const uint32_t cycles)
    {
        for (volatile int i = 0; i < cycles; i++);
    }

    inline void flicker_led(LedController<Gpio>& controller)
    {
        for (uint8_t c = 0; c < 20; c++)
        {
            controller.set(1);
            _delay(100000);
            controller.set(0);
            _delay(100000);
        }
    }
    
    inline void run()
    {

        Gpio PC(GpioPort::C);
        PC.set_mode(LED_PIN, GpioMode::Output);
        PC.set_output_mode(LED_PIN, GpioOutputMode::OpenDrain);

        Gpio PA(GpioPort::A);
        // PA.0 has an ACTIVE LOW button connected
        PA.set_mode(USER_SWITCH_PIN, GpioMode::Input);

        bool const ON = true;
        bool const OFF = false;

        LedController<Gpio> led_controller(PC);

        while(1)
        {
            if (!PA.read_pin(USER_SWITCH_PIN))
            {
                flicker_led(led_controller);

            }

            // set_led(portC, ON);
            led_controller.set(ON);
            _delay(500000);

            // set_led(portC, OFF);
            led_controller.set(OFF);
            _delay(500000);
        }
    }
}
