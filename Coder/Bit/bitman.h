#ifndef _BIT_H
#define _BIT_H

#include "system.h"
#include <stdbool.h>

void xor_be_bit(u8 *buf,int pos,bool val);//�԰���ˣ�Big Endian��˳���λ���в������������Чλ�������
void xor_le_bit(u8 *buf, int pos, bool val);//�԰�С�ˣ�Little Endian��˳���λ���в������������Чλ�����ұ�
void set_be_bit(u8 *buf, int pos, bool val);//�����˳������λ
void set_le_bit(u8 *buf, int pos, bool val);//��С��˳������λ
bool get_be_bit(const u8 *buf, int pos);//�����˳���ȡλ
bool get_le_bit(const u8 *buf, int pos);//��С��˳���ȡλ

#endif
