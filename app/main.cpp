#include <cstdint>
#include "system_stm32f4xx.h"
#include "stm32f4xx.h"

#include "scratch/gpio_scratch.hpp"
#include "scratch/adc_scratch.hpp"

int main()
{
    SystemInit();

    // gpio_scratch::run();
    adc_scratch::run();

    return 0;
}
