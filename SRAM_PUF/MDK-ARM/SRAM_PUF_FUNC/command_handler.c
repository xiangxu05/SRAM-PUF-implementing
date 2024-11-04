#include "command_handler.h"
#include <string.h>
#include <stdio.h>
#include <flashdb.h>

// AT指令的定义
#define AT_DEFAULT      "AT_default\r\n"
#define AT_INIT         "AT_init\r\n"
#define AT_SOURCE       "AT_source\r\n"
#define AT_STRONG_SRAM  "AT_strongSram\r\n"
#define AT_SOURCE_SRAM  "AT_sourceSram\r\n"
#define AT_DELAY        "AT_delay\r\n"
#define AT_FILE         "AT_File\r\n"
#define AT_STATU        "AT_Statu\r\n"
#define AT_DBINIT       "AT_initDB\r\n"

statuInfo_union statuMessage;
int flag1;

void usb_send(uint8_t* Buf, uint16_t Len)
{
		while(CDC_Transmit_FS(Buf, Len) == USBD_BUSY)
		osDelay(1);
}

// 指令处理函数的实现
void handleDefaultMode(void) {
    printf("随机输出值模式\n");
}

void handleInitMode(void) {
    printf("执行PUF初始化操作\n");
    SPI_SRAM_PUF_Init();
}

void handleSourceMode(void) {
    printf("查看helpdata值:\n");
    uint32_t tmp[32];
    W25_Flash_Read(0, (uint8_t*)tmp, 4 * 32);
    char buf[4 * 32] = {0};
    for (int i = 0; i < 32; i++) {
        sprintf(buf + strlen(buf), "%08x", tmp[i]);
    }
    usb_send((unsigned char*)buf, strlen(buf));
}

void handleStrongSramMode(void) {
    printf("执行强PUF模式，输出1024位\n");
}

void handleSourceSramMode(void) {
    printf("获取SRAM单元位置上的原始值\n");
		unsigned char buf[128];
    sram_read_random(0, buf, sizeof(buf), DELAY_TIME);
		for(int i = 0 ; i < 128 ; i++){
			printf("%x",buf[i]);
		}
}

void handleDelayMode(void) {
    printf("时延测试模式\n");
}

void handleFileMode(void) {
    printf("文件管理模式\n说明：此模式输入格式为(用户标签，文件名标签)，输出对应的KEY\n");
}

void handleStatuMode(Status_t* status) {
		printf("查看状态");
    statuMessage.data.status = *status;
    usb_send(statuMessage.bytes, sizeof(statuMessage.bytes));
}

void handleDBInitMode(void) {
		printf("初始化DB数据库");
}

// 命令映射表，将命令字符串与对应处理函数和状态关联
typedef struct {
    const char *command;
    void (*handler)(void);       // 指向处理函数的指针
    Status_t nextStatus;         // 切换到的状态
} CommandHandler;

CommandHandler commandHandlers[] = {
    {AT_DEFAULT, handleDefaultMode, STATUS_DEFAULT},
    {AT_INIT, handleInitMode, STATUS_INIT},
    {AT_SOURCE, handleSourceMode, STATUS_SOURCE},
    {AT_STRONG_SRAM, handleStrongSramMode, STATUS_STRONG_SRAM},
    {AT_SOURCE_SRAM, handleSourceSramMode, STATUS_SOURCE_SRAM},
    {AT_DELAY, handleDelayMode, STATUS_DELAY},
    {AT_FILE, handleFileMode, STATUS_FILE},
    {AT_STATU, (void (*)(void))handleStatuMode, STATUS_STATU},
		{AT_DBINIT, handleDBInitMode, STATUS_DBINIT}
};
const int commandHandlerCount = sizeof(commandHandlers) / sizeof(CommandHandler);

// check_command 函数：解析命令并根据结果切换状态
void check_command(struct usb_aRxBuffer_t* usb_data, int len, Status_t* status) {
    if (len <= 0 || len > APP_RX_DATA_SIZE) {
        *status = STATUS_UNKNOWN;
        return;
    }

    unsigned char *command = usb_data->buf;
		flag1 = 1;
		
    // 遍历命令处理映射表，找到对应指令并执行
    for (int i = 0; i < commandHandlerCount; i++) {
        if (strncmp((const char*)command, commandHandlers[i].command, len) == 0) {
            commandHandlers[i].handler();
            *status = commandHandlers[i].nextStatus;
						statuMessage.data.status = *status;
            return;
        }
    }
		
		flag1 = 0;
//    *status = STATUS_UNKNOWN;  // 未知命令
}
