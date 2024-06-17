#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "exti.h"
#include "tftlcd.h"
#include "key.h"
#include "sram.h"
#include "OLED.h"
#include "crc.h"
#include "bch.h"
#include "rng.h"
#include "stmflash.h"
//#include "hash.h"
//#include "encoder.h"
//#include "GF.h"

#define BUFFER_SIZE 256

/* 存储功能测试用
#define STM32_FLASH_SAVE_ADDR 0x080E0000

#define TEXTLEN sizeof(text_buf)
#define TEXTSIZE (TEXTLEN/4+((TEXTLEN%4)?1:0))
*/

/*
#define N 15 // 总码长
#define K 5  // 信息位长度
#define D ((K + 7) / 8) // 数据字节数
#define P ((N - K + 7) / 8) // 校验字节数
#define L (D + P) // 总字节数
*/

char *OLED_output_start=NULL;
char *OLED_output_mid=NULL;
char *OLED_output_end=NULL;

volatile int status=0;
volatile u8 inputFlag=0;
volatile u8 buffer[BUFFER_SIZE];
volatile u16 bufferIndex = 0;

//外部内存测试
void ExSRAM_Cap_Test(){
  u8 writeData = 0xf0, readData;
	u16 cap=0;
  u32 addr;
	char output[50]; // 用于存储结果的字符数组
	
	addr = 1024; //从1KB位置开始算起
	OLED_ShowString(3,1,"ExSRAMCap: 0KB"); 
	
	while(1)
	{	
		FSMC_SRAM_ReadBuffer(&readData,addr+8,1);
		writeData = readData;
		//FSMC_SRAM_WriteBuffer(&writeData, addr, 1);
		//FSMC_SRAM_ReadBuffer(&readData,addr,1);
		
		//查看读取到的数据是否跟写入数据一样
    if(readData == writeData){	
			//printf("%c", readData);
      cap++;
      addr += 1024;
      readData = 0;
			if(cap%128 == 0){
				sprintf(output, "ExSRAMCap:%dKB", cap);
				OLED_ShowString(3,1,output);//显示内存容量 
			}
      if(addr > 1024 * 1024){//SRAM容量最大为1MB
				OLED_ShowString(4,7,"ok!");
        break;
      }    
    }
    else{
      break;
    }     
	}
}

uint32_t calculate_length(uint8_t *input) {
    uint32_t length = 0;
    while (input[length] != '\0') {
        length++;
    }
    return length;
}

void System_Init(){
	SysTick_Init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组分2 组
	LED_Init();
	USART1_Init(921600);
	KEY_Init();
	OLED_Init();
	My_EXTI_Init();
	RNG_Init();
	
	OLED_ShowString(1,5,"SRAM PUF");
	FSMC_SRAM_Init();
	ExSRAM_Cap_Test(); 
}

/* 测试函数
void assert_equal(uint32_t expected, uint32_t actual, const char *message) {
    if (expected != actual) {
        printf("FAIL: %s Expected: 0x%08X, Got: 0x%08X\n", message, expected, actual);
    }
    else {
        printf("PASS: %s\n", message);
    }
}
*/

int main()
{	
	System_Init();
	
	uint8_t data[] = "Hello, world!";
  uint8_t hash[32];
	
	uint32_t length = calculate_length(data);
	HASH_SHA256(data, length, hash);
	for(int i =0 ; i<32;i++){
		printf("%08x",hash[i]);
	}
	
	while (1)
	{
		led1=!led1;
		delay_ms(100);
	}

	
	/* //SRAM PUF基本功能测试
	u32 data[32];
	
	FSMC_SRAM_PUF_Init(); //init功能
	FSMC_SRAM_PUF_Output(data); //读稳定输出
	for(int i =0 ; i<32;i++){
		printf("%08x",data[i]);
	}
	*/
	
	/* //读出数据功能测试
	printf("写入数据：");
	STMFLASH_Read(STM32_FLASH_SAVE_ADDR,data,32);
	for(int i=0;i<49;i++){
		printf("%08x",data[i]);
	}
	*/
	
	/*存储数据功能测试
	u8 i=0;
	u8 key;
	u8 read_buf[TEXTSIZE];
	const u8 text_buf[]="www.baidu.com";
	STMFLASH_Write(STM32_FLASH_SAVE_ADDR,(u32*)text_buf,TEXTSIZE);
	printf("写入数据为：%s\r\n",text_buf);
	STMFLASH_Read(STM32_FLASH_SAVE_ADDR,(u32*)read_buf,TEXTSIZE);
	printf("读取数据为：%s\r\n",read_buf);
	*/
	
	/* 读SRAM_PUF功能测试
	u32 *data = FSMC_SRAM_PUF_Read(0);
	for(int i = 0 ;i<32;i++){
		printf("%08x ",data[i]);
	}
	*/
	
	/*
	// BCH编解码测试
	u32 cw[] = {0x12345678, 0x9ABCDEF0, 0x0FEDCBA9};
	int n = sizeof(cw) / sizeof(cw[0]);
	int new_size;
	u32 *encoded;
	int out_length;
	
	encoded = bch_encoder(cw, n, &new_size);
	if (encoded != NULL) {
        // 打印编码后的数组
        for (int i = 0; i < new_size; i++) {
            printf("%08X\n", encoded[i]);
        }
  }
	u32 *repaired_cw = bch_decoder(encoded, new_size, &out_length);
	if (repaired_cw) {
        // 输出提取后的结果
        printf("Extracted high bits:\n");
        for (int i = 0; i < out_length-1; i++) {
            printf("%08X\n", repaired_cw[i]);
        }
  } else {
      printf("Memory allocation failed\n");
  }
	*/
	
	/* BCH编码测试
	const uint32_t TEST_CWS[] = { 0x7A89C197UL, 0x7CD215D8UL };
	
	printf("Testing BCH encoding and error correction:\n");
	
  for (size_t n = 0; n < sizeof(TEST_CWS) / sizeof(TEST_CWS[0]); n++) {
        uint32_t OriginalCW = bch_encode(TEST_CWS[n]);
        uint32_t mask = 0x80000000;

        while (mask > 0) {
            uint32_t damaged_cw = OriginalCW ^ mask;
            uint32_t repaired_cw;
            int result = bch_repair(damaged_cw, &repaired_cw);
            char message[100];
            snprintf(message, sizeof(message), "BCH single-bit error repair at mask 0x%08X", mask);
            if (result == 0) {
                assert_equal(OriginalCW & 0xFFFFFFFE, repaired_cw & 0xFFFFFFFE, message);
            }
            else {
                printf("FAIL: %s Repair failed.\n", message);
            }
            mask >>= 1;
        }
    }
		*/
}


