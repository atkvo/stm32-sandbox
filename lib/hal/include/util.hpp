#pragma once

#include <cstdint>

class Listener
{
public:
    /**
     * @brief Notifies the listener
     *
     * @param x
     */
    virtual void Notify(uint32_t x) = 0;
};
