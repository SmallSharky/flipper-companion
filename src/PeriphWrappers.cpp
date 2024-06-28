
#include "PeriphWrappers.hpp"

#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_spi.h"

namespace app
{

    void GPIO::write(bool what)
    {
        if(what) {
            LL_GPIO_SetOutputPin(port, pin);
        } else {
            LL_GPIO_ResetOutputPin(port, pin);
        }
    }

    void GPIO::toggle() {
        LL_GPIO_TogglePin(port, pin);
    }

    bool GPIO::read() {
        return LL_GPIO_IsInputPinSet(port, pin);
    }

    void SPI::write(const void *data, uint32_t size)
    {
        for(size_t i = 0; i<size; ++i) {
            LL_SPI_TransmitData8(spi, ((uint8_t*)data)[i]);
            while(LL_I2S_IsActiveFlag_BSY(spi));
        }
    }

}