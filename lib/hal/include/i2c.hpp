#pragma once

#include <cstdint>

/**
 * @brief I2C class
 *
 */
class I2C
{
public:
    enum PeripheralSelect
    {
        I2C1 = 0,
        I2C2 = 1,
        I2C3 = 2
    };

    enum DeviceMode
    {
        Slave,
        Master
    };

    /**
     * @brief Construct a new I2C object
     *
     */
    I2C(PeripheralSelect i2c);

    /**
     * @brief Destroy the I2C object
     *
     */
    ~I2C();

    /**
     * @brief Enables or disables the I2C peripheral
     *
     * @param enable
     */
    void SetPeripheralEnableState(bool enable);

    /**
     * @brief Gets the enable state of the I2C peripheral
     *
     * @return true
     * @return false
     */
    bool GetPeripheralEnableState();

    /**
     * @brief Set device mode
     * @param m - device mode (master/slave)
     */
    void SetDeviceMode(DeviceMode m);

    /**
     * @brief Get the device mode
     *
     * @return DeviceMode
     */
    DeviceMode GetDeviceMode()
    {
        return transmitterMode;
    }

    /**
     * @brief Writes a data stream to the specified device + address
     *
     * @param id        target device ID
     * @param data      pointer to data to write
     * @param len       length of data to write in bytes
     */
    void Write(uint8_t id, uint8_t *data, uint32_t len);


    /**
     * @brief Writes a data stream to the specified device + address
     *
     * @param id        target device ID
     * @param data      byte to write
     */
    void Write(uint8_t id, uint8_t data);

    /**
     * @brief Writes a data stream to the specified device + address
     *
     * @param id        target device ID
     * @param rw        read/write mode
     */

    /**
     * @brief Reads a byte from the specified device
     *
     * @param id
     * @return uint8_t
     */
    uint8_t Read(uint8_t id);

    /**
     * @brief Writes a data stream to the specified device + address
     *
     * @param id        target device ID
     * @param data      pointer to read data to
     * @param data      number of bytes to read
     */
    void Read(uint8_t id, uint8_t *data, uint32_t len);

private:

    DeviceMode       transmitterMode;
    PeripheralSelect i2cSelect;
};
