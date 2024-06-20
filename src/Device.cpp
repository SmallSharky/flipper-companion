

#include "Device.hpp"
#include "PeriphWrappers.hpp"

#include "main.h"

namespace app
{

    struct Device::_PImpl
    {
        SPI_HandleTypeDef display_spi_handle;
        SPI display_spi;
        GPIO display_cs;
        DisplayInterface display_interface;

        static _PImpl &instance()
        {
            static _PImpl inst;
            return inst;
        }

    private:
        _PImpl():
            display_spi_handle({
                .Instance = DISPLAY_SPI
            }),
            display_spi(&display_spi_handle),
            display_cs({DISPLAY_CS_PORT, DISPLAY_CS_PIN}),
            display_interface(&display_spi, &display_cs)
        {
            init_display_spi();
        }

        void init_display_spi() {
            auto& hspi = *display_spi.spi;
            // hspi.Instance = SPI3;
            hspi.Init.Mode = SPI_MODE_MASTER;
            hspi.Init.Direction = SPI_DIRECTION_1LINE;
            hspi.Init.DataSize = SPI_DATASIZE_8BIT;
            hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
            hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
            hspi.Init.NSS = SPI_NSS_SOFT;
            hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
            hspi.Init.FirstBit = SPI_FIRSTBIT_LSB;
            hspi.Init.TIMode = SPI_TIMODE_DISABLE;
            hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
            hspi.Init.CRCPolynomial = 7;
            hspi.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
            hspi.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
            
            if (HAL_SPI_Init(&hspi) != HAL_OK)
            {
                Error_Handler();
            }
        }
    };

    Device::Device():
    _impl(_PImpl::instance()),
    display(&_impl.display_interface),
    led({LD2_GPIO_Port, LD2_Pin}),
    button({B1_GPIO_Port, B1_Pin})

    {

    }

    Device& Device::instance() {
        static Device dev;
        return dev;
    }

    

} // namespace app
