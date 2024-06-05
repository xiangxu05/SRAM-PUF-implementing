#include "pwm_dac.h"

void TIM11_CH1_PWM_Init(u16 arr,u16 psc)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  	//TIM11ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); //ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_TIM11); //GPIOF7����λ��ʱ��11 AF7
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //GPIOF7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF,&GPIO_InitStructure); //��ʼ��PF7
 
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);//��ʼ����ʱ��11

	//��ʼ��TIM11 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC1Init(TIM11, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIMX OC2

	TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);  //ʹ��TIMX��CCRX�ϵ�Ԥװ�ؼĴ���
 
	TIM_ARRPreloadConfig(TIM11,ENABLE);//ARPEʹ�� 
	TIM_SetCompare1(TIM11,0);	//��ʼֵ
	TIM_Cmd(TIM11, ENABLE);  //ʹ��TIMX
	
 
} 
