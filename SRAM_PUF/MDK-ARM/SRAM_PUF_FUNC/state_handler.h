#ifndef STATE_HANDLER_H
#define STATE_HANDLER_H

#include "command_handler.h"
#include "status.h"

void execute_state_action(Status_t status, struct usb_aRxBuffer_t* usb_data);

#endif // STATE_HANDLER_H
