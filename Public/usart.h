#ifndef __usart_H
#define __usart_H

#include "system.h"
#include "stdio.h"
#include "sram.h"

#define BUFFER_SIZE 256

void USART1_Init(u32 bound);
int string_compare(const char* str1, const char* str2);

#endif


