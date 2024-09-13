#include "sram.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "gpio.h"
#include "stdio.h"


void sram_IO_input(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};


  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                          PCPin PCPin */
  GPIO_InitStruct.Pin = SRAM_IO4_Pin|SRAM_IO5_Pin|SRAM_IO6_Pin|SRAM_IO7_Pin
                          |SRAM_IO9_Pin|SRAM_IO8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = SRAM_IO14_Pin|SRAM_IO15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = SRAM_IO13_Pin|SRAM_IO12_Pin|SRAM_IO11_Pin|SRAM_IO10_Pin
                          |SRAM_IO0_Pin|SRAM_IO1_Pin|SRAM_IO2_Pin|SRAM_IO3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}



void sram_IO_output(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};



  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin */
  GPIO_InitStruct.Pin = SRAM_IO4_Pin|SRAM_IO5_Pin|SRAM_IO6_Pin|SRAM_IO7_Pin
                          |SRAM_IO9_Pin|SRAM_IO8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
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
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
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
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SRAM_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SRAM_A0_GPIO_Port, &GPIO_InitStruct);

}


void sram_GPIO_DeInit(void)
{
  HAL_GPIO_DeInit(SRAM_IO0_GPIO_Port, SRAM_IO0_Pin);
  HAL_GPIO_DeInit(SRAM_IO1_GPIO_Port, SRAM_IO1_Pin);
  HAL_GPIO_DeInit(SRAM_IO2_GPIO_Port, SRAM_IO2_Pin);
  HAL_GPIO_DeInit(SRAM_IO3_GPIO_Port, SRAM_IO3_Pin);
  HAL_GPIO_DeInit(SRAM_IO4_GPIO_Port, SRAM_IO4_Pin);
  HAL_GPIO_DeInit(SRAM_IO5_GPIO_Port, SRAM_IO5_Pin);
  HAL_GPIO_DeInit(SRAM_IO6_GPIO_Port, SRAM_IO6_Pin);
  HAL_GPIO_DeInit(SRAM_IO7_GPIO_Port, SRAM_IO7_Pin);
  HAL_GPIO_DeInit(SRAM_IO8_GPIO_Port, SRAM_IO8_Pin);
  HAL_GPIO_DeInit(SRAM_IO9_GPIO_Port, SRAM_IO9_Pin);
  HAL_GPIO_DeInit(SRAM_IO10_GPIO_Port, SRAM_IO10_Pin);
  HAL_GPIO_DeInit(SRAM_IO11_GPIO_Port, SRAM_IO11_Pin);
  HAL_GPIO_DeInit(SRAM_IO12_GPIO_Port, SRAM_IO12_Pin);
  HAL_GPIO_DeInit(SRAM_IO13_GPIO_Port, SRAM_IO13_Pin);
  HAL_GPIO_DeInit(SRAM_IO14_GPIO_Port, SRAM_IO14_Pin);
  HAL_GPIO_DeInit(SRAM_IO15_GPIO_Port, SRAM_IO15_Pin);
	
	HAL_GPIO_DeInit(SRAM_A0_GPIO_Port, SRAM_A0_Pin);
  HAL_GPIO_DeInit(SRAM_A1_GPIO_Port, SRAM_A1_Pin);
  HAL_GPIO_DeInit(SRAM_A2_GPIO_Port, SRAM_A2_Pin);
  HAL_GPIO_DeInit(SRAM_A3_GPIO_Port, SRAM_A3_Pin);
  HAL_GPIO_DeInit(SRAM_A4_GPIO_Port, SRAM_A4_Pin);
  HAL_GPIO_DeInit(SRAM_A5_GPIO_Port, SRAM_A5_Pin);
  HAL_GPIO_DeInit(SRAM_A6_GPIO_Port, SRAM_A6_Pin);
  HAL_GPIO_DeInit(SRAM_A7_GPIO_Port, SRAM_A7_Pin);
  HAL_GPIO_DeInit(SRAM_A8_GPIO_Port, SRAM_A8_Pin);
  HAL_GPIO_DeInit(SRAM_A9_GPIO_Port, SRAM_A9_Pin);
  HAL_GPIO_DeInit(SRAM_A10_GPIO_Port, SRAM_A10_Pin);
	
	HAL_GPIO_DeInit(SRAM_WE_GPIO_Port, SRAM_WE_Pin);
  HAL_GPIO_DeInit(SRAM_OE_GPIO_Port, SRAM_OE_Pin);
	
  HAL_GPIO_DeInit(SRAM_CS1_GPIO_Port, SRAM_CS1_Pin);
	
	HAL_GPIO_DeInit(SRAM_LB_GPIO_Port, SRAM_LB_Pin);
  HAL_GPIO_DeInit(SRAM_UB_GPIO_Port, SRAM_UB_Pin);
}


