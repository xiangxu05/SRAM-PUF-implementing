#include "wkup.h"

void Enter_Standby_Mode(void)
{
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//ʹ��PWR����ʱ��
	
	//PWR_BackupAccessCmd(ENABLE);//���������ʹ��
	//�������ʹ��RTC�жϹ��ܣ�������Ҫ�ر����RTC�ж�
	//���ȵ�����ʹ��RTC���ܣ������Ҫ�������Ѿ���Ҫ������������ע�͵���䣩
//	RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//�ر�RTC����жϣ�������RTCʵ����ˡ�
//	RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//���RTC����жϱ�־λ��
	
	PWR_ClearFlag(PWR_FLAG_WU);//���Wake-up ��־
	
	PWR_WakeUpPinCmd(ENABLE);//ʹ�ܻ��ѹܽ�	ʹ�ܻ���ʧ�ܻ��ѹܽŹ���
	
	
	PWR_EnterSTANDBYMode();//�������ģʽ
}



