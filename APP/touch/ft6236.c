#include "ft6236.h"
#include "touch.h"
#include "ctiic.h"
#include "usart.h"
#include "SysTick.h" 
#include "string.h" 
#include "led.h" 
#include "touch.h" 


u16 ctadjustx=0;
u16 ctadjusty=0;


//��FT5206д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i;
	u8 ret=0;
	CT_IIC_Start();	 
	CT_IIC_Send_Byte(FT_CMD_WR);	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
	for(i=0;i<len;i++)
	{	   
    	CT_IIC_Send_Byte(buf[i]);  	//������
		ret=CT_IIC_Wait_Ack();
		if(ret)break;  
	}
    CT_IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}
//��FT5206����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���			  
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
{
	u8 i; 
 	CT_IIC_Start();	
 	CT_IIC_Send_Byte(FT_CMD_WR);   	//����д���� 	 
	CT_IIC_Wait_Ack(); 	 										  		   
 	CT_IIC_Send_Byte(reg&0XFF);   	//���͵�8λ��ַ
	CT_IIC_Wait_Ack();  
 	CT_IIC_Start();  	 	   
	CT_IIC_Send_Byte(FT_CMD_RD);   	//���Ͷ�����		   
	CT_IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1); //������	  
	} 
    CT_IIC_Stop();//����һ��ֹͣ����     
} 
//��ʼ��FT5206������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ�� 
u8 FT5206_Init(void)
{
	u8 temp[2]; 
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOB,Cʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;//PB1����Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PC13����Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��	
	
	CT_IIC_Init();      	//��ʼ����������I2C����  
	FT_RST=0;				//��λ
	delay_ms(20);
 	FT_RST=1;				//�ͷŸ�λ		    
	delay_ms(50);  	
	temp[0]=0;
	FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);	//������������ģʽ 
	FT5206_WR_Reg(FT_ID_G_MODE,temp,1);		//��ѯģʽ 
	temp[0]=22;								//������Чֵ��22��ԽСԽ����	
	FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);	//���ô�����Чֵ
	temp[0]=12;								//�������ڣ�����С��12�����14
	FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	//��ȡ�汾�ţ��ο�ֵ��0x3003
	FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  
	if(temp[0]==0X30&&temp[1]==0X03)//�汾:0X3003
	{
		printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
		return 0;
	} 
	return 1;
}
const u16 FT5206_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};//
//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//1,�����޴���;0,�����д���
u8 FT5206_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=1;
	u8 temp;
	u8 tp_sta;
	
	FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬ 
	if((mode&0XF)&&((mode&0XF)<6))
	{
		temp=0XFF<<(mode&0XF);//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
		TouchData.tpsta=(~temp); 
		for(i=0;i<2;i++)
		{
			if(TouchData.tpsta&(1<<i))	//������Ч?
			{
				FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4);	//��ȡXY����ֵ 
				
				if(tftlcd_data.dir==1)//����
				{
					TouchData.lcdy[i]=320-(((u16)(buf[0]&0X0F)<<8)+buf[1]);
					TouchData.lcdx[i]=480-(((u16)(buf[2]&0X0F)<<8)+buf[3]);
				}
				else
				{
					TouchData.lcdy[i]=480-(((u16)(buf[2]&0X0F)<<8)+buf[3]);
					TouchData.lcdx[i]=320-(((u16)(buf[0]&0X0F)<<8)+buf[1]);
				}  
				//if((buf[0]&0XF0)!=0X80)TouchData.lcdx[0]=TouchData.lcdy[0]=0;//������contact�¼�������Ϊ��Ч
				//printf("x[%d]:%d,y[%d]:%d\r\n",i,TouchData.lcdx[i],i,TouchData.lcdy[i]);
			}			
		} 
		res=0;
		if(TouchData.lcdx[0]==0 && TouchData.lcdy[0]==0)mode=0;	//���������ݶ���0,����Դ˴�����
		
	}

	if((mode&0X1F)==0)//�޴����㰴��
	{ 
		TouchData.tpsta=0;
		TouchData.lcdx[0]=0xffff;
		TouchData.lcdy[0]=0xffff;
	} 	
	return res;
}

