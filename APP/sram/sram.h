#ifndef _sram_H
#define _sram_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "system.h"
#include "stmflash.h"
#include "rng.h"
#include "bch.h"
#include "usart.h"

#define STM32_FLASH_SAVE_ADDR 0x080E0000


//使用NOR/SRAM的 Bank1.sector3,地址位HADDR[27,26]=10 
//对IS61LV25616/IS62WV25616,地址线范围为A0~A17 
//对IS61LV51216/IS62WV51216,地址线范围为A0~A18
#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	

void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 NumHalfwordToRead);
void FSMC_SRAM_ReadBuffer_u32(u32* pBuffer, u32 ReadAddr, u32 n);

//sram单元读取策略
void FSMC_SRAM_PUF_Read_With_Size(u32 *data);

//SRAM PUF初始化操作
void FSMC_SRAM_PUF_Init(void);

//SRAM PUF输出函数
void FSMC_SRAM_PUF_Output(u32 *sramData);

#endif

