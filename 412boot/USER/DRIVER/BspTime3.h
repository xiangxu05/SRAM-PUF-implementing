#ifndef _DrvTime3_h_
#define _DrvTime3_h_

//#include "stm32f10x_type.h"

#ifndef NULL
#define NULL        ((void *)0)
#endif


void BspTim3Init(void);
void BspTim3Open(void);
void BspTim3Close(void);
void BspTim3SetCounter(unsigned short count);
void BspTim3SetIRQCallBack(void *fun);
void TIM3_IRQHandler_cbk(void);

#endif
/********************** END ***************************************************/


