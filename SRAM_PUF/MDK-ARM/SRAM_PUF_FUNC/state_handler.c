#include "state_handler.h"
#include <stdio.h>
#include <string.h>
#define min(a, b) ((a) < (b) ? (a) : (b))

extern int flag1;

int convert_usb_data_to_int(struct usb_aRxBuffer_t* usb_data, int* result);

void execute_state_action(Status_t status, struct usb_aRxBuffer_t* usb_data) {
		if(flag1)
			return;
    switch (status) {
        case STATUS_DEFAULT:{
						unsigned char buf[128];
						sram_read_random(0, buf, sizeof(buf), DELAY_TIME);
						int n = min(usb_data->len,128);
						for(int i = 0 ; i < 128 ; i++){
							if(i<n){
								printf("%x",buf[i]^usb_data->buf[i]);
							}
							else{
								printf("%x",buf[i]);
							}
						}
            usb_send(usb_data->buf, usb_data->len);
            break;
				}
        case STATUS_INIT:
            // 状态初始化操作
            break;
        case STATUS_SOURCE:
            // 查看 HelpData 值
            break;
        case STATUS_STRONG_SRAM:
            SPI_SRAM_PUF_STRONG(usb_data->buf, usb_data->len);
            break;
        case STATUS_SOURCE_SRAM:
            // SRAM 源值处理
            break;
        case STATUS_DELAY:{
            int delay_time = 1;
						unsigned char buf_random[1024];
            if (convert_usb_data_to_int(usb_data, &delay_time) == -1) {
                delay_time = 100;
            }
            sram_read_random(0, buf_random, sizeof(buf_random) / sizeof(buf_random[0]), delay_time);
            usb_send((uint8_t*)buf_random, sizeof(buf_random));
            break;
					}
        case STATUS_FILE:
            SPI_SRAM_PUF_FILE(usb_data->buf, usb_data->len);
            break;
        case STATUS_STATU:
            // 其他状态处理
            break;
				case STATUS_DBINIT:
						
						break;
        default:
            break;
    }
}

int convert_usb_data_to_int(struct usb_aRxBuffer_t* usb_data, int* result)
{
    int i = 0;
    long long num = 0; // 使用 long long 来处理可能的溢出
    *result = 0;       // 初始化返回结果

    // 检查缓冲区中是否为纯数字
    while (usb_data->buf[i] != '\0' && i < usb_data->len) {
        if (!isdigit(usb_data->buf[i])) {
            return 0; // 如果发现非数字字符，返回错误
        }

        // 将字符转换为数字，并累加到 num 中
        num = num * 10 + (usb_data->buf[i] - '0');

        // 检查是否发生溢出
        if (num > INT_MAX) {
            return -1; // 溢出，返回错误
        }else if(num>1000){
						num = 1000;
						*result = (int)num;
					return 0;
				}

        i++;
    }

    // 如果缓冲区中没有数字或全是空字符，返回错误
    if (i == 0) {
        return -1;
    }

    *result = (int)num; // 将 num 转换为 int 类型

    return 0; // 转换成功
}
