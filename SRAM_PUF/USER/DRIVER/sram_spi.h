#ifndef __SRAM_SPI_H
#define __SRAM_SPI_H


void sram_write(int p,unsigned char* buf,int len);
void sram_read(int p, unsigned char* buf,int len);
void sram_test(void);
void sram_read_random(int p, unsigned char* buf,int len,int time);
#endif



