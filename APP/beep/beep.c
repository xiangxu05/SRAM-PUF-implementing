#include "beep.h"

//��ʼ��������
void BEEP_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //ʹ�ܶ˿�Fʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //���ģʽ
	GPIO_InitStructure.GPIO_Pin=BEEP_Pin;//�ܽ�����F8
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//�ٶ�Ϊ100M
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
	GPIO_Init(BEEP_Port,&GPIO_InitStructure); //��ʼ���ṹ��
	
	GPIO_SetBits(BEEP_Port,BEEP_Pin); //�رշ�����
	
}

