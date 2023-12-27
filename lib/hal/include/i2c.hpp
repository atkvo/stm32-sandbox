#pragma once
/**
 * @file i2c.hpp
 * @brief Defines the I2C interface
 */

#include <cstdint>
#include "gpio.hpp"

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
    I2C(I2CId id, Gpio &gpioGroup, uint8_t sda, uint8_t scl);

    void Enable();

    void Disable();

    void WriteReg(uint8_t address, uint8_t reg, uint8_t data);

    void WriteByte(uint8_t address, uint8_t data);

    void WriteStream(uint8_t address, const uint8_t *data, uint32_t bytes);

    uint8_t ReadByte(uint8_t address);

    const I2CId mId;
};