void sram_pin_init()
{
	HAL_GPIO_WritePin(SRAM_LB_GPIO_Port,SRAM_LB_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SRAM_UB_GPIO_Port,SRAM_UB_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_WE_Pin,GPIO_PIN_SET);
}



void set_addr(uint32_t addr)
{
			HAL_GPIO_WritePin(SRAM_A0_GPIO_Port,SRAM_A0_Pin,(GPIO_PinState)(addr&(1<<0)));
			HAL_GPIO_WritePin(SRAM_A1_GPIO_Port,SRAM_A1_Pin,(GPIO_PinState)(addr&(1<<1)));
			HAL_GPIO_WritePin(SRAM_A2_GPIO_Port,SRAM_A2_Pin,(GPIO_PinState)(addr&(1<<2)));
			HAL_GPIO_WritePin(SRAM_A3_GPIO_Port,SRAM_A3_Pin,(GPIO_PinState)(addr&(1<<3)));
			HAL_GPIO_WritePin(SRAM_A4_GPIO_Port,SRAM_A4_Pin,(GPIO_PinState)(addr&(1<<4)));
			HAL_GPIO_WritePin(SRAM_A5_GPIO_Port,SRAM_A5_Pin,(GPIO_PinState)(addr&(1<<5)));
			HAL_GPIO_WritePin(SRAM_A6_GPIO_Port,SRAM_A6_Pin,(GPIO_PinState)(addr&(1<<6)));
			HAL_GPIO_WritePin(SRAM_A7_GPIO_Port,SRAM_A7_Pin,(GPIO_PinState)(addr&(1<<7)));
			HAL_GPIO_WritePin(SRAM_A8_GPIO_Port,SRAM_A8_Pin,(GPIO_PinState)(addr&(1<<8)));
			HAL_GPIO_WritePin(SRAM_A9_GPIO_Port,SRAM_A9_Pin,(GPIO_PinState)(addr&(1<<9)));
			HAL_GPIO_WritePin(SRAM_A10_GPIO_Port,SRAM_A10_Pin,(GPIO_PinState)(addr&(1<<10)));
}


unsigned short read_data( uint32_t addr )
{
		unsigned short sramdata;
	
		set_addr(addr);
		HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_WE_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(SRAM_OE_GPIO_Port,SRAM_OE_Pin,GPIO_PIN_RESET);
	
		sramdata = HAL_GPIO_ReadPin(SRAM_IO0_GPIO_Port,SRAM_IO0_Pin) << 0 |
							HAL_GPIO_ReadPin(SRAM_IO1_GPIO_Port,SRAM_IO1_Pin) << 1 |
							HAL_GPIO_ReadPin(SRAM_IO2_GPIO_Port,SRAM_IO2_Pin) << 2 |
							HAL_GPIO_ReadPin(SRAM_IO3_GPIO_Port,SRAM_IO3_Pin) << 3 |
							HAL_GPIO_ReadPin(SRAM_IO4_GPIO_Port,SRAM_IO4_Pin) << 4 |
							HAL_GPIO_ReadPin(SRAM_IO5_GPIO_Port,SRAM_IO5_Pin) << 5 |
							HAL_GPIO_ReadPin(SRAM_IO6_GPIO_Port,SRAM_IO6_Pin) << 6 |
							HAL_GPIO_ReadPin(SRAM_IO7_GPIO_Port,SRAM_IO7_Pin) << 7 |
							HAL_GPIO_ReadPin(SRAM_IO8_GPIO_Port,SRAM_IO8_Pin) << 8 |
							HAL_GPIO_ReadPin(SRAM_IO9_GPIO_Port,SRAM_IO9_Pin) << 9 |
							HAL_GPIO_ReadPin(SRAM_IO10_GPIO_Port,SRAM_IO10_Pin) << 10 |
							HAL_GPIO_ReadPin(SRAM_IO11_GPIO_Port,SRAM_IO11_Pin) << 11 |
							HAL_GPIO_ReadPin(SRAM_IO12_GPIO_Port,SRAM_IO12_Pin) << 12 |
							HAL_GPIO_ReadPin(SRAM_IO13_GPIO_Port,SRAM_IO13_Pin) << 13 |
							HAL_GPIO_ReadPin(SRAM_IO14_GPIO_Port,SRAM_IO14_Pin) << 14 |
							HAL_GPIO_ReadPin(SRAM_IO15_GPIO_Port,SRAM_IO15_Pin) << 15;
							
		HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_OE_Pin,GPIO_PIN_SET);
							
	  return sramdata;
}

