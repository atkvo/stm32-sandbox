#include "adc.hpp"
#include "stm32f411xe.h"

namespace
{
    Listener *_listener = nullptr;
}

void ADC_IRQHandler(void)
{
    if (_listener)
    {
        _listener->Notify(0);
    }
}

Adc::Adc()
{
    _listener = nullptr;
}

void Adc::RegisterListener(Event e, Listener &lis)
{
    _listener = &lis;
}

uint32_t Adc::Read()
{
    return ADC1->DR & m_ConfiguredResolution;
}

void Adc::ConfigureDmaAddress(uint32_t *data)
{

}
