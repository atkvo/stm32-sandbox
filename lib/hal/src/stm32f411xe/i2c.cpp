#include "i2c.hpp"
#include "stm32f411xe.h"

// anonymous namespace for private functions
namespace
{
    volatile I2C_TypeDef* GetI2C(I2C::PeripheralSelect select)
    {
        static_assert(sizeof(I2C_TypeDef) == 0x400, "hm");

        return reinterpret_cast<volatile I2C_TypeDef*>(I2C1_BASE + (sizeof(I2C_TypeDef) * select));
    }
};

/**
 * @brief Construct a new I2C object
 *
 */
I2C::I2C(PeripheralSelect i2c) : i2cSelect(i2c)
{
    // Enable the peripheral clock
    uint32_t const I2C_ENABLE_OFFSET = RCC_APB1ENR_I2C1EN_Pos + i2cSelect;
    RCC->APB1ENR |= (1 << I2C_ENABLE_OFFSET);
}

/**
 * @brief Destroy the I2C object
 *
 */
I2C::~I2C()
{
    // Disable the peripheral clock
    uint32_t const I2C_ENABLE_OFFSET = RCC_APB1ENR_I2C1EN_Pos + i2cSelect;
    RCC->APB1ENR &= ~(1 << I2C_ENABLE_OFFSET);
}

// The following is the required sequence in master mode.
// • Program the peripheral input clock in I2C_CR2 Register in order to generate correct
// timings
// • Configure the clock control registers
// • Configure the rise time register
// • Program the I2C_CR1 register to enable the peripheral
// • Set the START bit in the I2C_CR1 register to generate a Start condition
// The peripheral input clock frequency must be at least:
// • 2 MHz in Sm mode
// • 4 MHz in Fm mode

/**
 * @brief Enables or disables the I2C peripheral
 *
 * @param enable
 */
void I2C::SetPeripheralEnableState(bool enable)
{
    I2C1_BASE
}

/**
 * @brief Gets the enable state of the I2C peripheral
*
* @return true
* @return false
*/
bool I2C::GetPeripheralEnableState()
{
    uint32_t const I2C_ENABLE_OFFSET = RCC_APB1ENR_I2C1EN_Pos + i2cSelect;
    return (RCC->APB1ENR & (1 << I2C_ENABLE_OFFSET));
}

void I2C::Write(uint8_t id, uint8_t data)
{

}