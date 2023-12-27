#include <cstdint>

#include "i2c.hpp"
#include "stm32f411xe.h"

extern uint8_t __counter;

static volatile I2C_TypeDef* GetI2C(I2CId id)
{
    const uint32_t OFFSET_LEN = 0x400;
    const uint32_t I2C_OFFSET = OFFSET_LEN * static_cast<uint8_t>(id);
    return reinterpret_cast<volatile I2C_TypeDef*>(I2C1_BASE + I2C_OFFSET);
}

static inline
void __i2c_start(volatile I2C_TypeDef* i2c)
{
    i2c->CR1 |= I2C_CR1_START;
    while (!(i2c->SR1 & I2C_SR1_SB));
}

static inline
void __i2c_stop(volatile I2C_TypeDef* i2c)
{
    i2c->CR1 |= I2C_CR1_STOP;
    while (!(i2c->SR2 & I2C_SR2_BUSY));
}

static inline
void __i2c_addr(volatile I2C_TypeDef *i2c, uint8_t address)
{
    i2c->DR = address << 1;
    while (!(i2c->SR1 & I2C_SR1_ADDR));
    volatile uint8_t tmp = i2c->SR1 | i2c->SR2;    // dummy read to clear addr bit
}

static inline
void __i2c_write(volatile I2C_TypeDef* i2c, uint8_t data)
{
    while (!(i2c->SR1 & (I2C_SR1_TXE)));
    i2c->DR = data;
    while (!(i2c->SR1 & (I2C_SR1_BTF)));
}

I2C::I2C(I2CId id, Gpio &gpio, uint8_t sda, uint8_t scl) : mId(id)
{
    // Turn on I2C clock
    const uint8_t RCC_I2C_BASE_BIT_INDEX = 21;
    const uint8_t RCC_I2C_BIT_INDEX = RCC_I2C_BASE_BIT_INDEX + static_cast<uint8_t>(id);
    RCC->APB1ENR |= (1 << RCC_I2C_BIT_INDEX);

    // Configure GPIO pins
    gpio.SetMode(sda, GpioMode::Alternate);
    gpio.SetMode(scl, GpioMode::Alternate);

    // @todo: Whose responsibility is it configure
    // the AF mode for the I2C tx/rx GPIO pins?
    const uint8_t I2C_AF_MODE = 4;
    gpio.SetAfMode(sda, I2C_AF_MODE);
    gpio.SetAfMode(scl, I2C_AF_MODE);

    gpio.SetOutputType(sda, GpioOutputType::OpenDrain);
    gpio.SetOutputType(scl, GpioOutputType::OpenDrain);

    gpio.SetSpeed(sda, GpioSpeed::Fast);
    gpio.SetSpeed(scl, GpioSpeed::Fast);

    gpio.SetPullUpPullDown(sda, GpioPuPdMode::Pullup);
    gpio.SetPullUpPullDown(scl, GpioPuPdMode::Pullup);

    volatile I2C_TypeDef* i2c = GetI2C(id);

    i2c->CR1 |= (I2C_CR1_SWRST);
    i2c->CR1 &= ~(I2C_CR1_SWRST);

    i2c->CR1 &= ~(I2C_CR1_PE);    // Disable peripheral before configuring clock

    // configure clock
    i2c->CR2 &= ~(I2C_CR2_FREQ_Msk);
    const uint32_t freq_mhz = SystemCoreClock / 1000000; // convert to MHz
    i2c->CR2 |= freq_mhz << I2C_CR2_FREQ_Pos;

    // get nanosecond period
    // period in mhz = us
    // 1000 ns / us
    // period_ns = 1000 (ns/us) * (f us) = (1000 * f) ns
    const uint32_t PERIOD_NS = (1000) / freq_mhz;

    // See datasheet for details (Section 18.6.8 (I2C_CCR))
    const uint32_t tRSCL_NS = 300;
    const uint32_t tWSCLL_NS = 4000;
    // CCR = trSCL + twSCLH / tPCLK1
    const uint32_t ccr = (tRSCL_NS + tWSCLL_NS ) / PERIOD_NS;

    i2c->CCR =  ccr & I2C_CCR_CCR_Msk;
    i2c->TRISE = (tRSCL_NS / PERIOD_NS) + 1;
}

void I2C::WriteReg(uint8_t address, uint8_t reg, uint8_t data)
{
    volatile I2C_TypeDef *i2c = GetI2C(mId);
    __i2c_start(i2c);
    __i2c_addr(i2c, address);
    __i2c_write(i2c, reg);
    __i2c_write(i2c, data);
    __i2c_stop(i2c);
}

void I2C::WriteByte(uint8_t address, uint8_t data)
{
    volatile I2C_TypeDef *i2c = GetI2C(mId);
    __i2c_start(i2c);
    __i2c_addr(i2c, address);
    __i2c_write(i2c, data);
    __i2c_stop(i2c);
}

void I2C::WriteStream(uint8_t address, const uint8_t *data, uint32_t bytes)
{
    volatile I2C_TypeDef *i2c = GetI2C(mId);
    __i2c_start(i2c);
    __i2c_addr(i2c, address);

    for (uint32_t i = 0; i < bytes; i++)
    {
        __i2c_write(i2c, data[i]);
    }
    __i2c_stop(i2c);
}

uint8_t I2C::ReadByte(uint8_t address)
{
    uint8_t data = 0xFF;

    volatile I2C_TypeDef *i2c = GetI2C(mId);

    // Wait for I2C to come out of busy
    while (i2c->SR2 & I2C_SR2_BUSY) { };

    __i2c_start(i2c);

    i2c->DR = address << 1;

    (void)i2c->SR2; // dummy read to clear address flag

    return data;
}

void I2C::Enable()
{
    GetI2C(mId)->CR1 |= I2C_CR1_PE;
}

void I2C::Disable()
{
    GetI2C(mId)->CR1 &= ~I2C_CR1_PE;
}
