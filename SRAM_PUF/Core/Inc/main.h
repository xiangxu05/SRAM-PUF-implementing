/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h" 
#include "task.h" 
#include "semphr.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
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

/* USER CODE BEGIN Private defines */
#define	W25_SPI_FLASH_CSH() HAL_GPIO_WritePin(FLASH_CS_GPIO_Port,FLASH_CS_Pin,GPIO_PIN_SET)  //选中FLASH	
#define	W25_SPI_FLASH_CSL() HAL_GPIO_WritePin(FLASH_CS_GPIO_Port,FLASH_CS_Pin,GPIO_PIN_RESET)  //取消选中FLASH	

#define	W25_SPI_FLASH_WPH() HAL_GPIO_WritePin(FLASH_WP_GPIO_Port,FLASH_WP_Pin,GPIO_PIN_SET)  //flash写引脚拉高
#define	W25_SPI_FLASH_WPL() HAL_GPIO_WritePin(FLASH_WP_GPIO_Port,FLASH_WP_Pin,GPIO_PIN_RESET) 

#define	W25_SPI_FLASH_RSTH() HAL_GPIO_WritePin(FLASH_RST_GPIO_Port,FLASH_RST_Pin,GPIO_PIN_SET)  //重置引脚拉高
#define	W25_SPI_FLASH_RSTL() HAL_GPIO_WritePin(FLASH_RST_GPIO_Port,FLASH_RST_Pin,GPIO_PIN_RESET)  	

#define W25_Handle hspi2

#define	SRAM_PWR_H() HAL_GPIO_WritePin(SRAM_PWR_GPIO_Port,SRAM_PWR_Pin,GPIO_PIN_SET)  
#define	SRAM_PWR_L() HAL_GPIO_WritePin(SRAM_PWR_GPIO_Port,SRAM_PWR_Pin,GPIO_PIN_RESET)  	

#define	SRAM_CS_H() HAL_GPIO_WritePin(SRAM_CS_GPIO_Port,SRAM_CS_Pin,GPIO_PIN_SET)  //选中FLASH	
#define	SRAM_CS_L() HAL_GPIO_WritePin(SRAM_CS_GPIO_Port,SRAM_CS_Pin,GPIO_PIN_RESET)  //取消选中FLASH	



extern QueueHandle_t   usart1Msg;
extern QueueHandle_t   usbMsg;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
