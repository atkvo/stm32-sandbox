#include "gpio.hpp"
#include "stm32f411xe.h"

enum
{
    GPIO_PERIPHERAL_SIZE_IN_BYTES = 0x400,
    MAX_PINS_PER_GPIO_REG = 16,
};

static volatile GPIO_TypeDef* get_gpio_ptr(GpioPort port)
{
    return reinterpret_cast<GPIO_TypeDef*>(GPIOA_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * static_cast<uint8_t>(port)));
}

static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::A)) == GPIOA_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::B)) == GPIOB_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::C)) == GPIOC_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::D)) == GPIOD_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::E)) == GPIOE_BASE, "Gpio offset assumptions will not work");
static_assert((AHB1PERIPH_BASE + (GPIO_PERIPHERAL_SIZE_IN_BYTES * (int)GpioPort::H)) == GPIOH_BASE, "Gpio offset assumptions will not work");

Gpio::Gpio(GpioPort port) : m_gpio(get_gpio_ptr(port)), m_port(port)
{
    RCC->AHB1ENR |= (1 << static_cast<uint8_t>(port));
}

void Gpio::set_mode(uint8_t pin, GpioMode mode)
{
    if (pin >= MAX_PINS_PER_GPIO_REG)
    {
        return;
    }

    // Each MODERxx is 2 bits wide
    uint8_t const BITS_PER_MODE_CONTROL = 2;
    uint8_t const MODE_INDEX = pin * BITS_PER_MODE_CONTROL;

    uint8_t const mode_bits = static_cast<uint8_t>(mode);

    uint32_t const SET_MASK = (0x3 << MODE_INDEX);
    uint32_t const CLEAR_MASK = ~SET_MASK;

    // AND with the save mask to clear the desired bits to set
    m_gpio->MODER = (m_gpio->MODER & CLEAR_MASK) | (mode_bits << MODE_INDEX);
}

void Gpio::set_output_mode(uint8_t pin, GpioOutputMode mode)
{
    const uint8_t MASK = ~(1 << pin);
    m_gpio->OTYPER = (m_gpio->OTYPER & MASK) | (mode << pin);
}

void Gpio::set_pupd(uint8_t pin, GpioPuPdMode mode)
{
    uint8_t const BITS_PER_MODE_CONTROL = 2;
    uint8_t const MODE_INDEX = pin * BITS_PER_MODE_CONTROL;

    uint8_t const MODE_BITS = static_cast<uint8_t>(mode);

    uint32_t const SET_MASK = (0x3 << MODE_INDEX);
    uint32_t const CLEAR_MASK = ~SET_MASK;

    m_gpio->PUPDR &= CLEAR_MASK;
    m_gpio->PUPDR |= (MODE_BITS << MODE_INDEX);
}

void Gpio::write_pin(uint8_t pin, bool state)
{
    const uint8_t MASK = ~(1 << pin);
    m_gpio->ODR = (m_gpio->ODR & MASK) | ((state ? 1 : 0) << pin);
}

void Gpio::write_port(uint32_t reg)
{
    m_gpio->ODR = reg;
}

bool Gpio::read_pin(uint8_t pin)
{
    return (m_gpio->IDR & (1 << pin)) ? true : false;
}

uint32_t Gpio::read_port()
{
    return m_gpio->IDR;
}

void Gpio::set_af(uint8_t pin, uint8_t af_mode)
{
    const uint8_t PINS_PER_AFR = 8;
    volatile uint32_t &AFR_REG = (pin < PINS_PER_AFR) ? m_gpio->AFR[0] : m_gpio->AFR[1];

    const uint8_t AFR_MASK     = 0xF; // 4 bits
    const uint8_t AFR_INDEX    = pin % PINS_PER_AFR;
    const uint8_t AFR_BITS_PER_PIN = 4;
    const uint8_t AFR_POS      = AFR_INDEX * AFR_BITS_PER_PIN;

    // clear then set
    AFR_REG &= ~(AFR_MASK << AFR_POS);
    AFR_REG |= (af_mode & AFR_MASK) << (AFR_POS);
}

void Gpio::set_speed(uint8_t pin, GpioSpeed speed)
{
    const uint8_t MASK = 0x11; // each map is 2 bits wide
    const uint8_t BITS_PER_PIN = 2;
    m_gpio->OSPEEDR &= ~(MASK << (pin * BITS_PER_PIN));
    m_gpio->OSPEEDR |= ~((static_cast<uint8_t>(speed) & MASK) << (pin * BITS_PER_PIN));
}

Gpio::~Gpio()
{
    RCC->AHB1ENR &= ~(1 << static_cast<uint8_t>(m_port));
}
