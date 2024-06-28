#include "main.h"
#include "stm32g4xx_it.h"
#include "Device.hpp"

extern TIM_HandleTypeDef htim2;
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
        app::Device::instance().handleInterruptInputSPI();
    }

    void EXTI9_5_IRQHandler(void)
    {
        LL_SPI_Enable(INPUT_DISPLAY_SPI);
        if(LL_GPIO_IsInputPinSet(INPUT_DISPLAY_CS_PORT, INPUT_DISPLAY_CS_PIN))
        {
            LL_SPI_Disable(INPUT_DISPLAY_SPI);
        }
        LL_EXTI_ClearFlag_0_31(INPUT_DISPLAY_CS_PIN);
    }

    void EXTI15_10_IRQHandler(void)
    {
        HAL_GPIO_EXTI_IRQHandler(B1_Pin);
    }

} // extern "C"
