


#include "DisplayInterface.hpp"
#include "PeriphWrappers.hpp"


namespace app
{

    // using HAL_SPI_T = SPI_HandleTypeDef;
    // using HAL_GPIO_T = GPIO_TypeDef;

    

    DisplayInterface::DisplayInterface(SPI *spi, GPIO *cs) : _spi(spi),
                                                             _cs(cs)
    {
        _cs->write(false);
    }

    void DisplayInterface::write(uint8_t data) {
        _spi->write(&data, 1);
    }

    void DisplayInterface::write(const void *data, uint16_t size)
    {
        _spi->write(data, size);
        
    }

    void DisplayInterface::writeTransaction(const void *data, uint16_t size)
    {
        beginTransaction();
        write(data, size);
        endTransaction();
        
    }

    void DisplayInterface::beginTransaction() {
        _cs->write(true);
        // for(uint32_t i = 0; i<100000; ++i);
    }

    void DisplayInterface::endTransaction() {
        _cs->write(false);
        //  for(uint32_t i = 0; i<100000; ++i);
    }

} // namespace app