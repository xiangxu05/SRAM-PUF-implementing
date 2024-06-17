#ifndef __usart_H
#define __usart_H

#include "system.h"
#include "stdio.h"
#include "sram.h"
#include "sha512.h"

#define BUFFER_SIZE 512

void USART1_Init(u32 bound);

//�Ƚ��ַ�������
int string_compare(const char* str1, const char* str2);

//�������鳤��
u32 calculate_length(u8 *input);
#endif


