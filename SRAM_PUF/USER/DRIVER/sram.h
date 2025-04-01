#ifndef __SRAM_H
#define __SRAM_H
#include <stdint.h>
#include "task_usb_recv.h"
#include "bch.h"
#include "w25xx_SPI.h"
#include "sha512.h"
#include "main.h"

#define DELAY_TIME 100
extern statuInfo_union statuMessage;

void sram_pin_init(void);
void sram_write(int p,unsigned short* buf,int len);
void sram_read(int p, unsigned short* buf,int len);
void sram_test(void);
void sram_read_random(int p, unsigned short* buf,int len,int time);

//SRAM PUF初始化操作
void SPI_SRAM_PUF_Init(void);

//SRAM PUF输出函数
void SPI_SRAM_PUF_Output(uint32_t *sramData);

//选取的SRAM单元位置上的原始值
void sram_source_Sram(int p, unsigned short* buf,int len,int time);
	
//读取SRAM PUF稳定输出
void SPI_SRAM_PUF_Stable_Output(uint32_t *Data);

//Strong PUF模式
void SPI_SRAM_PUF_STRONG(unsigned char* Messages,int len);

//文件管理模式
void SPI_SRAM_PUF_FILE(unsigned char* Infos,int len);

//对文件的标签进行操作
void SPI_SRAM_PUF_FILE_LABEL(uint32_t *Files,int len,uint32_t *helpData);
#endif



