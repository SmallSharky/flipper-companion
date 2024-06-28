

#include "Device.hpp"
#include "PeriphWrappers.hpp"


#include "main.h"

#include <cstdio>

extern void unlockRxTask();

namespace app
{
    
    struct Device::_PImpl
    {
        SPI inputSPI;
        GPIO inputSPI_CS;
        GPIO inputSPI_DI;
        GPIO inputSPI_RST;

        SPI outputSPI;
        GPIO outputSPI_CS;
        GPIO outputSPI_DC;
        GPIO outputSPI_RST;

        static _PImpl &instance()
        {
            static _PImpl inst;
            return inst;
        }

        static void inputSPIRxISR([[maybe_unused]] SPI_HandleTypeDef *spi)
        {
            printf("iSPI RX\n");
        }

    private:
        _PImpl() : inputSPI(INPUT_DISPLAY_SPI),
                   inputSPI_CS({INPUT_DISPLAY_CS_PORT, INPUT_DISPLAY_CS_PIN}),
                   inputSPI_DI({INPUT_DISPLAY_DI_PORT, INPUT_DISPLAY_DI_PIN}),
                   inputSPI_RST({INPUT_DISPLAY_RST_PORT, INPUT_DISPLAY_RST_PIN}),
                   outputSPI(OUTPUT_DISPLAY_SPI),
                   outputSPI_CS({OUTPUT_DISPLAY_CS_PORT, OUTPUT_DISPLAY_CS_PIN}),
                   outputSPI_DC({OUTPUT_DISPLAY_DC_PORT, OUTPUT_DISPLAY_DC_PIN}),
                   outputSPI_RST({OUTPUT_DISPLAY_RST_PORT, OUTPUT_DISPLAY_RST_PIN})
        {
            initInputSPI();
            initOutputSPI();
        }

