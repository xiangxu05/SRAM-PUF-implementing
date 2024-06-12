#ifndef _BIT_H
#define _BIT_H

#include "system.h"
#include <stdbool.h>

void xor_be_bit(u8 *buf,int pos,bool val);//对按大端（Big Endian）顺序的位进行操作，即最高有效位在最左边
void xor_le_bit(u8 *buf, int pos, bool val);//对按小端（Little Endian）顺序的位进行操作，即最高有效位在最右边
void set_be_bit(u8 *buf, int pos, bool val);//按大端顺序设置位
void set_le_bit(u8 *buf, int pos, bool val);//按小端顺序设置位
bool get_be_bit(const u8 *buf, int pos);//按大端顺序获取位
bool get_le_bit(const u8 *buf, int pos);//按小端顺序获取位

#endif