unsigned short write_data( uint32_t addr ,unsigned short data)
{
		unsigned short sramdata;
	
		set_addr(addr);
	

		HAL_GPIO_WritePin(SRAM_IO0_GPIO_Port,SRAM_IO0_Pin,  (GPIO_PinState)(data&(1<<0)));
		HAL_GPIO_WritePin(SRAM_IO1_GPIO_Port,SRAM_IO1_Pin,  (GPIO_PinState)(data&(1<<1)));
		HAL_GPIO_WritePin(SRAM_IO2_GPIO_Port,SRAM_IO2_Pin,  (GPIO_PinState)(data&(1<<2)));
		HAL_GPIO_WritePin(SRAM_IO3_GPIO_Port,SRAM_IO3_Pin,  (GPIO_PinState)(data&(1<<3)));
		HAL_GPIO_WritePin(SRAM_IO4_GPIO_Port,SRAM_IO4_Pin,  (GPIO_PinState)(data&(1<<4)));
		HAL_GPIO_WritePin(SRAM_IO5_GPIO_Port,SRAM_IO5_Pin,  (GPIO_PinState)(data&(1<<5)));
		HAL_GPIO_WritePin(SRAM_IO6_GPIO_Port,SRAM_IO6_Pin,  (GPIO_PinState)(data&(1<<6)));
		HAL_GPIO_WritePin(SRAM_IO7_GPIO_Port,SRAM_IO7_Pin,  (GPIO_PinState)(data&(1<<7)));
		HAL_GPIO_WritePin(SRAM_IO8_GPIO_Port,SRAM_IO8_Pin,  (GPIO_PinState)(data&(1<<8)));
		HAL_GPIO_WritePin(SRAM_IO9_GPIO_Port,SRAM_IO9_Pin,  (GPIO_PinState)(data&(1<<9)));
		HAL_GPIO_WritePin(SRAM_IO10_GPIO_Port,SRAM_IO10_Pin,  (GPIO_PinState)(data&(1<<10)));
		HAL_GPIO_WritePin(SRAM_IO11_GPIO_Port,SRAM_IO11_Pin,  (GPIO_PinState)(data&(1<<11)));
		HAL_GPIO_WritePin(SRAM_IO12_GPIO_Port,SRAM_IO12_Pin,  (GPIO_PinState)(data&(1<<12)));
		HAL_GPIO_WritePin(SRAM_IO13_GPIO_Port,SRAM_IO13_Pin,  (GPIO_PinState)(data&(1<<13)));
		HAL_GPIO_WritePin(SRAM_IO14_GPIO_Port,SRAM_IO14_Pin,  (GPIO_PinState)(data&(1<<14)));
		HAL_GPIO_WritePin(SRAM_IO15_GPIO_Port,SRAM_IO15_Pin,  (GPIO_PinState)(data&(1<<15)));
	
	
	  HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_WE_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(SRAM_OE_GPIO_Port,SRAM_OE_Pin,GPIO_PIN_RESET);
	
		HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_OE_Pin,GPIO_PIN_SET);
							
	  return sramdata;
}


void sram_test()
{
	unsigned short buf[512];
	unsigned short buf_read[512]; 
	
	for(int i = 0;i<512;i++){
			buf[i] = i;
	}
		
	sram_write(0,buf,512);
	sram_read(0,buf_read,512);
	
	for(int i = 0;i<512;i++){
		printf("%d\r\n",(int)buf_read[i]);
	}
}


void sram_read(int p, unsigned short* buf,int len)
{
		sram_IO_input();
		for(int i = 0;i<len ;i++){
			buf[i] =  read_data( p+i );
		}
}


void sram_write(int p,unsigned short* buf,int len)
{
		sram_IO_output();
		for(int i = 0;i<len ;i++){
			 //write_data( p+i ,buf[i]);
			write_data( p+i ,i%5);
		}
}


//获取随机数
// int p                 读取内存位置
// unsigned char* buf    读取数据存放位置
// int len               读取长度
// int time              断电~通电时间
void sram_read_random(int p, unsigned short* buf,int len,int time)
{
		//sram断电
		SRAM_PWR_L();
		sram_GPIO_DeInit();
		osDelay(time);
	
		//sram上电
		SRAM_PWR_H();
	  MX_GPIO_Init();

		sram_read(0, buf,len);
		for(int i = 0;i<len;i++){
			printf("%d\r\n",(int)buf[i]);
		}
		

}

	
	
