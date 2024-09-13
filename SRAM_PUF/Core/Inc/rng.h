#ifndef _rng_H
#define _rng_H

#include "main.h"
#include <stdint.h>
//#include "system.h"

uint8_t RNG_Init(void);
uint32_t RNG_Get_RandomNum(void);
int RNG_Get_RandomRange(int min,int max);


#endif
