#include "touch_key.h"
#include "SysTick.h"
#include "usart.h"

#define Touch_ARR_MAX_VAL 0xffffffff  //����ARRֵ(TIM2��32λ��ʱ��)	
u16 touch_default_val=0;  //Ϊ���´�������ʱ��ֵ


void TIM2_CH1_Input_Init(u32 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ��TIM2ʱ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);//�ܽŸ���
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;//�ܽ�����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF; //�������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//�ٶ�Ϊ100M
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;//����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ���ṹ��
	
	
	TIM_TimeBaseInitStructure.TIM_Period=arr;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);	
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1; //ͨ��1
	TIM_ICInitStructure.TIM_ICFilter=0x00;  //�˲�
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//������
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1; //��Ƶϵ��
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ֱ��ӳ�䵽TI1
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
		
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��
}

//����������λ �ȷŵ�Ȼ���粢�ͷż�ʱ���ڵ�ֵ
void Touch_Reset(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  //PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���� 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA5
	 
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);//���0,�ŵ�

	delay_ms(5);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־
	TIM_SetCounter(TIM2,0);		//��0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PA5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���� 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//���������� 
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA5
}

//���ز���ߵ�ƽֵ
u16 Touch_Get_Val(void)
{
	Touch_Reset();
	while(TIM_GetFlagStatus(TIM2,TIM_FLAG_CC1)==0) //�ȴ����񵽸ߵ�ƽ��־
	{
		if(TIM_GetCounter(TIM2)>Touch_ARR_MAX_VAL-500)  //��ʱ��ֱ�ӷ���CNTֵ
		{
			return TIM_GetCounter(TIM2);
		}
	}
	return TIM_GetCapture1(TIM2); //���ز���ߵ�ƽֵ
}

//����������ʼ��
u8 Touch_Key_Init(u8 psc)  
{
	u8 i;
	u16 buf[10];
	u8 j;
	u16 temp;
	TIM2_CH1_Input_Init(Touch_ARR_MAX_VAL,psc);
	
	for(i=0;i<10;i++) //��ȡ10��Ϊ����ʱ��Ĵ���ֵ
	{
		buf[i]=Touch_Get_Val();
		delay_ms(10);
	}
	
	for(i=0;i<9;i++)   //��С��������
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[j]=buf[j];
				buf[j]=temp;
			}
		}
	}
	
	temp=0;
	for(i=2;i<8;i++)  //ȡ�м�6����ֵ��� ȡ��ƽ����
	{
		temp+=buf[i];
	}
	touch_default_val=temp/6;
	printf("touch_default_val=%d \r\n",touch_default_val);
	if(touch_default_val>Touch_ARR_MAX_VAL/2)
	{
		return 1;//��ʼ����������Touch_ARR_MAX_VAL/2����ֵ,������!
	}
	return 0;
}

//��ȡn��,ȡ���ֵ
//n��������ȡ�Ĵ���
//����ֵ��n�ζ������������������ֵ
u16 Touch_Get_MaxVal(u8 n)
{
	u16 temp=0;
	u16 res=0; 
	while(n--)
	{
		temp=Touch_Get_Val();//�õ�һ��ֵ
		if(temp>res)res=temp;
	}
	return res;
}  

//ɨ�败������
//mode:0,��֧����������(����һ�α����ɿ����ܰ���һ��);1,֧����������(����һֱ����)
//����ֵ:0,û�а���;1,�а���;										  
#define TOUCH_GATE_VAL 	100	//����������ֵ,Ҳ���Ǳ������tpad_default_val+TOUCH_GATE_VAL,����Ϊ����Ч����.
u8 Touch_Key_Scan(u8 mode)
{
	static u8 keyen=0;	//0,���Կ�ʼ���;>0,�����ܿ�ʼ���	 
	u8 res=0;
	u8 sample=3;		//Ĭ�ϲ�������Ϊ3��	 
	u16 rval;
	if(mode)
	{
		sample=6;	//֧��������ʱ�����ò�������Ϊ6��
		keyen=0;	//֧������	  
	}
	rval=Touch_Get_MaxVal(sample); 
	if(rval>(touch_default_val+TOUCH_GATE_VAL)&&rval<(10*touch_default_val))//����touch_default_val+TPAD_GATE_VAL,��С��10��touch_default_val,����Ч
	{							 
		if((keyen==0)&&(rval>(touch_default_val+TOUCH_GATE_VAL)))	//����touch_default_val+TOUCH_GATE_VAL,��Ч
		{
			res=1;
		}	   
		printf("�����󲶻�ߵ�ƽֵΪ��%d\r\n",rval);		     	    					   
		keyen=3;				//����Ҫ�ٹ�3��֮����ܰ�����Ч   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	 
