#ifndef _DrvTime2_h_
#define _DrvTime2_h_

//#include "stm32f10x_type.h"



#ifndef NULL
#define NULL        ((void *)0)
#endif





enum {
	eTim1,
	eTim2,
	eWdog,
	eTimUpdata,
	eTimYModem,
	eTimMax,
};



//如果index对应的时间超时，清零并返回1，未超时返回0
#define IS_TIMEOUT_1MS(index, count)    ((g_Tim2Array[(unsigned short)(index)] >= (count))?  \
                                        ((g_Tim2Array[(unsigned short)(index)] = 0) == 0): 0)


extern volatile int g_Tim2Array[(unsigned short)eTimMax];

void BspTim2Init(void);
void BspTim2Close(void);
void TIM2_IRQHandler_cbk(void);



#endif
/*********************************** END **************************************/


