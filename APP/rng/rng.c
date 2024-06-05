#include "rng.h"
#include "SysTick.h"

//��ʼ��RNG
//����ֵ��1������0���ɹ�
u8 RNG_Init(void)
{
	u16 i;
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG,ENABLE);//ʹ��RNGʱ�� AHB2
	RNG_Cmd(ENABLE);//ʹ��RNG
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==0)  //�ȴ�DRDY�ȶ�
	{
		i++;
		delay_us(100);
		if(i>=10000)
		{
			return 1; //�����������������
		}
	}
	return 0;
}

//����32λ�����
u32 RNG_Get_RandomNum(void)
{
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==0);
	return RNG_GetRandomNumber();
}

//����[min,max]��Χ�������
int RNG_Get_RandomRange(int min,int max)
{
	return min+RNG_Get_RandomNum()%(max-min+1);
}
