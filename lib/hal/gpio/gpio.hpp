#pragma once
#include <cstdint>

enum GpioMode
{
    Input       = 0x0,
    Output      = 0x1,
    Alternate   = 0x2,
    Analog      = 0x3
};

enum GpioOutputType
{
    PushPull  = 0x0,
    OpenDrain = 0x1
};

class GpioInterface
{
public:
    GpioInterface() { }
    virtual void SetMode(uint8_t pinIndex, GpioMode mode) = 0;
    virtual void SetOutputType(uint8_t pinIndex, GpioOutputType mode) = 0;
    virtual void Write(uint8_t pinIndex, bool state) = 0;
    virtual uint8_t PinsPerRegister() = 0;
    virtual ~GpioInterface() { }
};