#include "key.h"
#include "SysTick.h"


//������ʼ������
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA,ENABLE); //ʹ�ܶ˿�ʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; //����ģʽ
	GPIO_InitStructure.GPIO_Pin=KEY_LEFT_Pin|KEY_DOWN_Pin|KEY_RIGHT_Pin;//�ܽ�����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
	GPIO_Init(KEY_Port,&GPIO_InitStructure); //��ʼ���ṹ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN; //����ģʽ
	GPIO_InitStructure.GPIO_Pin=KEY_UP_Pin;//�ܽ�����
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;//����
	GPIO_Init(KEY_UP_Port,&GPIO_InitStructure); //��ʼ���ṹ��
}

//mode=0:���ΰ��°���
//mode=1���������°���
//����ֵ��0��ʾδ�а�������
u8 KEY_Scan(u8 mode)
{
	static u8 key=1;
	if(key==1&&(K_UP==1||K_DOWN==0||K_LEFT==0||K_RIGHT==0)) //����һ����������
	{
		delay_ms(10);  //����
		key=0;
		if(K_UP==1)
		{
			return KEY_UP; 
		}
		else if(K_DOWN==0)
		{
			return KEY_DOWN; 
		}
		else if(K_LEFT==0)
		{
			return KEY_LEFT; 
		}
		else
		{
			return KEY_RIGHT;
		}
	}
	else if(K_UP==0&&K_DOWN==1&&K_LEFT==1&&K_RIGHT==1)    //�ް�������
	{
		key=1;
	}
	if(mode==1) //������������
	{
		key=1;
	}
	return 0;
}

