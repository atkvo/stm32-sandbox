#pragma once
#include "stm32f411xe.h"

#include "gpio.hpp"

/**
 * @brief GpioPort 
 * 
 * Enumeration of the available GPIO ports
 * Each enum value is defined as an index of
 * the available GPIO peripherals. This index
 * can be used as a shortcut to caclulate register
 * indices depending on which GPIO is desired
 */
enum class GpioPort : uint8_t
{
    A = 0, 
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    H = 7
};

const uint64_t GPIO_PERIPHERAL_SIZE_IN_BYTES = 0x400;

static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::A)) == GPIOA_BASE, "ur wrong");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::B)) == GPIOB_BASE, "ur wrong");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::C)) == GPIOC_BASE, "ur wrong");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::D)) == GPIOD_BASE, "ur wrong");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::E)) == GPIOE_BASE, "ur wrong");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::H)) == GPIOH_BASE, "ur wrong");

template <GpioPort PORT>
class Gpio : public GpioInterface
{
public: 
    Gpio()
    {
        mGpio = reinterpret_cast<GPIO_TypeDef*>(GPIOA_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * static_cast<uint8_t>(PORT)));
        EnableGpioClock();
    }

    virtual void SetMode(uint8_t pinIndex, GpioMode mode) final
    {
        if (pinIndex > MAX_PINS_PER_GPIO_REG)
        {
            return;
        }

        // Each MODERxx is 2 bits wide
        uint8_t const BITS_PER_MODE_CONTROL = 2;
        uint8_t const MODE_INDEX = pinIndex * BITS_PER_MODE_CONTROL;

        uint8_t const modeBits = static_cast<uint8_t>(mode);

        uint32_t const SET_MASK = (0x3 << MODE_INDEX);
        uint32_t const CLEAR_MASK = ~SET_MASK;

        // AND with the save mask to clear the desired bits to set
        mGpio->MODER = (mGpio->MODER & CLEAR_MASK) | (modeBits << MODE_INDEX);
    }

    virtual void SetOutputType(uint8_t pinIndex, GpioOutputType mode) final
    {
        const uint8_t CLEAR_MASK = 1 << pinIndex;
        mGpio->OTYPER = (mGpio->OTYPER & CLEAR_MASK) | (mode << pinIndex);
    }

    virtual void Write(uint8_t pinIndex, bool state) final
    {
        const uint8_t CLEAR_MASK = 1 << pinIndex;
        mGpio->ODR = (mGpio->ODR & CLEAR_MASK) | ((state ? 1 : 0) << pinIndex);
    }

    virtual uint8_t PinsPerRegister() final { return MAX_PINS_PER_GPIO_REG; }

    virtual ~Gpio()
    {
        // Disable the clock to the GPIO port
        RCC->AHB1ENR &= ~(1 << static_cast<uint8_t>(PORT));
    }

private:
    GPIO_TypeDef volatile *mGpio;
    uint8_t const MAX_PINS_PER_GPIO_REG = 16;

    void EnableGpioClock()
    {
        RCC->AHB1ENR |= (1 << static_cast<uint8_t>(PORT));
    }
};
