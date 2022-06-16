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
    void ConfigurePin(Gpio &gpioPort, uint8_t pinIndex);

    /**
     * @brief Reads the ADC data register
     *
     * @return uint32_t
     */
    uint32_t Read();

    /**
     * @brief Check if an ADC conversion is done
     *
     * @return true
     * @return false
     */
    bool IsConversionDone();

    /**
     * @brief Starts the conversion
     *
     */

    void BeginConversion();

    /**
     * @brief Set the ADC resolution
     *
     * @param r
     * @return true if the resolution is supported
     * @return false  if the resolution is not supported
     */
    bool SetResolution(Resolution r);

    static const uint32_t MAX_RESOLUTION;

private:
    Resolution m_ConfiguredResolution;
};
