#ifndef _input_H
#define _input_H

#include "system.h"

extern u8 TIM5_CH1_CAPTURE_STA; //���벶���״̬
extern u32 TIM5_CH1_CAPTURE_VAL;//���벶��ֵ


void TIM5_CH1_Input_Init(u32 arr,u16 psc);
	
#endif
