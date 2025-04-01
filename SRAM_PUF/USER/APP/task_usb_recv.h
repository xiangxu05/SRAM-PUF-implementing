#ifndef __TASK_USB_RECV__
#define __TASK_USB_RECV__

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "cmsis_armcc.h"
#include "stdio.h"
#include "w25xx_SPI.h"
#include "sram.h"
#include "spi.h"
#include "usbd_cdc_if.h"
#include <ctype.h>  // 用于 isdigit 函数
#include <limits.h> // 用于 INT_MAX 和 INT_MIN

#define AT_DEFAULT      "AT_default\r\n"
#define AT_INIT         "AT_init\r\n"
#define AT_SOURCE       "AT_source\r\n"
#define AT_STRONG_SRAM  "AT_strongSram\r\n"
#define AT_SOURCE_SRAM  "AT_sourceSram\r\n"
#define AT_DELAY        "AT_delay\r\n"
#define AT_FILE        "AT_File\r\n"
#define AT_STATU        "AT_Statu\r\n"

#define STATUS_DEFAULT      0
#define STATUS_INIT         1
#define STATUS_SOURCE       2
#define STATUS_STRONG_SRAM  3
#define STATUS_SOURCE_SRAM  4
#define STATUS_DELAY        5
#define STATUS_FILE         6
#define STATUS_STATU        7
#define STATUS_UNKNOWN      255  // 未知命令
#define DELAY_TIME 100

extern void task_usb_rx(void const * argument);
extern void output(const char* str);
extern void usb_send(uint8_t* Buf, uint16_t Len);
#endif
