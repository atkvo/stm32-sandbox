#pragma once
#include <stdint.h>
#include "hal/include/i2c.hpp"

namespace ssd1306
{
    enum ScrollDirection
    {
        SCROLL_RIGHT = 0,
        SCROLL_LEFT = 1,
    };

    enum ScrollInterval
    {
        SCROLL_INTERVAL_5   = 0x0,
        SCROLL_INTERVAL_64  = 0x1,
        SCROLL_INTERVAL_128 = 0x2,
        SCROLL_INTERVAL_256 = 0x3,
        SCROLL_INTERVAL_25  = 0x4,
        SCROLL_INTERVAL_3   = 0x5,
        SCROLL_INTERVAL_4   = 0x6,
        SCROLL_INTERVAL_2   = 0x7,
    };

    class OledDisplay
    {
    public:
        OledDisplay(I2C &i2c, uint8_t address);

        // APIs
        void init();

        void refresh();

        void set_pixel(uint32_t x, uint32_t y, bool state);

        void scroll_horizontal(ScrollDirection d, ScrollInterval step, uint8_t pageStart, uint8_t pageEnd);

    private:
        I2C &m_i2c;
        uint8_t m_addr;
        // uint32_t m_ram[4][2]; // 128 x 64  - maybe better not to optimize for space until ready
        uint8_t m_ram[128][64]; // 128 x 64  - unoptimized

        void send_cmd(uint8_t cmd);
    };
}
