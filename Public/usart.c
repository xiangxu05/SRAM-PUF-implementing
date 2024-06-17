#include "usart.h"	

extern int status;
extern u8 buffer[];
extern u16 bufferIndex;
extern u8 inputFlag;
int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}


//初始化IO 串口1 
//bound:波特率
void USART1_Init(u32 bound)
{
   //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10 ; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、	
}

int string_compare(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0');
}

u32 calculate_length(u8 *input) {
    u32 length = 0;
    while (input[length] != '\0') {
        length++;
    }
    return length;
}

void USART1_IRQHandler(void) {
    uint8_t r;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {  // 接收中断
        r = USART_ReceiveData(USART1);  // 读取接收到的数据
        if (bufferIndex < BUFFER_SIZE) {
            // 跳过多余的 '\r' 字符
            if (r == '\r') {
                return;
            }
            buffer[bufferIndex++] = r;
            
            // 检查是否收到一个完整的命令
            if (r == '\n' || bufferIndex==BUFFER_SIZE) {  // 假设命令以 '\n' 结束
                buffer[bufferIndex - 1] = '\0';  // 将 '\n' 替换为字符串结束符

                if (string_compare((char*)buffer, "AT_default")) {
										printf("Change statu to default\n");
										for (int i = 0; i < bufferIndex; i++) {
												buffer[i] = 0;
										}
										bufferIndex = 0;
										status = 0;
										return;
                } else if (string_compare((char*)buffer, "AT_init")) {
                    status = 1;
                } else if (string_compare((char*)buffer, "AT_source")) {
                    status = 2;
                } else if (string_compare((char*)buffer, "AT_strongSram")) {
										printf("Change statu to strongSram\n");
                    status = 3;
										for (int i = 0; i < bufferIndex; i++) {
												buffer[i] = 0;
										}
										bufferIndex = 0;
										return;
                } else if (string_compare((char*)buffer, "AT_sourceSram")) {
										status = 4;
								}

                switch (status) {
                    case 0:
                        for (int i = 0; i < bufferIndex - 1; i++) {
                            printf("%c", (char)buffer[i]);
                        }
                        break;
                    case 1:
                        FSMC_SRAM_PUF_Init();
                        status = 3;
                        break;
                    case 2:
                        u32 data[32];
                        FSMC_SRAM_PUF_Output(data);  // 读稳定输出
												printf("Souce Entropy:\n");
                        for (int i = 0; i < 32; i++) {
                            printf("%08x", data[i]);
                        }
												status = 3;
                        break;
                    case 3:
												SHA512_CTX ctx;
												u8 hash[SHA512_BLOCK_SIZE];
												u32 hash1[16];
    										// 计算前512位的哈希值
												sha512_init(&ctx);
    										sha512_update(&ctx, buffer, calculate_length(buffer));
    										sha512_final(&ctx, hash);
												
                        FSMC_SRAM_PUF_Output(data);  // 读稳定输出
												printf("Response:\n");
												for (int i = 0; i < 16; i++) {
														hash1[i] = ((u32)hash[4*i] << 24) | ((u32)hash[4*i+1] << 16) |
																	((u32)hash[4*i+2] << 8) | (u32)hash[4*i+3];
												}
                        for (int i = 0; i < 32; i++) {
                           printf("%08x", data[i]^hash1[i%16]);
                        }
                        break;
										case 4:
												u32 sramData[32];
												FSMC_SRAM_PUF_Read_With_Size(sramData);
												for (int i = 0; i < 32; i++) {
                           printf("%08x", sramData[i]);
                        }
												status = 3;
												break;
                    default:
                        break;
                }

                // 清空缓冲区
                for (int i = 0; i < bufferIndex; i++) {
                    buffer[i] = 0;
                }
                bufferIndex = 0;
            }
        }
    }
}



 



