#include "ssd1306.hpp"

namespace ssd1306
{
    enum CommandCodes
    {
        CMD_STREAM = 0x0,
        DATA_STREAM = 0x40,

        CMD_SET_ADDRESS_MODE = 0x20,
        CMD_SET_CONTRAST = 0x81,

        CMD_DISPLAY_ON_RESUME    = 0xA4,
        CMD_DISPLAY_ON_RESET     = 0xA5,

        CMD_SET_DISPLAY_NORMAL   = 0xA6,
        CMD_SET_DISPLAY_INVERSE  = 0xA7,

        CMD_DISPLAY_OFF          = 0xAE,
        CMD_DISPLAY_ON           = 0xAF,

        // Scrolling commands
        CMD_SCROLL_HORIZONTAL_R  = 0x26, // Params: Dummy 0, Page Start Addr, Time Interval, Page End
        CMD_SCROLL_HORIZONTAL_L  = 0x27, // Params: Dummy 0, Page Start Addr, Time Interval, Page End
        CMD_SCROLL_DEACTIVATE    = 0x2E,
        CMD_SCROLL_ACTIVATE      = 0x2F,

        // Address setting commands
        CMD_SET_LOWER_COL_BASE   = 0x00,
        CMD_SET_HIGHER_COL_BASE  = 0x10,
        CMD_SET_MEMORY_ADDR_MODE = 0x20,
        CMD_SET_COL_ADDRESS      = 0x21, // Params: Col Start, Col End
        CMD_SET_PAGE_ADDRESS     = 0x22, // Params: Page Start, Page End
        
    };

    OledDisplay::OledDisplay(I2C &i2c, uint8_t address) : m_i2c(i2c), m_addr(address)
    {
    }

    void OledDisplay::init()
    {
        // taken from https://gist.github.com/pulsar256/564fda3b9e8fc6b06b89
        m_i2c.tx_start(m_addr);
        m_i2c.tx_data(CMD_STREAM);

        m_i2c.tx_data(CMD_DISPLAY_OFF);

        m_i2c.tx_data(0xD4); // Set Display Clock Divide Ratio / OSC Frequency
        m_i2c.tx_data(0x80); // Display Clock Divide Ratio / OSC Frequency

        m_i2c.tx_data(0xA8); // Set Multiplex Ratio
        m_i2c.tx_data(0x3F); // Multiplex Ratio for 128x64 (64-1)

        m_i2c.tx_data(0xD3); // Set Display Offset
        m_i2c.tx_data(0x00); // Display Offset

        m_i2c.tx_data(0x40); // Set Display Start Line

        m_i2c.tx_data(0x8D); // Set Charge Pump
        m_i2c.tx_data(0x14); // Charge Pump (0x10 External, 0x14 Internal DC/DC)

        m_i2c.tx_data(0xA1); // Set Segment Re-Map
        m_i2c.tx_data(0xC8); // Set Com Output Scan Direction

        m_i2c.tx_data(0xDA); // Set COM Hardware Configuration
        m_i2c.tx_data(0x12); // COM Hardware Configuration

        m_i2c.tx_data(CMD_SET_CONTRAST); // Set Contrast
        m_i2c.tx_data(0xCF); // Contrast

        m_i2c.tx_data(0xD9); // Set Pre-Charge Period
        m_i2c.tx_data(0xF1); // Set Pre-Charge Period (0x22 External, 0xF1 Internal)

        m_i2c.tx_data(0xDB); // Set VCOMH Deselect Level
        m_i2c.tx_data(0x40); // VCOMH Deselect Level

        m_i2c.tx_data(CMD_DISPLAY_ON_RESET); // Set all pixels OFF
        m_i2c.tx_data(CMD_DISPLAY_ON_RESUME); // Set all pixels OFF
        m_i2c.tx_data(CMD_SET_DISPLAY_NORMAL); // Set display not inverted
        m_i2c.tx_data(CMD_DISPLAY_ON); // Set display On

        m_i2c.tx_end();
    }

    void OledDisplay::set_pixel(uint32_t x, uint32_t y, bool state)
    {
    }

    void OledDisplay::refresh()
    {
    }

    void OledDisplay::send_cmd(uint8_t command)
    {
        m_i2c.write_reg(m_addr, CMD_STREAM, command);
    }

    void OledDisplay::scroll_horizontal(ScrollDirection d, ScrollInterval step, uint8_t page_start, uint8_t page_end)
    {
        const uint8_t PARAM_MASK = 0x7;

        m_i2c.tx_start(m_addr);
        m_i2c.tx_data(CMD_STREAM);
        m_i2c.tx_data(uint8_t(CMD_SCROLL_HORIZONTAL_R) | d);
        m_i2c.tx_data(0);                       // Dummy byte
        m_i2c.tx_data(page_start & PARAM_MASK);  // Page Start
        m_i2c.tx_data(step & PARAM_MASK);       // Scroll step
        m_i2c.tx_data(page_end & PARAM_MASK);    // Page End
        m_i2c.tx_data(0x00);                    // Dummy byte
        m_i2c.tx_data(0xFF);                    // Dummy byte

        m_i2c.tx_data(CMD_SCROLL_ACTIVATE);
        m_i2c.tx_end();
    }

    void OledDisplay::set_addressing_mode(AddressMode mode)
    {
        send_cmd(CMD_SET_ADDRESS_MODE);
        send_cmd(static_cast<uint8_t>(mode));
    }

    void OledDisplay::set_col(uint8_t start, uint8_t end)
    {
        send_cmd(CMD_SET_COL_ADDRESS);
        send_cmd(start);
        send_cmd(end);
    }


    void OledDisplay::set_page(uint8_t start, uint8_t end)
    {
        // Maximum of 8 pages total (0-7)
        enum { PAGE_ADDRESS_MASK = 0x7 };

        send_cmd(CMD_SET_PAGE_ADDRESS);
        send_cmd(start & PAGE_ADDRESS_MASK);
        send_cmd(end & PAGE_ADDRESS_MASK);
    }

    void OledDisplay::draw_buffer(const uint8_t *buffer, const uint32_t len)
    {
        m_i2c.tx_start(m_addr);
        m_i2c.tx_data(DATA_STREAM);

        for (uint32_t i = 0; i < len; i++)
        {
            m_i2c.tx_data(buffer[i]);
        }

        m_i2c.tx_end();
    }

    void OledDisplay::update()
    {
        draw_buffer(m_ram, sizeof(m_ram));
    }
}
