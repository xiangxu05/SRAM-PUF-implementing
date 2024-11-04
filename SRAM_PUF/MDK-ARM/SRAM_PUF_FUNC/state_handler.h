#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include "command_handler.h"
#include "status.h"
#include <ctype.h>  // 用于 isdigit 函数
#include <limits.h> // 用于 INT_MAX 和 INT_MIN

void execute_state_action(Status_t status, struct usb_aRxBuffer_t* usb_data);

#endif // STATE_HANDLER_H
