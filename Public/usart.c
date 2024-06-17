#include "usart.h"	

extern int status;
extern u8 buffer[];
extern u16 bufferIndex;
extern u8 inputFlag;
int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
{
	USART_SendData(USART1,(u8)ch);	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	return ch;
}


//��ʼ��IO ����1 
//bound:������
void USART1_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10 ; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
	

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����	
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
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {  // �����ж�
        r = USART_ReceiveData(USART1);  // ��ȡ���յ�������
        if (bufferIndex < BUFFER_SIZE) {
            // ��������� '\r' �ַ�
            if (r == '\r') {
                return;
            }
            buffer[bufferIndex++] = r;
            
            // ����Ƿ��յ�һ������������
            if (r == '\n' || bufferIndex==BUFFER_SIZE) {  // ���������� '\n' ����
                buffer[bufferIndex - 1] = '\0';  // �� '\n' �滻Ϊ�ַ���������

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
                        FSMC_SRAM_PUF_Output(data);  // ���ȶ����
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
    										// ����ǰ512λ�Ĺ�ϣֵ
												sha512_init(&ctx);
    										sha512_update(&ctx, buffer, calculate_length(buffer));
    										sha512_final(&ctx, hash);
												
                        FSMC_SRAM_PUF_Output(data);  // ���ȶ����
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

                // ��ջ�����
                for (int i = 0; i < bufferIndex; i++) {
                    buffer[i] = 0;
                }
                bufferIndex = 0;
            }
        }
    }
}



 



