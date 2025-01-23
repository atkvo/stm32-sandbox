#pragma once
#include <cstdint>
#include "gpio_interface.hpp"
#include "stm32f411xe.h"

/**
 * @brief Gpio class that abstracts
 * configuration and usage of the GPIO
 * peripheral
 */
class Gpio
{
public:
    /**
     * @brief Construct a new Gpio object
     *
     * @param port - GPIO port to use
     */
    Gpio(GpioPort port);

    /**
     * @brief Set the GPIO mode
     *
     * @param pin
     * @param mode
     */
    void set_mode(uint8_t pin, GpioMode mode);

    /**
     * @brief Set the output type for the specified pin index
     *
     * @param pin
     * @param mode
     */
    void set_output_mode(uint8_t pin, GpioOutputMode mode);

    /**
     * @brief Set the pull up/pull down for specified pin
     *
     * @param pin
     * @param mode
     */
    void set_pupd(uint8_t pin, GpioPuPdMode mode);

    /**
     * @brief Writes a value to the specified pin index
     *
     * @param pin
     * @param state
     */
    void write_pin(uint8_t pin, bool state);

    /**
     * @brief Writes to the entire output register
     *
     * @param reg
     */
    void write_port(uint32_t reg);

    /**
     * @brief Read an input state of the specified pin
     *
     * @param pin
     */
    bool read_pin(uint8_t pin);

    /**
     * @brief Reads the entire input register
     */
    uint32_t read_port();

    /**
     * @brief Configures the pin alternate function mode
     */
    void set_af(uint8_t pin, uint8_t af_mode);

    /**
     * @brief Set the GPIO pin speed
     */
    void set_speed(uint8_t pin, GpioSpeed speed);

    /**
     * @brief Destroy the Gpio object
     *
     */
    ~Gpio();

private:
    volatile GPIO_TypeDef* m_gpio;
    GpioPort m_port;
};
