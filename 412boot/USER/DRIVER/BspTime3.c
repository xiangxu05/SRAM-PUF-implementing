/*
********************************************************************************
*                               定时器驱动程序设计
*                              ARM Cortex-M3 Port
*
* File          : DrvTime3.C
* Version       : V1.0
* By            : 王宏强
*
* For           : Stm32f10x
* Mode          : Thumb2
* Toolchain     : 
*                   RealView Microcontroller Development Kit (MDK)
*                   Keil uVision
* Description   : 定时器驱动  占用定时器。定时器pwm 4路通道(跟具需要选择)
* Date          : 2012.05.22
*******************************************************************************/
#include "main.h"
#include "BspTime3.h"
//#include "stdio.h"
#include "tim.h"
static void (*IRQHandler)(void) = NULL;

void BspTim3Open(void)
{
      TIM3->CNT = 0;  
      __HAL_TIM_ENABLE(&htim3);
			__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
}

/*******************************************************************************
* Function Name :void BspTim3Close()
* Description   :关闭定时器
* Input         :
* Output        :
* Other         :
* Date          :2013.02.19
*******************************************************************************/
void BspTim3Close(void)
{	    
      __HAL_TIM_DISABLE(&htim3);
      TIM3->CNT = 0;
      __HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE);
}


void BspTim3SetIRQCallBack(void *fun)
{
    IRQHandler = (void (*)(void))fun;
}

void TIM3_IRQHandler_cbk(void)
{
        if (IRQHandler != NULL)
            (*IRQHandler)();			//中断函数
}



