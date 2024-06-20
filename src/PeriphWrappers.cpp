
#include "PeriphWrappers.hpp"
namespace app
{

    void GPIO::write(bool what)
    {
        HAL_GPIO_WritePin(port, pin, (what ? GPIO_PIN_SET : GPIO_PIN_RESET));
    }

    void GPIO::toggle() {
        HAL_GPIO_TogglePin(port, pin);
    }

    void SPI::write(const void *data, uint32_t size)
    {
        HAL_SPI_Transmit(spi, static_cast<uint8_t*>(const_cast<void *>(data)), size, 100000);
        while(HAL_SPI_GetState(spi) != HAL_SPI_STATE_READY);
    }

}