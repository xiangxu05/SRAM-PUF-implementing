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
#define FLASH_WP_Pin GPIO_PIN_0
#define FLASH_WP_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_1
#define FLASH_CS_GPIO_Port GPIOA
#define FLASH_RST_Pin GPIO_PIN_2
#define FLASH_RST_GPIO_Port GPIOA
#define SRAM_PWR_Pin GPIO_PIN_3
#define SRAM_PWR_GPIO_Port GPIOA
#define SRAM_CS_Pin GPIO_PIN_4
#define SRAM_CS_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */
#define	W25_SPI_FLASH_CSH() HAL_GPIO_WritePin(FLASH_CS_GPIO_Port,FLASH_CS_Pin,GPIO_PIN_SET)  //选中FLASH	
#define	W25_SPI_FLASH_CSL() HAL_GPIO_WritePin(FLASH_CS_GPIO_Port,FLASH_CS_Pin,GPIO_PIN_RESET)  //取消选中FLASH	

#define	W25_SPI_FLASH_WPH() HAL_GPIO_WritePin(FLASH_WP_GPIO_Port,FLASH_WP_Pin,GPIO_PIN_SET)  
#define	W25_SPI_FLASH_WPL() HAL_GPIO_WritePin(FLASH_WP_GPIO_Port,FLASH_WP_Pin,GPIO_PIN_RESET) 

#define	W25_SPI_FLASH_RSTH() HAL_GPIO_WritePin(FLASH_RST_GPIO_Port,FLASH_RST_Pin,GPIO_PIN_SET)  
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
