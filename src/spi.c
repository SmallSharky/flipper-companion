/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    spi.c
 * @brief   This file provides code for the configuration
 *          of the SPI instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

SPI_HandleTypeDef hspi;

/* SPI3 init function */
void MX_SPI_Init(void)
{

    /* USER CODE BEGIN SPI3_Init 0 */

    /* USER CODE END SPI3_Init 0 */

    /* USER CODE BEGIN SPI3_Init 1 */

    /* USER CODE END SPI3_Init 1 */
    
    /* USER CODE BEGIN SPI3_Init 2 */

    /* USER CODE END SPI3_Init 2 */
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *spiHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (spiHandle->Instance == SPI3)
    {
        /* USER CODE BEGIN SPI3_MspInit 0 */

        /* USER CODE END SPI3_MspInit 0 */
        /* SPI3 clock enable */
        
        
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = DISPLAY_CS_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(DISPLAY_CS_PORT, &GPIO_InitStruct);

        /* USER CODE BEGIN SPI3_MspInit 1 */

        /* USER CODE END SPI3_MspInit 1 */
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *spiHandle)
{

    if (spiHandle->Instance == SPI3)
    {
        /* USER CODE BEGIN SPI3_MspDeInit 0 */

        /* USER CODE END SPI3_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_3 | GPIO_PIN_5);

        /* USER CODE BEGIN SPI3_MspDeInit 1 */

        /* USER CODE END SPI3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
