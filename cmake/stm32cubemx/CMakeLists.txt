cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
    USE_FULL_LL_DRIVER
	STM32G431xx
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../src
    ../../Drivers/STM32G4xx_HAL_Driver/Inc
    ../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy
    ../../Middlewares/Third_Party/FreeRTOS/Source/include
    ../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    ../../Drivers/CMSIS/Device/ST/STM32G4xx/Include
    ../../Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c

    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_comp.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_cordic.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crc.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crs.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dac.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_exti.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmac.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmc.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_hrtim.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_i2c.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lptim.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lpuart.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_opamp.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rng.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rtc.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_tim.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_ucpd.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usart.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.c 
    ../../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.c


    ../../Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/list.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/queue.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/timers.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    ../../startup_stm32g431xx.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


