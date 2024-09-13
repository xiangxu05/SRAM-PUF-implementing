#ifndef __SRAM_H
#define __SRAM_H
#include <stdint.h>
#include "task_usb_recv.h"
#include "bch.h"

#define DELAY_TIME 100
void sram_pin_init(void);
void sram_write(int p,unsigned short* buf,int len);
void sram_read(int p, unsigned short* buf,int len);
void sram_test(void);
void sram_read_random(int p, unsigned short* buf,int len,int time);

//SRAM PUF初始化操作
void SPI_SRAM_PUF_Init(void);

//SRAM PUF输出函数
void SPI_SRAM_PUF_Output(uint32_t *sramData);
#endif



