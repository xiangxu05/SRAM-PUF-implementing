#ifndef _CRC_H
#define _CRC_H

#include "system.h"

typedef struct {
    u16 crc;
    u16 poly;
} CRC_Implement;

void CRC_Init(CRC_Implement *ctx, u16 poly, u16 crc);
void CRC_Reset(CRC_Implement *ctx, u16 crc);
u16 CRC_Update(CRC_Implement *ctx, u16 prev, uint8_t data);
u16 CRC_Process8(CRC_Implement *ctx, u8 data);
u16 CRC_Final(CRC_Implement *ctx);
u16 CRC_Calculate(const char *str, u16 poly, u16 initial_crc);
u16 CRC_Verify(char *str, u16 poly, u16 initial_crc);
#endif