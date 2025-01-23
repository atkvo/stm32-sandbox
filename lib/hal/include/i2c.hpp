#pragma once
/**
 * @file i2c.hpp
 * @brief Defines the I2C interface
 */

#include <cstdint>
#include "gpio_interface.hpp"
#include "stm32f411xe.h"

/**
 * @brief Gpio mode
 */
enum class I2CMode
{
    SlaveTx,
    SlaveRx,
    MasterTx,
    MasterRx
};

enum class I2CId
{
    I2C_1,
    I2C_2,
    I2C_3
};

class I2C
{
public:
    template <typename T>
    requires IsGpioPort<T>
    I2C(I2CId id, T &gpio, uint8_t sda, uint8_t scl)
    {
        // Turn on I2C clock
        const uint8_t RCC_I2C_BASE_BIT_INDEX = 21;
        const uint8_t RCC_I2C_BIT_INDEX = RCC_I2C_BASE_BIT_INDEX + static_cast<uint8_t>(id);
        RCC->APB1ENR |= (1 << RCC_I2C_BIT_INDEX);

        // Configure GPIO pins
        gpio.set_mode(sda, GpioMode::Alternate);
        gpio.set_mode(scl, GpioMode::Alternate);

        // @todo: Whose responsibility is it configure
        // the AF mode for the I2C tx/rx GPIO pins?
        const uint8_t I2C_AF_MODE = 4;
        gpio.set_af(sda, I2C_AF_MODE);
        gpio.set_af(scl, I2C_AF_MODE);

        gpio.set_output_mode(sda, GpioOutputMode::OpenDrain);
        gpio.set_output_mode(scl, GpioOutputMode::OpenDrain);

        gpio.set_speed(sda, GpioSpeed::Fast);
        gpio.set_speed(scl, GpioSpeed::Fast);

        gpio.set_pupd(sda, GpioPuPdMode::Pullup);
        gpio.set_pupd(scl, GpioPuPdMode::Pullup);

        m_i2c = get_i2c(id);

        m_i2c->CR1 |= (I2C_CR1_SWRST);
        m_i2c->CR1 &= ~(I2C_CR1_SWRST);

        m_i2c->CR1 &= ~(I2C_CR1_PE);    // Disable peripheral before configuring clock

        // configure clock
        m_i2c->CR2 &= ~(I2C_CR2_FREQ_Msk);
        const uint32_t freq_mhz = SystemCoreClock / 1000000; // convert to MHz
        m_i2c->CR2 |= freq_mhz << I2C_CR2_FREQ_Pos;

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

        m_i2c->CCR =  ccr & I2C_CCR_CCR_Msk;
        m_i2c->TRISE = (tRSCL_NS / PERIOD_NS) + 1;
    }

    template <typename T>
    requires IsGpioPort<T>
    void ConfigurePin(T &gpio_port, uint8_t pinIndex)
    {
        gpio_port.set_mode(pinIndex, GpioMode::Analog);
        ADC1->SQR3 |= ADC_SQR3_SQ1_0;
    }

    void enable();

    void disable();

    void write_reg(uint8_t address, uint8_t reg, uint8_t data);

    void write_byte(uint8_t address, uint8_t data);

    void write_stream(uint8_t address, const uint8_t *data, uint32_t bytes);

    uint8_t read_byte(uint8_t address);

private:
    volatile I2C_TypeDef *m_i2c;

    inline
    void __i2c_start()
    {
        m_i2c->CR1 |= I2C_CR1_START;
        while (!(m_i2c->SR1 & I2C_SR1_SB));
    }

    inline
    void __i2c_stop()
    {
        m_i2c->CR1 |= I2C_CR1_STOP;
        while (!(m_i2c->SR2 & I2C_SR2_BUSY));
    }

    inline
    void __i2c_addr(uint8_t address)
    {
        m_i2c->DR = address << 1;
        while (!(m_i2c->SR1 & I2C_SR1_ADDR));
        volatile uint8_t tmp = m_i2c->SR1 | m_i2c->SR2;    // dummy read to clear addr bit
    }

    inline
    void __i2c_write(uint8_t data)
    {
        while (!(m_i2c->SR1 & (I2C_SR1_TXE)));
        m_i2c->DR = data;
        while (!(m_i2c->SR1 & (I2C_SR1_BTF)));
    }

    static volatile I2C_TypeDef* get_i2c(I2CId id)
    {
        const uint32_t OFFSET_LEN = 0x400;
        const uint32_t I2C_OFFSET = OFFSET_LEN * static_cast<uint8_t>(id);
        return reinterpret_cast<volatile I2C_TypeDef*>(I2C1_BASE + I2C_OFFSET);
    }

};
