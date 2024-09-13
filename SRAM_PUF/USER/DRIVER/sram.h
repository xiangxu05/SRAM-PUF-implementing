#ifndef __SRAM_H
#define __SRAM_H

void sram_pin_init(void);
void sram_write(int p,unsigned short* buf,int len);
void sram_read(int p, unsigned short* buf,int len);
void sram_test(void);
void sram_read_random(int p, unsigned short* buf,int len,int time);
#endif



