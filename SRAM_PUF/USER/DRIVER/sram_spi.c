#include "sram_spi.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"


struct instruction_t{
	#pragma pack(1)
	char cmd[1];
	unsigned char addrH;	
	unsigned char addrL;	
};


void sram_test()
{
	unsigned char buf[1024];
	unsigned char buf_read[1024]; 
	
	for(int i = 0;i<254;i++){
		for(int j = 0;j<4;j++){
			buf[i*4 + j] = i;
		}
	}
		
	sram_write(0,buf,1024);
	sram_read(0,buf_read,1024);
	
	for(int i = 0;i<1024;i++){
		printf("%d\r\n",(int)buf_read[i]);
	}
}

void sram_read(int p, unsigned char* buf,int len)
{
		struct instruction_t instruction = {
			.cmd = 3,
		};
		
		
		for(int i = 0;i<len;i++){
			
			instruction.addrH = (p + i)/0xff ;
			instruction.addrL = (p + i)&0xff ;
			
			SRAM_CS_L();
			HAL_SPI_Transmit(&hspi1, (unsigned char*)&instruction, 3,  20);      /*发送数据*/
			HAL_SPI_Receive(&hspi1, &buf[i], 1,  20);/*接收数据*/
			SRAM_CS_H();
			
		}
}


void sram_write(int p,unsigned char* buf,int len)
{
		struct instruction_t instruction = {
			.cmd = 2,
		};
		
		for(int i = 0;i<len;i++){
			instruction.addrH = (p + i)/0xff ;
			instruction.addrL = (p + i)&0xff ;
			SRAM_CS_L();
			HAL_SPI_Transmit(&hspi1, (unsigned char*)&instruction, 3,  20);      /*发送数据*/
			HAL_SPI_Transmit(&hspi1, &buf[i], 1,  20);/*接收数据*/
			SRAM_CS_H();
			
		}
}


//获取随机数
// int p                 读取内存位置
// unsigned char* buf    读取数据存放位置
// int len               读取长度
// int time              断电~通电时间
void sram_read_random(int p, unsigned char* buf,int len,int time)
{
		//sram断电
		SRAM_PWR_L();
		SRAM_CS_L();
		HAL_SPI_MspDeInit(&hspi1);
		osDelay(time);		//延时时间短数据不变
		
		//sram上电
		SRAM_PWR_H();
		SRAM_CS_H();
		osDelay(1);
		SRAM_CS_L();
		HAL_SPI_MspInit(&hspi1);
	
		sram_read(0, buf,len);
		
		SRAM_PWR_L();
		SRAM_CS_L();
//		for(int i = 0;i<len;i++){
//			printf("%d\r\n",(int)buf[i]);
//		}
}

	
	
