#include "stm32f411xe.h"
#include "usart.hpp"


static volatile USART_TypeDef *getUsart(void *p)
{
    return reinterpret_cast<USART_TypeDef*>(p);
}

Usart::Usart(const UsartNum u)
{
    USART_TypeDef *usart = nullptr;

    switch (u)
    {
        case UsartNum::USART_1:
            usart = USART1;
            RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
            break;
        case UsartNum::USART_2:
            usart = USART2;
            RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
            break;
        case UsartNum::USART_6:
            usart = USART6;
            RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
            break;
    }

    if (usart == nullptr)
    {
        /* error! */
        return;
    }
    else
    {
        mUsart = usart;
    }
}

Usart::~Usart()
{

}

void Usart::Configure(uint32_t baudRate)
{
    volatile USART_TypeDef *u = getUsart(mUsart);

    // Formula for calculating baudrate:
    //
    //     baudrate = freq / (8 * (2 - OVER8) * USARTDIV)
    //
    //     where OVER8 = oversampling enabled (1 to enable)
    //           freq  = system clock frequency
    //
    // Use the above formula to calculate the USARTDIV to program
    // into the respective registers (Fractional and Mantissa separately)
    //
    //     USARTDIV = freq / (8 * (2 - OVER8) * baudrate)
    //
    const uint32_t UARTDIV = SystemCoreClock / baudRate;

    // Divide by 16 == (8 * (2 - OVER8)), OVER8 = 1
    const uint32_t BRR_VAL = ((UARTDIV/16) << USART_BRR_DIV_Mantissa_Pos) |
        (((UARTDIV % 16) << USART_BRR_DIV_Fraction_Pos) & USART_BRR_DIV_Fraction_Msk);

    // Clear control register
    u->CR1 = 0;

    // Enable usart, transmitter, receiver
    const uint32_t CR1_VAL = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
    u->BRR = BRR_VAL;
    u->CR1 |= CR1_VAL;
}

bool Usart::Tx(uint32_t data)
{
    volatile USART_TypeDef *u = getUsart(mUsart);

    while (!(u->SR & USART_SR_TXE))
    {
    }

    u->DR = data;

    return true;
}

bool Usart::Rx(uint32_t &data)
{
    volatile USART_TypeDef *u = getUsart(mUsart);
    bool data_received = false;

    if (u->SR & USART_SR_RXNE)
    {
        data = u->DR;
        data_received = true;
    }

    return data_received;
}