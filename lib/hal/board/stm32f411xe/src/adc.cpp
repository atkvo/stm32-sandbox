#include "adc.hpp"
#include "stm32f411xe.h"

/**
 * @brief Get the Resolution From Config Bits object
 *
 * @return Adc::Resolution
 */
Adc::Resolution get_current_resolution();

/*
void ADC_IRQHandler(void)
{
}
*/

const uint32_t Adc::MAX_RESOLUTION = Adc::Resolution::ADC_RESOLUTION_12_BIT;

Adc::Adc()
{
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // enable ADC1 clock
    ADC1->CR2    |= ADC_CR2_ADON;       // enable ADC peripheral

    m_configured_resolution = get_current_resolution();
}

Adc::~Adc()
{
    RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN; // disable ADC1 clock
    ADC1->CR2    &= ~ADC_CR2_ADON;       // disable ADC peripheral
}

bool Adc::set_resolution(Resolution r)
{
    uint8_t mode;

    switch (r)
    {
        case Resolution::ADC_RESOLUTION_12_BIT:
            mode = 0x00;
            break;
        case Resolution::ADC_RESOLUTION_10_BIT:
            mode = 0x01;
            break;
        case Resolution::ADC_RESOLUTION_8_BIT:
            mode = 0x10;
            break;
        case Resolution::ADC_RESOLUTION_6_BIT:
            mode = 0x11;
            break;
        default:
            return false;
    }

    ADC1->CR1 &= ~(ADC_CR1_RES);
    ADC1->CR1 |= (mode << ADC_CR1_RES_Pos);

    return true;
}

uint32_t Adc::read()
{
    return ADC1->DR & m_configured_resolution;
}

void Adc::start_conversion()
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

bool Adc::is_conversion_done()
{
    return ((ADC1->SR & ADC_SR_EOC) != 0);
}

Adc::Resolution get_current_resolution()
{
    uint8_t const mode = (ADC1->CR1 & ADC_CR1_RES) >> ADC_CR1_RES_Pos;

    switch (mode)
    {
        case 0x01:
            return Adc::Resolution::ADC_RESOLUTION_10_BIT;
        case 0x10:
            return Adc::Resolution::ADC_RESOLUTION_8_BIT;
        case 0x11:
            return Adc::Resolution::ADC_RESOLUTION_6_BIT;

        case 0x00:   /* intentional fall through - reset state */
        default:
            return Adc::Resolution::ADC_RESOLUTION_12_BIT;
    }
}
