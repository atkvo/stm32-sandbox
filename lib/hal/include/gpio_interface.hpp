#pragma once
/**
 * @file gpio.hpp
 * @brief Defines the GPIO interface
 */

#include <stdint.h>
#include <concepts>

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
enum GpioOutputMode
{
    PushPull  = 0x0,
    OpenDrain = 0x1
};

/**
 * @brief Gpio pull up pull down modes
*/
enum class GpioPuPdMode : uint8_t
{
    None     = 0x0,
    Pullup   = 0x1,
    Pulldown = 0x2,
};

/**
 * @brief Gpio speed modes
*/
enum class GpioSpeed : uint8_t
{
    Low    = 0x0,
    Medium = 0x1,
    Fast   = 0x2,
    High   = 0x3,
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

template<typename T>
concept IsGpioPort = requires(T x) {
    { x.set_mode(uint8_t(), GpioMode()) };
    { x.set_output_mode(uint8_t(), GpioOutputMode()) };
    { x.set_pupd(uint8_t(), GpioPuPdMode()) };
    { x.write_pin(uint8_t(), bool()) };
    { x.write_port(uint32_t()) };
    { x.read_pin(uint8_t()) } -> std::convertible_to<bool>;
    { x.read_port() } -> std::convertible_to<uint32_t>;
};