        void initInputSPI()
        {
            GPIO_InitTypeDef GPIO_InitStruct{};
            GPIO_InitStruct.Pin = INPUT_DISPLAY_MOSI_PIN;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
            HAL_GPIO_Init(INPUT_DISPLAY_MOSI_PORT, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = INPUT_DISPLAY_SCK_PIN;
            HAL_GPIO_Init(INPUT_DISPLAY_SCK_PORT, &GPIO_InitStruct);

            LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

            LL_SPI_InitTypeDef SPI_InitStruct{};
            LL_SPI_Disable(inputSPI.spi);
            LL_SPI_StructInit(&SPI_InitStruct);
            SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
            SPI_InitStruct.Mode = LL_SPI_MODE_SLAVE;
            SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
            SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
            SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
            // SPI_InitStruct.NSS = LL_SPI_NSS_HARD_INPUT;
            SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
            SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
            SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
            SPI_InitStruct.CRCPoly = 7;
            LL_SPI_Init(inputSPI.spi, &SPI_InitStruct);
            LL_SPI_SetStandard(inputSPI.spi, LL_SPI_PROTOCOL_MOTOROLA);
            LL_SPI_DisableNSSPulseMgt(inputSPI.spi);

            GPIO_InitStruct.Pin = inputSPI_CS.pin;            
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
            // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            // GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
            HAL_GPIO_Init(inputSPI_CS.port, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = inputSPI_DI.pin;
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
            HAL_GPIO_Init(inputSPI_DI.port, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = inputSPI_RST.pin;
            HAL_GPIO_Init(inputSPI_RST.port, &GPIO_InitStruct);

            HAL_NVIC_SetPriority(SPI1_IRQn, 5, 5);
            // NVIC_SetPriority(SPI1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0));
            NVIC_EnableIRQ(SPI1_IRQn);

            HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 6);
            HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
        }


        void initOutputSPI() {
            printf("Init Output Display Periph begin\n");
            GPIO_InitTypeDef GPIO_InitStruct{};
            GPIO_InitStruct.Pin = OUTPUT_DISPLAY_MOSI_PIN;
            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
            HAL_GPIO_Init(OUTPUT_DISPLAY_MOSI_PORT, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = OUTPUT_DISPLAY_SCK_PIN;
            HAL_GPIO_Init(OUTPUT_DISPLAY_SCK_PORT, &GPIO_InitStruct);

            LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

            // LL_SPI_InitTypeDef SPI_InitStruct{};
            // LL_SPI_Disable(outputSPI.spi);
            // LL_SPI_StructInit(&SPI_InitStruct);
            // SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
            // SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
            // SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
            // SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
            // SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
            // SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
            // SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV16;
            // SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
            // SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
            // SPI_InitStruct.CRCPoly = 7;
            // LL_SPI_Init(outputSPI.spi, &SPI_InitStruct);
            // LL_SPI_SetStandard(outputSPI.spi, LL_SPI_PROTOCOL_MOTOROLA);
            // LL_SPI_DisableNSSPulseMgt(outputSPI.spi);
            // LL_SPI_Enable(outputSPI.spi);

            LL_SPI_InitTypeDef SPI_InitStruct{};
            LL_SPI_Disable(outputSPI.spi);
            LL_SPI_StructInit(&SPI_InitStruct);
            SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
            SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
            SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
            SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
            SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
            SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
            SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV32;
            SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
            SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
            SPI_InitStruct.CRCPoly = 7;
            LL_SPI_Init(outputSPI.spi, &SPI_InitStruct);
            LL_SPI_SetStandard(outputSPI.spi, LL_SPI_PROTOCOL_MOTOROLA);
            LL_SPI_EnableNSSPulseMgt(outputSPI.spi);
            LL_SPI_Enable(outputSPI.spi);


            GPIO_InitStruct.Pin = outputSPI_CS.pin;            
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            HAL_GPIO_Init(outputSPI_CS.port, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = outputSPI_DC.pin;
            HAL_GPIO_Init(outputSPI_DC.port, &GPIO_InitStruct);

            GPIO_InitStruct.Pin = outputSPI_RST.pin;
            HAL_GPIO_Init(outputSPI_RST.port, &GPIO_InitStruct);

            printf("Init Output Display Periph done\n");
        }
    };

    Device::Device() : _impl(_PImpl::instance()),
                       led({LD2_GPIO_Port, LD2_Pin}),
                       button({B1_GPIO_Port, B1_Pin}),
                       display(_impl.outputSPI, _impl.outputSPI_CS, _impl.outputSPI_DC, _impl.outputSPI_RST)

    {
        printf("Display init begin\n");
        inputSPIReceiveIT();
        display.init();
        printf("Device init done\n");
    }

    Device &Device::instance()
    {
        static Device dev;
        return dev;
    }

    void Device::handleInterruptInputSPI()
    {
        auto spi = INPUT_DISPLAY_SPI;
        // auto &dev = app::Device::instance();

        uint16_t tmp;

        
        while (LL_SPI_IsActiveFlag_RXNE(spi) ) {
            reinterpret_cast<uint8_t *>(&tmp)[0] = LL_SPI_ReceiveData8(spi);
            reinterpret_cast<uint8_t *>(&tmp)[1] = LL_GPIO_IsInputPinSet(INPUT_DISPLAY_DI_PORT, INPUT_DISPLAY_DI_PIN);
            
            
            // std::swap(displayRxBuf.new_back(), tmp);
            // tmp.isCommand = LL_GPIO_IsInputPinSet(DISPLAY_DI_PORT, DISPLAY_DI_PIN) == 0; //(_impl.inputSPI_DI.port->IDR & (1 << _impl.inputSPI_DI.pin))==0;
            // tmp.chipSelect = HAL_GPIO_ReadPin(_impl.inputSPI_CS.port, _impl.inputSPI_CS.pin) == GPIO_PIN_SET;
            // tmp.reset = HAL_GPIO_ReadPin(_impl.inputSPI_RST.port, _impl.inputSPI_RST.pin) == GPIO_PIN_SET;
            // tmp.data = LL_SPI_ReceiveData8(spi);
            if (displayRxBuf.writeAvailable())
            {
                displayRxBuf.push(tmp);
            } 
            // else {
            //     printf("IGNORE\n");
            // }

        } // while (LL_SPI_IsActiveFlag_RXNE(spi));

        if (LL_SPI_IsActiveFlag_OVR(spi))
        {
            LL_SPI_ClearFlag_OVR(spi);
        }
        unlockRxTask();
        // // auto spi = _impl.inputSPI.spi;
        
        // // DispRxFrame tmp;
        

        // // while (LL_SPI_IsActiveFlag_RXNE(spi) && displayRxBuf.isSpaceAvailable())
        // // {
        // //     tmp.isCommand = LL_GPIO_IsInputPinSet(_impl.inputSPI_DI.port, _impl.inputSPI_DI.pin) == 0;//(_impl.inputSPI_DI.port->IDR & (1 << _impl.inputSPI_DI.pin))==0;
        // //     // tmp.chipSelect = HAL_GPIO_ReadPin(_impl.inputSPI_CS.port, _impl.inputSPI_CS.pin) == GPIO_PIN_SET;
        // //     // tmp.reset = HAL_GPIO_ReadPin(_impl.inputSPI_RST.port, _impl.inputSPI_RST.pin) == GPIO_PIN_SET;
        // //     tmp.data = LL_SPI_ReceiveData8(spi);
        // //     displayRxBuf.push(tmp);
        // // }
        

        // // if (LL_SPI_IsActiveFlag_OVR(spi))
        // // {
        // //     LL_SPI_ClearFlag_OVR(spi);
        // // }
        // _rxCallback();
    }
    void Device::handleInterruptInputSPI_CS()
    {
        // printf("SPI CS interrupt\n");
        auto spi = _impl.inputSPI.spi;
        if(!LL_GPIO_IsInputPinSet(_impl.inputSPI_CS.port, _impl.inputSPI_CS.pin)){
            LL_SPI_Enable(spi);
        } else {
            LL_SPI_Disable(spi);
        }
        LL_EXTI_ClearFlag_0_31(_impl.inputSPI_CS.pin);
        // HAL_GPIO_EXTI_IRQHandler(_impl.inputSPI_CS.pin);
        
        

        // HAL_SPI_IRQHandler(_impl.inputSPI.spi);
    }

    void Device::inputSPIReceiveIT()
    {
        printf("SPI enable interrupt\n");
        // return;
        auto spi = _impl.inputSPI.spi;
        LL_SPI_Disable(spi);
        LL_SPI_SetRxFIFOThreshold(spi, LL_SPI_RX_FIFO_TH_QUARTER);

        LL_SPI_EnableIT_ERR(spi);
        LL_SPI_EnableIT_RXNE(spi);
        // LL_SPI_Enable(spi);
        printf("SPI enable interrupt DONE\n");
        // spi->SR &= ~LL_SPI_SR_RXNE;
        // auto hspi = _impl.inputSPI.spi;
        // /* Set the transaction information */
        // hspi->State = HAL_SPI_STATE_BUSY_RX;
        // hspi->ErrorCode = HAL_SPI_ERROR_NONE;
        // hspi->pRxBuffPtr = _impl.inputSPIRxBuf.data();
        // hspi->RxXferSize = _impl.inputSPIRxBuf.size();
        // hspi->RxXferCount = _impl.inputSPIRxBuf.size();

        // /* Init field not used in handle to zero */
        // hspi->pTxBuffPtr = (uint8_t *)NULL;
        // hspi->TxXferSize = 0U;
        // hspi->TxXferCount = 0U;
        // hspi->TxISR = NULL;

        // SET_BIT(hspi->Instance->CR2, SPI_RXFIFO_THRESHOLD);
        // hspi->RxISR = _PImpl::inputSPIRxISR;

        // if ((hspi->Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE)
        // {
        //     /* Enable SPI peripheral */
        //     __HAL_SPI_ENABLE(hspi);
        // }

        // __HAL_SPI_ENABLE_IT(hspi, (SPI_IT_RXNE | SPI_IT_ERR));
        // HAL_SPI_Receive_IT(_impl.inputSPI.spi, _impl.inputSPIRxBuf.data(), _impl.inputSPIRxBuf.size());
    }

} // namespace app
