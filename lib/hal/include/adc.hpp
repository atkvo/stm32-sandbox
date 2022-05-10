#pragma once

#include "util.hpp"

#include <cstdint>


class Adc
{
public:
    enum Event
    {
        ADC_EVENT_READ_DONE,
        ADC_TOTAL_EVENTS,
    };

    enum Resolution
    {
        ADC_RESOLUTION_6_BIT  = 0x003F, // 0000 0000 0011 1111
        ADC_RESOLUTION_8_BIT  = 0x00FF, // 0000 0000 1111 1111
        ADC_RESOLUTION_10_BIT = 0x03FF, // 0000 0011 1111 1111
        ADC_RESOLUTION_12_BIT = 0x0FFF, // 0000 1111 1111 1111
    };

    /**
     * @brief
     *
     * Note: This listener will be notified from the IRQ context
     * It's not a good idea to do too much in there. Set a flag instead.
     *
     * @param e
     * @param lis
     */
    void RegisterListener(Event e, Listener &lis);

    uint32_t Read();

    void ConfigureDmaAddress(uint32_t *data);

    bool SetResolution(Resolution r);

    void SetEnableState(bool state);

    static const uint32_t MAX_RESOLUTION;

private:
    uint32_t *m_DmaAddress = nullptr;
    Resolution m_ConfiguredResolution;
};
