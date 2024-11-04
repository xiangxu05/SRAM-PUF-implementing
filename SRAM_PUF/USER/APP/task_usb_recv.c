/*****************************************************
功能: 串口1接收解析
			
说明:	
******************************************************/
#include "task_usb_recv.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "cmsis_armcc.h"
#include "stdio.h"
#include "w25xx_SPI.h"
#include "spi.h"
#include "usbd_cdc_if.h"

struct usb_aRxBuffer_t usb_data;

//usb数据接收解析
void task_usb_rx(void const * argument)
{		
		Status_t status = STATUS_DEFAULT;
    while(1){
        if( xQueueReceive(usbMsg, &usb_data, 1000) == pdPASS ){
							check_command(&usb_data, usb_data.len, &status);
							execute_state_action(status, &usb_data);
        }
			}
  /* USER CODE END StartDefaultTask */
}
