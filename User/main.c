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
//#include "encoder.h"
//#include "GF.h"

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
		FSMC_SRAM_ReadBuffer(&readData,addr,1);
		writeData = readData;
		//FSMC_SRAM_WriteBuffer(&writeData, addr, 1);
		//FSMC_SRAM_ReadBuffer(&readData,addr,1);
		
		/* 查看读取到的数据是否跟写入数据一样 */
        if(readData == writeData)
        {	
			printf("%c", readData);
            cap++;
            addr += 1024;
            readData = 0;
			if(cap%128 == 0){
				sprintf(output, "ExSRAMCap:%dKB", cap);
				OLED_ShowString(3,1,output);//显示内存容量 
			}
            if(addr > 1024 * 1024) //SRAM容量最大为1MB
            {	
				OLED_ShowString(4,7,"ok!");
                break;
            }    
        }
        else
        {
            break;
        }     
	}
}

void System_Init(){
	SysTick_Init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组分2 组
	LED_Init();
	USART1_Init(921600);
	KEY_Init();
	OLED_Init();
	My_EXTI_Init();
	
	OLED_ShowString(1,5,"SRAM PUF");
	FSMC_SRAM_Init();
	ExSRAM_Cap_Test(); 
}

void assert_equal(uint32_t expected, uint32_t actual, const char *message) {
    if (expected != actual) {
        printf("FAIL: %s Expected: 0x%08X, Got: 0x%08X\n", message, expected, actual);
    }
    else {
        printf("PASS: %s\n", message);
    }
}

int main()
{	
	//u32 test = 0x12230c12;
	System_Init();
	
	//printf("0x%08x",bch_encode(test));
	
	/* BCH编码测试
	const uint32_t TEST_CWS[] = { 0x7A89C197UL, 0x7CD215D8UL };
	
	printf("Testing BCH encoding and error correction:\n");
	
	for (size_t n = 0; n < sizeof(TEST_CWS) / sizeof(TEST_CWS[0]); n++) {
        uint32_t encoded = bch_encode(TEST_CWS[n]);
        char message[100];
        snprintf(message, sizeof(message), "BCH encoding sanity check for index %zu", n);
        assert_equal(TEST_CWS[n], encoded, message);
    }
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
	
	while (1)
	{
		led1=!led1;
		delay_ms(100);
	}
}


