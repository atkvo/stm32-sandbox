#pragma once
/**
 * @file gpio.hpp
 * @brief Defines the GPIO interface
 */

#include <cstdint>

/**
 * @brief Gpio mode
 */
enum GpioMode
{
    Input       = 0x0,
    Output      = 0x1,
    Alternate   = 0x2,
    Analog      = 0x3
};

/**
 * @brief Gpio output types
 */
enum GpioOutputType
{
    PushPull  = 0x0,
    OpenDrain = 0x1
};

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
     * @param pinIndex 
     * @param mode 
     */
    void SetMode(uint8_t pinIndex, GpioMode mode);

    /**
     * @brief Set the output type for the specified pin index
     * 
     * @param pinIndex 
     * @param mode 
     */
    void SetOutputType(uint8_t pinIndex, GpioOutputType mode);

    /**
     * @brief Writes a value to the specified pin index
     * 
     * @param pinIndex 
     * @param state 
     */
    void WriteOutput(uint8_t pinIndex, bool state);

    /**
     * @brief Writes to the entire output register
     * 
     * @param register 
     */
    void WriteOutputRegister(uint32_t register);

    /**
     * @brief Read an input state of the specified pin
     * 
     * @param pinIndex 
     */
    bool ReadInput(uint8_t pinIndex);

    /**
     * @brief Reads the entire input register
     */
    uint32_t ReadInputRegister();

    /**
     * @brief Destroy the Gpio object
     * 
     */
    ~Gpio();

private:
    // @todo: is there a better way for this?
    void volatile *mGpioRegister;
    GpioPort mPort;
};