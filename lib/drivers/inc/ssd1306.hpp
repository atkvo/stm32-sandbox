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

    enum class AddressMode : uint8_t
    {
        HORIZONTAL = 0b00,
        VERTICAL = 0b01,
    };

    // @todo: Add a framebuffer class
    // @todo: Make OledDisplay an interface and the SSD1306 an impl of that interface
    class OledDisplay
    {
    public:
        enum
        {
            TOTAL_COLUMNS = 128,
            TOTAL_PAGES = 8,
            ROWS_PER_PAGE = 8,
        };

        OledDisplay(I2C &i2c, uint8_t address);

        // APIs
        void init();

        void refresh();

        void set_pixel(uint32_t x, uint32_t y, bool state);

        void scroll_horizontal(ScrollDirection d, ScrollInterval step, uint8_t pageStart, uint8_t pageEnd);

        void update();

        void draw_buffer(const uint8_t *buffer, const uint32_t len);

        void set_addressing_mode(AddressMode);

        void set_col(uint8_t start, uint8_t end);

        void set_page(uint8_t start, uint8_t end);

    private:

        I2C &m_i2c;
        uint8_t m_addr;
        uint8_t m_ram[TOTAL_COLUMNS * TOTAL_PAGES]; // 128 x (8*8 = 64)

        void send_cmd(uint8_t cmd);
    };
}
