#include "stm32f4xx.h"
#include "system_stm32f4xx.h"
#include <cstdint>

#include "scratch/adc_scratch.hpp"
#include "scratch/gpio_scratch.hpp"
#include "scratch/i2c_scratch.hpp"
#include "scratch/usart_scratch.hpp"

uint8_t __counter = 0;
int main() {
  SystemInit();
  SystemCoreClockUpdate();

  // gpio_scratch::run();
  // adc_scratch::run();
  // usart_scrath::run();
  i2c_scratch::run();

  return 0;
}
