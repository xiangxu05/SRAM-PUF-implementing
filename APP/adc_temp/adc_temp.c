#include "adc_temp.h"
#include "SysTick.h"

//��ʼ��ADC_Temp	
void ADC_Temp_Init(void)
{	
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	//RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����
	
	ADC_TempSensorVrefintCmd(ENABLE);//��ADC�ڲ��¶ȴ�����
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����
}


//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_ADC_Temp_Value(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles);	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	for(t=0;t<times;t++)
	{
		ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
		temp_val+=ADC_GetConversionValue(ADC1);
		delay_ms(5);
	}
	return temp_val/times;
} 

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
int Get_Temperture(void)
{
	u32 adc_value;
	int temp;
 	double temperture;
	adc_value=Get_ADC_Temp_Value(ADC_Channel_16,10);	//��ȡͨ��16�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
	temperture=(float)adc_value*(3.3/4096);		//��ѹֵ
	temperture=(temperture-0.76)/0.0025 + 25; //ת��Ϊ�¶�ֵ 
	temp=temperture*100;					//����100��.
	return temp;
}
