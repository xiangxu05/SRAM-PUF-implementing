#include "stdio.h"	
#include <string.h>
#include "BspUsart1.h"
#include "main.h"
#include "usart.h"


void (*receChar)(uint8_t ch) = NULL;
static SerialBuffType m_SendBuff = SerialBuffDefault();
static SerialBuffType *sb = &m_SendBuff;


static char volatile eTXIdle = 1;



void BspUsart1Close(void)
{
      HAL_UART_MspDeInit(&huart1);     
}


/*******************************************************************************
* Function Name :s32 BspUsartSend(uint8_t* buf, u32 len)
* Description   :发送一组字符串
* Input         :*buf:字符串指针。len:长度
* Output        :
* Other         :
* Date          :2012.05.11  11:45:38
*******************************************************************************/
uint16_t BspUsart1Send(uint8_t *buf, uint16_t len)
{
		for(int i = 0;i<len;i++){
			while((USART1->SR&0X80)==0);//循环发送,直到发送完毕   
			USART1->DR =  buf[i];  	
		}
    return 1; 
}



/*******************************************************************************
* Function Name :uint8_t BspUsart1IRQCallBack(void *fun)
* Description   :设置串口回调函数
* Input         :
* Output        :
* Other         :
* Date          :2013.02.19
*******************************************************************************/
void BspUsart1IRQCallBack(void *fun)
{
    receChar = (void (*)(uint8_t))fun;
}

/*******************************************************************************
* Function Name :void USART1_IRQHandler(void)
* Description   :USART1 中断程序
* Input         :
* Output        :
* Other         :
* Date          :2011.11.16  16:57:39
*******************************************************************************/
void USART1_IRQHandler_cbk(void)
{
		if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)!=RESET))
    {
        if (sb->len > 0 )
        {
            USART3->DR = sb->buf[sb->ind++];
            sb->len--;
        }
        else
        {
            __HAL_UART_DISABLE_IT(&huart1,UART_FLAG_TXE);
            __HAL_UART_DISABLE_IT(&huart1,UART_FLAG_TC);
        }
    }
    if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)!=RESET))
    {
        __HAL_UART_CLEAR_FLAG(&huart1,UART_FLAG_TC);
        __HAL_UART_DISABLE_IT(&huart1,UART_FLAG_TC);
        sb->len = 0;
        eTXIdle = 1;
    }
    if((__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)!=RESET))
    {
        uint8_t ch;
        ch = (uint8_t)(huart1.Instance->DR&(uint8_t)0x00FF);
        __HAL_UART_DISABLE_IT(&huart1,UART_FLAG_RXNE);
        if (receChar != NULL)
            (*receChar)(ch);
    }	
}



/********************** END ***************************************************/


