#pragma once

#include "hal.h"

namespace app
{
    struct SPI
    {
        SPI_HandleTypeDef *spi;
        void write(const void *data, uint32_t size);
    };

    struct GPIO
    {
        GPIO_TypeDef *port;
        uint16_t pin;

        void write(bool what);
        void toggle();
    };
}