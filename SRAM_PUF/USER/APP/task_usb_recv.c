/*****************************************************
功能: 串口1接收解析
			
说明:	
******************************************************/
#include "task_usb_recv.h"


static unsigned char status = 5;
static int flag = 0;
struct usb_aRxBuffer_t usb_data;
static unsigned short buf_random[64];
void usb_send(uint8_t* Buf, uint16_t Len)
{
		while(CDC_Transmit_FS(Buf, Len) == USBD_BUSY)
			osDelay(1);
}

void output(const char* str) {
    uint16_t len = (uint16_t)strlen(str);  // 获取字符串的长度
    usb_send((uint8_t*)str, len);          // 调用 usb_send 函数发送字符串
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

/*
	接收到数据后，先检查是否切换模式
*/
void check_command(struct usb_aRxBuffer_t* usb_data, int len, unsigned char* status){
		if (len <= 0 || len > APP_RX_DATA_SIZE) {
        *status = STATUS_UNKNOWN;
        return;
    }
		unsigned char *command = usb_data->buf;
		flag = 1;
		// 匹配指令集中的命令
    if (strncmp((const char*)command, AT_DEFAULT, len) == 0) {
        *status = STATUS_DEFAULT;  // 设置 status 为 0
				//output("默认回传模式\n");
				printf("默认回传模式\n");//两种方法均可
    } else if (strncmp((const char*)command, AT_INIT, len) == 0) {
        output("执行PUF初始化操作\n");
        *status = STATUS_INIT;  // 设置 status 为 1
				SPI_SRAM_PUF_Init();
    } else if (strncmp((const char*)command, AT_SOURCE, len) == 0) {
			output("查看helpdata值:\n");
			uint32_t tmp[32];
			W25_Flash_Read(0,(uint8_t *) tmp , 4*32);
			char buf[4*32];
			buf[0] = 0;
			for(int i = 0 ; i<32;i++){
				sprintf(buf + strlen((const char*)buf),"%08x",tmp[i]);
			}
			usb_send((unsigned char*)buf,strlen((const char*)buf));
        *status = STATUS_SOURCE;  // 设置 status 为 2
    } else if (strncmp((const char*)command, AT_STRONG_SRAM, len) == 0) {
        output("执行强PUF模式，输出1024位\n");
        *status = STATUS_STRONG_SRAM;  // 设置 status 为 3
    } else if (strncmp((const char*)command, AT_SOURCE_SRAM, len) == 0) {
        output("获取SRAM单元位置上的原始值\n");
				sram_source_Sram(0, buf_random,sizeof(buf_random)/sizeof(buf_random[0]),DELAY_TIME);
        *status = STATUS_SOURCE_SRAM;  // 设置 status 为 4
    } else if (strncmp((const char*)command, AT_DELAY, strlen(AT_DELAY)) == 0) {
        output("时延测试模式\n");
        *status = STATUS_DELAY;  // 设置 status 为 5
    } else if(strncmp((const char*)command, AT_FILE, strlen(AT_FILE)) == 0){
				output("文件管理模式\n");
				output("说明：此模式输入格式为(用户标签，文件名标签)，输出对应的KEY");
				*status = STATUS_FILE;
		}else {
				flag = 0;
        // 如果命令不匹配任何已知指令
        //output("未知命令\n");
        //*status = STATUS_UNKNOWN;  // 设置 status 为 255，表示未知命令
    }
		
}
//usb数据接收解析
static char buf_random_str[20*1024];
void task_usb_rx(void const * argument)
{
    while(1){
        if( xQueueReceive(usbMsg, &usb_data, 1000) == pdPASS ){
					
					/*
					//usb发送方法测试
					//usb_send((uint8_t*)"recv--> \r\n",strlen("recv--> \r\n"));
					//usb_send(usb_data.buf, usb_data.len);
					*/
					
					check_command(&usb_data,usb_data.len,&status);
					if(!flag){
					switch(status){
						case 0:{
							usb_send(usb_data.buf, usb_data.len);
							break;
						}
						case 1:{
							break;
						}
						case 2:{
							break;
						}
						case 3:{
							SPI_SRAM_PUF_STRONG(usb_data.buf,usb_data.len);
							break;
						}
						case 4:{
							break;
						}
						case 5:{
							int delay_time = 1;
							if(convert_usb_data_to_int(&usb_data,&delay_time)==-1){
								delay_time = 100;
							}
							//char char_time[100];
							//sprintf(char_time,"%d",delay_time);
							//output(char_time);
							
							sram_read_random(0, buf_random,sizeof(buf_random)/sizeof(buf_random[0]),delay_time);

							buf_random_str[0] = 0;		
							for(int i = 0;i < sizeof(buf_random)/sizeof(buf_random[0]);i++){
								sprintf(buf_random_str + strlen((const char*)buf_random_str),"%04x",(int)buf_random[i]);
							}
							usb_send((unsigned char*)buf_random_str,strlen((const char*)buf_random_str));
							break;
						}
						case 6:{
							SPI_SRAM_PUF_FILE(usb_data.buf,usb_data.len);
							break;
						}
						default:
							break;
					}
				}
					/*
					int delay_time = 1;
					if(convert_usb_data_to_int(&usb_data,&delay_time)!=0){
						delay_time = 100;
					}
					sram_read_random(0, buf_random,sizeof(buf_random)/sizeof(buf_random[0]),delay_time);
					buf_random_str[0] = 0;		
					for(int i = 0;i < sizeof(buf_random)/sizeof(buf_random[0]);i++){
						sprintf(buf_random_str + strlen((const char*)buf_random_str),"%04x",(int)buf_random[i]);
					}
					usb_send((unsigned char*)buf_random_str,strlen((const char*)buf_random_str));
        }  */
      else{
						
      }
    }
	}
  /* USER CODE END StartDefaultTask */
}
