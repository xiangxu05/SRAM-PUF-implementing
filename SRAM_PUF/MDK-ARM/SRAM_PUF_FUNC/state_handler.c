#include "state_handler.h"
#include <stdio.h>
#include <string.h>

void execute_state_action(Status_t status, struct usb_aRxBuffer_t* usb_data) {
    switch (status) {
        case STATUS_DEFAULT:
            usb_send(usb_data->buf, usb_data->len);
            break;
        case STATUS_INIT:
            // 状态初始化操作
            break;
        case STATUS_SOURCE:
            // 查看 HelpData 值
            break;
        case STATUS_STRONG_SRAM:
//            SPI_SRAM_PUF_STRONG(usb_data->buf, usb_data->len);
            break;
        case STATUS_SOURCE_SRAM:
            // SRAM 源值处理
            break;
        case STATUS_DELAY:
//            int delay_time = 1;
//            if (convert_usb_data_to_int(usb_data, &delay_time) == -1) {
//                delay_time = 100;
//            }
//            sram_read_random(0, buf_random, sizeof(buf_random) / sizeof(buf_random[0]), delay_time);
//            buf_random_str[0] = 0;
//            for (int i = 0; i < sizeof(buf_random) / sizeof(buf_random[0]); i++) {
//                sprintf(buf_random_str + strlen((const char*)buf_random_str), "%04x", (int)buf_random[i]);
//            }
//            usb_send((unsigned char*)buf_random_str, strlen(buf_random_str));
            break;
        case STATUS_FILE:
//            SPI_SRAM_PUF_FILE(usb_data->buf, usb_data->len);
            break;
        case STATUS_STATU:
            // 其他状态处理
            break;
        default:
            break;
    }
}
