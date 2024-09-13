#ifndef __TASK_USB_RECV__
#define __TASK_USB_RECV__

#define AT_DEFAULT      "AT_default\r\n"
#define AT_INIT         "AT_init\r\n"
#define AT_SOURCE       "AT_source\r\n"
#define AT_STRONG_SRAM  "AT_strongSram\r\n"
#define AT_SOURCE_SRAM  "AT_sourceSram\r\n"
#define AT_DELAY        "AT_delay\r\n"

#define STATUS_DEFAULT      0
#define STATUS_INIT         1
#define STATUS_SOURCE       2
#define STATUS_STRONG_SRAM  3
#define STATUS_SOURCE_SRAM  4
#define STATUS_DELAY        5
#define STATUS_UNKNOWN      255  // Î´ÖªÃüÁî

extern void task_usb_rx(void const * argument);


#endif
