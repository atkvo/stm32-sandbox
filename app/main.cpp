#include <cstdint>
#include "system_stm32f4xx.h"

enum class DemoMode
{
    ADCApp,
    GPIOApp,
    I2CApp,
    USARTApp,
    OLEDApp
};

// constexpr DemoMode DEMO = DemoMode::GPIOApp;
constexpr DemoMode DEMO = DemoMode::OLEDApp;

#include "scratch/adc_scratch.hpp"
#include "scratch/gpio_scratch.hpp"
#include "scratch/i2c_scratch.hpp"
#include "scratch/usart_scratch.hpp"
#include "lib/drivers/inc/ssd1306.hpp"
#include "lib/hal/include/i2c.hpp"

template<uint16_t cycles>
inline void delay_cycles()
{
    for (volatile uint16_t i = 0; i < cycles; i++);
}

void ssd1306_demo() 
{
    Gpio PB(GpioPort::B);
    PB.set_output_mode(5, GpioOutputMode::PushPull);
    PB.set_mode(5, GpioMode::Output);

    I2C i2c1(I2CId::I2C_1, PB, 6, 7);
    i2c1.enable();

    const uint8_t OLED_ADDRESS = 0x3D;
    ssd1306::OledDisplay oled(i2c1, OLED_ADDRESS);

    oled.init();

    delay_cycles<1000>();

    oled.scroll_horizontal(
        ssd1306::SCROLL_RIGHT, 
        ssd1306::SCROLL_INTERVAL_3,
        0,
        6);


    while (1)
    {
    };
}

int main() {
    SystemInit();
    SystemCoreClockUpdate();

    constexpr auto run_demo = []() -> void
    {
        switch (DEMO)
        {
            case DemoMode::ADCApp:
                return adc_scratch::run();
            case DemoMode::GPIOApp:
                return gpio_scratch::run();
            case DemoMode::I2CApp:
                return i2c_scratch::run();
            case DemoMode::USARTApp:
                return usart_scratch::run();
            case DemoMode::OLEDApp:
                return ssd1306_demo();
            default:
                return gpio_scratch::run();
        }
    }; 

    run_demo();

    return 0;
}
