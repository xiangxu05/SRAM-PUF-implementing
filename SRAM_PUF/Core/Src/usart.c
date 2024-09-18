/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h" 
#include "task.h" 
#include "semphr.h"


int Rx_cnt;
int bufused1;
struct urt_aRxBuffer_t urt1_aRxBuffer[2];
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
	__HAL_UART_ENABLE(&huart1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); 
	
	dma_usart1_receive(&huart1, (unsigned int)urt1_aRxBuffer[0].rx_buf, sizeof(urt1_aRxBuffer[0].rx_buf));
  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */



int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (char) ch;
	uint8_t message = (uint8_t) ch;
	usb_send(&message,1);
	return ch;
}


void USART1_send(char* buf, int len)
{ 		
	for(int i = 0;i<len;i++){
			while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
			USART1->DR =  buf[i];      
	}
}


void dma_usart1_receive(UART_HandleTypeDef * huart, unsigned int rx_buf, uint16_t size)
{
	
	/*配置DMA缓存中断传输*/
	HAL_DMA_Start_IT(huart->hdmarx,(uint32_t)&huart->Instance->DR,rx_buf,size);
	
	/*使能Usart1_DMA接收*/
	huart->Instance->CR3 |= USART_CR3_DMAR;
}

void USART1_IRQHandler_user(void)					
{ 
	
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE) !=RESET)) 
	{		
		/*清标志位*/
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);		
	}	
	
	/*空闲中断处理*/
	if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) !=RESET)) 
	{		
		/*清标志位*/
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);		
		
		if(huart1.hdmarx != NULL)
		{
			Rx_cnt = sizeof(urt1_aRxBuffer[0].rx_buf) - huart1.hdmarx->Instance->NDTR;
			HAL_DMA_Abort(huart1.hdmarx);
		}
		else{
			return;
		}
		
		bufused1 = (bufused1+1)&1;
		dma_usart1_receive(&huart1, (unsigned int)urt1_aRxBuffer[bufused1].rx_buf, sizeof(urt1_aRxBuffer[0].rx_buf));
		
		if(Rx_cnt == 0){
			return;
		}
		
		static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		urt1_aRxBuffer[(bufused1+1)&1].rx_len = Rx_cnt;
		xQueueSendFromISR( usart1Msg, &urt1_aRxBuffer[(bufused1+1)&1], &xHigherPriorityTaskWoken);
	}	
}


void dma_usart1_transmit(UART_HandleTypeDef *huart, unsigned int pData, uint16_t Size)
{
	/*开启DMA传输*/
    HAL_DMA_Start(huart->hdmatx, (unsigned int)pData, (unsigned int)&huart->Instance->DR, Size);

	/*使能串口DMA发送*/
    huart->Instance->CR3 |= USART_CR3_DMAT;
}	 

void dma_usart1_tx_start(unsigned char * buf, unsigned int size)
{
	UART_HandleTypeDef *huart = &huart1;	

	static char init = 0;
	if(init == 0){
			init++;
	}
	else{
			while(1)
			{																				
				/*等待DMA2_Steam7传输完成*/
				if(__HAL_DMA_GET_FLAG(&hdma_usart1_tx,DMA_FLAG_TCIF3_7))
				{
					/*清除DMA2_Steam7传输完成标志*/
					__HAL_DMA_CLEAR_FLAG(&hdma_usart1_tx,DMA_FLAG_TCIF3_7);

					/*传输完成以后关闭串口DMA*/
					huart->Instance->CR3 &= ~USART_CR3_DMAT;

					/*杀死Dma_Usart1_Tx数据流*/
					if(huart->hdmatx != NULL)
					{
						HAL_DMA_Abort(huart->hdmatx);
					}
					break;
				}
				else
				{
					
				}
			}
	}

	HAL_DMA_Abort(huart->hdmatx);
	/*启动传输*/
	
	dma_usart1_transmit(&huart1,(unsigned int)buf,size);   
}

/* USER CODE END 1 */
