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

char *text_buf="www.prechin.net";
#define TEXT_LEN sizeof(text_buf)
char *OLED_output_start=NULL;
char *OLED_output_mid=NULL;
char *OLED_output_end=NULL;

//外部内存测试
void ExSRAM_Cap_Test()
{
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

int main()
{	
	u8 i=0;
	u8 key;
	u8 k=0;
	char *text= NULL;
	
	SysTick_Init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组分2 组
	LED_Init();
	USART1_Init(115200);
	KEY_Init();
	OLED_Init();
	My_EXTI_Init();
	
	OLED_ShowString(1,5,"SRAM PUF");
	FSMC_SRAM_Init();
	ExSRAM_Cap_Test(); 
	
	//显示系统正常运行，其他逻辑全用中断处理
	while (1)
	{
		led1=!led1;
		delay_ms(100);
	}
}


