#ifndef _HASH_H
#define _HASH_H

#include "system.h"
#include "stm32f4xx_hash.h"

#define HASH_OUTPUT_SIZE 32

void HASH_Config(void);
void HASH_SHA256(uint8_t *input, uint32_t length, uint8_t *output);


#endif
