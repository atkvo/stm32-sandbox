#include "gpio.hpp"
#include "stm32f411xe.h"

static const uint64_t GPIO_PERIPHERAL_SIZE_IN_BYTES = 0x400;
static uint8_t const MAX_PINS_PER_GPIO_REG = 16;

static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::A)) == GPIOA_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::B)) == GPIOB_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::C)) == GPIOC_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::D)) == GPIOD_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::E)) == GPIOE_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::H)) == GPIOH_BASE, "Gpio offset assumptions will not work");

volatile GPIO_TypeDef* getGpio(volatile void* ptr) { return reinterpret_cast<volatile GPIO_TypeDef*>(ptr); }

Gpio::Gpio(GpioPort port) : mPort(port)
{
    mGpioRegister = reinterpret_cast<GPIO_TypeDef*>(GPIOA_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * static_cast<uint8_t>(port)));

    // Enable the clock to the GPIO port
    RCC->AHB1ENR |= (1 << static_cast<uint8_t>(port));
}

Gpio::~Gpio()
{
    // Disable the clock to the GPIO port
    RCC->AHB1ENR &= ~(1 << static_cast<uint8_t>(mPort));
}

void Gpio::SetMode(uint8_t pinIndex, GpioMode mode) 
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
    volatile GPIO_TypeDef* gpio = getGpio(mGpioRegister);
    gpio->MODER = (gpio->MODER & CLEAR_MASK) | (modeBits << MODE_INDEX);
}

void Gpio::SetOutputType(uint8_t pinIndex, GpioOutputType mode) 
{
    const uint8_t CLEAR_MASK = 1 << pinIndex;
    volatile GPIO_TypeDef* gpio = getGpio(mGpioRegister);
    gpio->OTYPER = (gpio->OTYPER & CLEAR_MASK) | (mode << pinIndex);
}

void Gpio::Write(uint8_t pinIndex, bool state) 
{
    const uint8_t CLEAR_MASK = 1 << pinIndex;
    volatile GPIO_TypeDef* gpio = getGpio(mGpioRegister);
    gpio->ODR = (gpio->ODR & CLEAR_MASK) | ((state ? 1 : 0) << pinIndex);
}
