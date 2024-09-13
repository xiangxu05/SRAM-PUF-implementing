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
#include "BspTime2.h"
#include "tim.h"
volatile int g_Tim2Array[(uint16_t)eTimMax] = {0,};


void BspTim2Close(void)
{
			__HAL_TIM_DISABLE(&htim2);
			__HAL_TIM_DISABLE_IT(&htim2, TIM_IT_UPDATE);

}

/*******************************************************************************
* Function Name :void TIM2_IRQHandler(void)
* Description   :tim2中断函数
* Input         :
* Output        :
* Other         :
* Date          :2013.01.27
*******************************************************************************/
void TIM2_IRQHandler_cbk(void)
{
    uint16_t i = 0;

    for (i = 0; i < (uint16_t)eTimMax; i++)
    {
        g_Tim2Array[i]++;
    }
}


/********************** END ***************************************************/

