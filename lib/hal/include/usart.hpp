#pragma once

#include <stdint.h>

enum class UsartNum
{
    USART_1, 
    USART_2,
    USART_6
};

/**
 * @brief Class to manage the USART peripheral
 *
 */
class Usart
{
public:
    /**
     * @brief Construct a new USART object
     *
     */
    Usart(const UsartNum u);

    /**
     * @brief Destroy the USART object
     *
     */
    ~Usart();

    /**
     * @brief Enables the USART at the specified frequency
     * 
     * @todo API is barebones at the moment. Need to add support
     * for other configuration parameters like parity, stop bits, etc.
     * 
     * @param baudRate 
     */
    void Configure(uint32_t baudRate);

    /**
     * @brief Transmits data over the uart (blocking)
     * 
     * @param data 
     * @return true 
     * @return false 
     */
    bool Tx(uint32_t data);

    /**
     * @brief Attempts to receive data over the uart
     * 
     * @param data 
     * @return true if data was received
     * @return false if no data was received
     */
    bool Rx(uint32_t &data);

private:
    void *mUsart = nullptr;
};
