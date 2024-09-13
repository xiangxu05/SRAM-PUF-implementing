/*****************************************************
功能: 串口1接收解析
			
说明:	
******************************************************/
#include "task_uart1_recv.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "cmsis_armcc.h"
#include "usart.h"
#include "stdio.h"
#include "w25xx_SPI.h"
#include "sram.h"
#include "spi.h"
#include "usbd_cdc_if.h"
struct urt_aRxBuffer_t uart1_data;
int w25_id;
static unsigned short buf_random[512];
static char flg = 10;
int ramdata;
__STATIC_INLINE void __set_FAULTMASK1(uint32_t faultMask)
{
  register uint32_t __regFaultMask       __ASM("faultmask");
  __regFaultMask = (faultMask & (uint32_t)1U);
}

//重复输入C重启，用于进入IAP更新程序
void check_goto_iap(unsigned char* buf)
{
		
		if(buf[0] == 'c' || buf[0] == 'C'){
				printf("boot %d\r\n",flg);
				if(flg-- <= 0){
						__set_FAULTMASK1(1);
						NVIC_SystemReset();
				}
		}
}



//串口数据接收解析
void task_uart1_rx(void const * argument)
{
//		w25_id = W25_Flash_ReadJedecID();
//		printf("W25q256 JedecID %x\r\n",w25_id);
//		w25_id = W25_Flash_ReadID();
//		printf("W25q256 ID %x\r\n",w25_id);
//		
//		sram_pin_init();
//		sram_test();


    while(1){
        if( xQueueReceive( usart1Msg, &uart1_data, 1000) == pdPASS ){
						check_goto_iap(uart1_data.rx_buf);

						sram_read_random(0, buf_random,512,100);	
//						for(int i = 0;i<512;i++){
//							printf("%d\r\n",(int)buf_random[i]);
//						}
        }  
        else{
            flg = 10;
        }
    }
  /* USER CODE END StartDefaultTask */
}
