#ifndef __TASK_USB_RECV__
#define __TASK_USB_RECV__

#include "main.h"
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "cmsis_armcc.h"
#include "command_handler.h"
#include "state_handler.h"


extern void task_usb_rx(void const * argument);
void usb_send(uint8_t* Buf, uint16_t Len);

#endif
