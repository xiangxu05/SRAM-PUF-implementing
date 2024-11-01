#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <stdint.h>
#include "usbd_cdc_if.h"
#include "task_usb_recv.h"
#include "status.h"
#include "sram_func.h"
#include "sram_spi.h"

extern int flag;

void check_command(struct usb_aRxBuffer_t* usb_data, int len, Status_t* status);

#endif // COMMAND_HANDLER_H
