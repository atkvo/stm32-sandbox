#include <cstdint>

#include "i2c.hpp"
#include "stm32f411xe.h"

void I2C::write_reg(uint8_t address, uint8_t reg, uint8_t data)
{
    _i2c_start();
    _i2c_addr(address);
    _i2c_write(reg);
    _i2c_write(data);
    _i2c_stop();
}

void I2C::write_byte(uint8_t address, uint8_t data)
{
    _i2c_start();
    _i2c_addr(address);
    _i2c_write(data);
    _i2c_stop();
}

void I2C::write_stream(uint8_t address, const uint8_t *data, uint32_t bytes)
{
    _i2c_start();
    _i2c_addr(address);

    for (uint32_t i = 0; i < bytes; i++)
    {
        _i2c_write(data[i]);
    }
    _i2c_stop();
}

uint8_t I2C::read_byte(uint8_t address)
{
    uint8_t data = 0xFF;

    // Wait for I2C to come out of busy
    while (m_i2c->SR2 & I2C_SR2_BUSY) { };

    _i2c_start();

    m_i2c->DR = address << 1;

    (void)m_i2c->SR2; // dummy read to clear address flag

    return data;
}

void I2C::enable()
{
    m_i2c->CR1 |= I2C_CR1_PE;
}

void I2C::disable()
{
    m_i2c->CR1 &= ~I2C_CR1_PE;
}
