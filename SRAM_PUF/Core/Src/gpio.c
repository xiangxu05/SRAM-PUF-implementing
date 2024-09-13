/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SRAM_WE_Pin|SRAM_OE_Pin|SRAM_A7_Pin|SRAM_A8_Pin
                          |SRAM_A9_Pin|SRAM_A3_Pin|SRAM_A2_Pin|SRAM_A1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, FLASH_WP_Pin|FLASH_CS_Pin|FLASH_RST_Pin|SRAM_PWR_Pin
                          |SRAM_LB_Pin|SRAM_UB_Pin|SRAM_A10_Pin|SRAM_A4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SRAM_A6_Pin|SRAM_A5_Pin|SRAM_CS1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SRAM_A0_GPIO_Port, SRAM_A0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin */
  GPIO_InitStruct.Pin = SRAM_IO4_Pin|SRAM_IO5_Pin|SRAM_IO6_Pin|SRAM_IO7_Pin
                          |SRAM_IO9_Pin|SRAM_IO8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = SRAM_WE_Pin|SRAM_OE_Pin|SRAM_A7_Pin|SRAM_A8_Pin
                          |SRAM_A9_Pin|SRAM_A3_Pin|SRAM_A2_Pin|SRAM_A1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = FLASH_WP_Pin|FLASH_CS_Pin|FLASH_RST_Pin|SRAM_PWR_Pin
                          |SRAM_LB_Pin|SRAM_UB_Pin|SRAM_A10_Pin|SRAM_A4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = SRAM_IO14_Pin|SRAM_IO15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = SRAM_A6_Pin|SRAM_A5_Pin|SRAM_CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = SRAM_IO13_Pin|SRAM_IO12_Pin|SRAM_IO11_Pin|SRAM_IO10_Pin
                          |SRAM_IO0_Pin|SRAM_IO1_Pin|SRAM_IO2_Pin|SRAM_IO3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SRAM_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SRAM_A0_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

#define SRAM_IO4_Pin GPIO_PIN_13
#define SRAM_IO4_GPIO_Port GPIOC
#define SRAM_IO5_Pin GPIO_PIN_14
#define SRAM_IO5_GPIO_Port GPIOC
#define SRAM_IO6_Pin GPIO_PIN_15
#define SRAM_IO6_GPIO_Port GPIOC
#define SRAM_IO7_Pin GPIO_PIN_0
#define SRAM_IO7_GPIO_Port GPIOC
#define SRAM_WE_Pin GPIO_PIN_1
#define SRAM_WE_GPIO_Port GPIOC
#define FLASH_WP_Pin GPIO_PIN_0
#define FLASH_WP_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_1
#define FLASH_CS_GPIO_Port GPIOA
#define FLASH_RST_Pin GPIO_PIN_2
#define FLASH_RST_GPIO_Port GPIOA
#define SRAM_PWR_Pin GPIO_PIN_3
#define SRAM_PWR_GPIO_Port GPIOA
#define SRAM_IO14_Pin GPIO_PIN_4
#define SRAM_IO14_GPIO_Port GPIOA
#define SRAM_IO15_Pin GPIO_PIN_5
#define SRAM_IO15_GPIO_Port GPIOA
#define SRAM_LB_Pin GPIO_PIN_6
#define SRAM_LB_GPIO_Port GPIOA
#define SRAM_UB_Pin GPIO_PIN_7
#define SRAM_UB_GPIO_Port GPIOA
#define SRAM_OE_Pin GPIO_PIN_4
#define SRAM_OE_GPIO_Port GPIOC
#define SRAM_A7_Pin GPIO_PIN_5
#define SRAM_A7_GPIO_Port GPIOC
#define SRAM_A6_Pin GPIO_PIN_0
#define SRAM_A6_GPIO_Port GPIOB
#define SRAM_A5_Pin GPIO_PIN_1
#define SRAM_A5_GPIO_Port GPIOB
#define SRAM_IO13_Pin GPIO_PIN_12
#define SRAM_IO13_GPIO_Port GPIOB
#define SRAM_IO12_Pin GPIO_PIN_13
#define SRAM_IO12_GPIO_Port GPIOB
#define SRAM_IO11_Pin GPIO_PIN_14
#define SRAM_IO11_GPIO_Port GPIOB
#define SRAM_IO10_Pin GPIO_PIN_15
#define SRAM_IO10_GPIO_Port GPIOB
#define SRAM_IO9_Pin GPIO_PIN_6
#define SRAM_IO9_GPIO_Port GPIOC
#define SRAM_IO8_Pin GPIO_PIN_7
#define SRAM_IO8_GPIO_Port GPIOC
#define SRAM_A8_Pin GPIO_PIN_8
#define SRAM_A8_GPIO_Port GPIOC
#define SRAM_A9_Pin GPIO_PIN_9
#define SRAM_A9_GPIO_Port GPIOC
#define SRAM_A10_Pin GPIO_PIN_8
#define SRAM_A10_GPIO_Port GPIOA
#define SRAM_A4_Pin GPIO_PIN_15
#define SRAM_A4_GPIO_Port GPIOA
#define SRAM_A3_Pin GPIO_PIN_10
#define SRAM_A3_GPIO_Port GPIOC
#define SRAM_A2_Pin GPIO_PIN_11
#define SRAM_A2_GPIO_Port GPIOC
#define SRAM_A1_Pin GPIO_PIN_12
#define SRAM_A1_GPIO_Port GPIOC
#define SRAM_A0_Pin GPIO_PIN_2
#define SRAM_A0_GPIO_Port GPIOD
#define SRAM_CS1_Pin GPIO_PIN_3
#define SRAM_CS1_GPIO_Port GPIOB
#define SRAM_IO0_Pin GPIO_PIN_4
#define SRAM_IO0_GPIO_Port GPIOB
#define SRAM_IO1_Pin GPIO_PIN_5
#define SRAM_IO1_GPIO_Port GPIOB
#define SRAM_IO2_Pin GPIO_PIN_8
#define SRAM_IO2_GPIO_Port GPIOB
#define SRAM_IO3_Pin GPIO_PIN_9
#define SRAM_IO3_GPIO_Port GPIOB

/* USER CODE END 2 */
