#include "main.h"
#include "stm32g4xx_it.h"
#include "Device.hpp"

extern TIM_HandleTypeDef htim2;
extern void unlockRxTask();
extern "C"
{

    /******************************************************************************/
    /*           Cortex-M4 Processor Interruption and Exception Handlers          */
    /******************************************************************************/

    void NMI_Handler(void)
    {
        while (1)
        {
        }
    }

    void HardFault_Handler(void)
    {
        while (1)
        {
        }
    }

    void MemManage_Handler(void)
    {
        while (1)
        {
        }
    }

    void BusFault_Handler(void)
    {
        while (1)
        {
        }
    }

    void UsageFault_Handler(void)
    {
        while (1)
        {
        }
    }

    void DebugMon_Handler(void)
    {
    }

    /******************************************************************************/
    /* STM32G4xx Peripheral Interrupt Handlers                                    */
    /* Add here the Interrupt Handlers for the used peripherals.                  */
    /* For the available peripheral interrupt handler names,                      */
    /* please refer to the startup file (startup_stm32g4xx.s).                    */
    /******************************************************************************/

    void TIM2_IRQHandler(void)
    {
        HAL_TIM_IRQHandler(&htim2);
    }

    void SPI1_IRQHandler(void)
    {
        // auto spi = DISPLAY_SPI;
        auto &dev = app::Device::instance();

        // app::DispRxFrame tmp;

        // while (LL_SPI_IsActiveFlag_RXNE(spi) && dev.displayRxBuf.isSpaceAvailable())
        // {
            
            
        //     tmp.data = LL_SPI_ReceiveData8(spi);
        //     tmp.isCommand = LL_GPIO_IsInputPinSet(DISPLAY_DI_PORT, DISPLAY_DI_PIN) == 0;
        //     std::swap(dev.displayRxBuf.new_back(), tmp);
        //     // tmp.isCommand = LL_GPIO_IsInputPinSet(DISPLAY_DI_PORT, DISPLAY_DI_PIN) == 0; //(_impl.inputSPI_DI.port->IDR & (1 << _impl.inputSPI_DI.pin))==0;
        //     // tmp.chipSelect = HAL_GPIO_ReadPin(_impl.inputSPI_CS.port, _impl.inputSPI_CS.pin) == GPIO_PIN_SET;
        //     // tmp.reset = HAL_GPIO_ReadPin(_impl.inputSPI_RST.port, _impl.inputSPI_RST.pin) == GPIO_PIN_SET;
        //     // tmp.data = LL_SPI_ReceiveData8(spi);
        //     // if (dev.displayRxBuf.isSpaceAvailable())
        //     // {
        //     // dev.displayRxBuf.push(tmp);
        //     // }

        // } //while (LL_SPI_IsActiveFlag_RXNE(spi));

        // if (LL_SPI_IsActiveFlag_OVR(spi))
        // {
        //     LL_SPI_ClearFlag_OVR(spi);
        // }
        // unlockRxTask();
        dev.handleInterruptInputSPI();
    }
    void EXTI4_IRQHandler(void)
    {
        if (!LL_GPIO_IsInputPinSet(INPUT_DISPLAY_CS_PORT, INPUT_DISPLAY_CS_PIN))
        {
            LL_SPI_Enable(INPUT_DISPLAY_SPI);
        }
        else
        {
            LL_SPI_Disable(INPUT_DISPLAY_SPI);
        }
        LL_EXTI_ClearFlag_0_31(INPUT_DISPLAY_CS_PIN);
        // HAL_GPIO_EXTI_IRQHandler(B1_Pin);
        // app::Device::instance().handleInterruptInputSPI_CS();
    }

    void EXTI9_5_IRQHandler(void)
    {
        LL_SPI_Enable(INPUT_DISPLAY_SPI);
        if(LL_GPIO_IsInputPinSet(INPUT_DISPLAY_CS_PORT, INPUT_DISPLAY_CS_PIN))
        {
            LL_SPI_Disable(INPUT_DISPLAY_SPI);
        }
        LL_EXTI_ClearFlag_0_31(INPUT_DISPLAY_CS_PIN);
        // app::Device::instance().handleInterruptInputSPI_CS();
    }

    void EXTI15_10_IRQHandler(void)
    {
        HAL_GPIO_EXTI_IRQHandler(B1_Pin);
    }

} // extern "C"
