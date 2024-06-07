#include "crc.h"

void CRC_Init(CRC_Implement *ctx, u16 poly, u16 crc)
{
    ctx->poly = poly;
    ctx->crc = crc;
}

void CRC_Reset(CRC_Implement *ctx, u16 crc)
{
    ctx->crc = crc;
}

u16 CRC_Update(CRC_Implement *ctx, u16 prev, u8 data)
{		
		int i = 0;
    u16 tmp = prev ^ data;
    for (i = 0; i < 8; ++i) {
        if (tmp & 1)
            tmp = (tmp >> 1) ^ ctx->poly;
        else
            tmp >>= 1;
    }
    return tmp;
}

u16 CRC_Process8(CRC_Implement *ctx, u8 data)
{
    ctx->crc = CRC_Update(ctx, ctx->crc, data);
    return ctx->crc;
}

u16 CRC_Final(CRC_Implement *ctx)
{
    return ctx->crc;
}

u16 CRC_Calculate(const char *str, u16 poly, u16 initial_crc)
{		 
		u8 back = 0;
		u16 front = 0;
    CRC_Implement ctx;
    CRC_Init(&ctx, poly, initial_crc);

    while (*str != '\0') {
        CRC_Process8(&ctx, (uint8_t)*str);
        str++;
    }
		back = ctx.crc>>8;
		front = ctx.crc<<8;
		ctx.crc = front | back;
    return CRC_Final(&ctx);
}
u16 CRC_Verify(char *str, u16 poly, u16 initial_crc){
	char *fast = str+2;
	char *low = str;
	u16 Verify_crc;
	CRC_Implement ctx;
  CRC_Init(&ctx, poly, initial_crc);
	
	while(*fast != '\0'){
		fast++;
		low++;
	}
	Verify_crc = *low;
	Verify_crc = Verify_crc<<8 | *(low+1);
	*low = '\0';
	
  return (Verify_crc == CRC_Calculate(str,poly,initial_crc));
	
}