#pragma once

#include "util.hpp"
#include "gpio.hpp"

#include <cstdint>

/**
 * @brief Class to manage the ADC peripheral
 *
 * todo:
 *  - Add DMA support
 *  - Add interrupt support
 *  - Add multiple channel read support
 *
 */
class Adc
{
public:

    enum Resolution
    {
        ADC_RESOLUTION_6_BIT  = 0x003F, // 0000 0000 0011 1111
        ADC_RESOLUTION_8_BIT  = 0x00FF, // 0000 0000 1111 1111
        ADC_RESOLUTION_10_BIT = 0x03FF, // 0000 0011 1111 1111
        ADC_RESOLUTION_12_BIT = 0x0FFF, // 0000 1111 1111 1111
    };

    /**
     * @brief Construct a new Adc object
     *
     */
    Adc();

    /**
     * @brief Destroy the Adc object
     *
     */
    ~Adc();

    /**
     * @brief Configures the GPIO pin for analog input
     *
     * @param gpioPort
     * @param pinIndex
     */
    template <typename T>
    requires IsGpioPort<T>
    void configure_pin(T &gpio_port, uint8_t pinIndex)
    {
        gpio_port.set_mode(pinIndex, GpioMode::Analog);
        ADC1->SQR3 |= ADC_SQR3_SQ1_0;
    }


    /**
     * @brief Reads the ADC data register
     *
     * @return uint32_t
     */
    uint32_t read();

    /**
     * @brief Check if an ADC conversion is done
     *
     * @return true
     * @return false
     */
    bool is_conversion_done();

    /**
     * @brief Starts the conversion
     *
     */

    void start_conversion();

    /**
     * @brief Set the ADC resolution
     *
     * @param r
     * @return true if the resolution is supported
     * @return false  if the resolution is not supported
     */
    bool set_resolution(Resolution r);

    static const uint32_t MAX_RESOLUTION;

private:
    Resolution m_configured_resolution;
};
