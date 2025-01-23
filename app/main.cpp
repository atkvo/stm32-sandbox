#include <cstdint>
#include "system_stm32f4xx.h"

enum class DemoMode
{
    ADCApp,
    GPIOApp,
    I2CApp,
    USARTApp
};

// constexpr DemoMode DEMO = DemoMode::GPIOApp;
constexpr DemoMode DEMO = DemoMode::GPIOApp;

#include "scratch/adc_scratch.hpp"
#include "scratch/gpio_scratch.hpp"
#include "scratch/i2c_scratch.hpp"
#include "scratch/usart_scratch.hpp"

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
            default:
                return gpio_scratch::run();
        }
    }; 

    run_demo();

    return 0;
}
