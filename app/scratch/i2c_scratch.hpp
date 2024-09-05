#pragma once

#include "gpio.hpp"
#include "lib/hal/include/usart.hpp"
#include "lib/hal/include/i2c.hpp"

extern uint32_t SystemCoreClock;

namespace i2c_scratch
{
    static I2C *g_i2c = nullptr;
    enum Operations
    {
        LED_OFF = 0,
        LED_ON,
        HELLO
    };

    enum OledCmd : uint8_t
    {
        OLED_CMD_SCREEN_ON = 0xAF,
        OLED_CMD_SCREEN_OFF = 0xAE,
    };

    enum SSD1306_Commands
    {
        PIXELS_OFF = 0xA4,

        DISPLAY_OFF = 0xAE,
        DISPLAY_ON = 0xAF,
    };

    void ssd1306Init();

    void i2c_OLED_send_cmd(uint8_t cmd)
    {
        enum
        {
            OLED_MONOCHROME = 0x3D,
            OELD_TWO_TONE   = 0x3C
        };

        const uint8_t OLED_ADDRESS = OLED_MONOCHROME;
        // const uint8_t CMD_BYTES[] = { 0x0, cmd };

        // Both methods work
        g_i2c->write_reg(OLED_ADDRESS, 0x0, cmd);
    }

    void run() {
        Gpio PB(GpioPort::B);
        PB.set_output_mode(5, GpioOutputMode::PushPull);
        PB.set_mode(5, GpioMode::Output);

        I2C i2c1(I2CId::I2C_1, PB, 6, 7);
        i2c1.enable();
        g_i2c = &i2c1;

        ssd1306Init();

        Gpio PC(GpioPort::C);
        enum { LED_PIN = 13 };
        PC.set_mode(LED_PIN, GpioMode::Output);
        PC.set_output_mode(LED_PIN, GpioOutputMode::OpenDrain);
        while (1)
        {
            i2c_OLED_send_cmd(0xA6); // set normal mode
            PC.write_pin(LED_PIN, 1);
            for (volatile int i = 0; i < 500000; i++); // arbitrary delay

            PC.write_pin(LED_PIN, 0);
            i2c_OLED_send_cmd(0xA7); // set inverse mode
            i2c_OLED_send_cmd(DISPLAY_ON);
            for (volatile int i = 0; i < 500000; i++); // arbitrary delay
        };
    }


    void ssd1306Init(void)
    {
        // taken from https://gist.github.com/pulsar256/564fda3b9e8fc6b06b89
        i2c_OLED_send_cmd(0xAE); // Set display OFF

        i2c_OLED_send_cmd(0xD4); // Set Display Clock Divide Ratio / OSC Frequency
        i2c_OLED_send_cmd(0x80); // Display Clock Divide Ratio / OSC Frequency

        i2c_OLED_send_cmd(0xA8); // Set Multiplex Ratio
        i2c_OLED_send_cmd(0x3F); // Multiplex Ratio for 128x64 (64-1)

        i2c_OLED_send_cmd(0xD3); // Set Display Offset
        i2c_OLED_send_cmd(0x00); // Display Offset

        i2c_OLED_send_cmd(0x40); // Set Display Start Line

        i2c_OLED_send_cmd(0x8D); // Set Charge Pump
        i2c_OLED_send_cmd(0x14); // Charge Pump (0x10 External, 0x14 Internal DC/DC)

        i2c_OLED_send_cmd(0xA1); // Set Segment Re-Map
        i2c_OLED_send_cmd(0xC8); // Set Com Output Scan Direction

        i2c_OLED_send_cmd(0xDA); // Set COM Hardware Configuration
        i2c_OLED_send_cmd(0x12); // COM Hardware Configuration

        i2c_OLED_send_cmd(0x81); // Set Contrast
        i2c_OLED_send_cmd(0xCF); // Contrast

        i2c_OLED_send_cmd(0xD9); // Set Pre-Charge Period
        i2c_OLED_send_cmd(0xF1); // Set Pre-Charge Period (0x22 External, 0xF1 Internal)

        i2c_OLED_send_cmd(0xDB); // Set VCOMH Deselect Level
        i2c_OLED_send_cmd(0x40); // VCOMH Deselect Level

        i2c_OLED_send_cmd(0xA4); // Set all pixels OFF
        i2c_OLED_send_cmd(0xA7); // Set display not inverted
        i2c_OLED_send_cmd(0xAF); // Set display On

        // i2c_OLED_clear_display();
    }
} // namespace i2c_scratch
