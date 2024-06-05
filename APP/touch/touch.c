#include "touch.h"
#include "24cxx.h"
#include "SysTick.h"
#include "tftlcd.h"
#include "ft6236.h"
#include "gt5663.h"


#define TOUCH_AdjDelay500ms() delay_ms(500)

TouchTypeDef TouchData;         //���������洢��ȡ��������
static PosTypeDef TouchAdj;     //����һ��������������У������



void TOUCH_Init(void)
{
#if	defined(TFTLCD_HX8357D)||defined(TFTLCD_HX8352C)||defined(TFTLCD_ST7793)|| \
	defined(TFTLCD_ILI9327)||defined(TFTLCD_ILI9486)
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOB,C,Fʱ��

	//GPIOB1,2��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;//PB1/PB2 ����Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PB0����Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//PC13����Ϊ�������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PF11�����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���ģʽ
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��
	
	AT24CXX_Init();		//��ʼ��24CXX
	AT24CXX_Read(TOUCH_ADJ_ADDR,&TouchAdj.posState,sizeof(TouchAdj));
	if(TouchAdj.posState != TOUCH_ADJ_OK)
    {
        TOUCH_Adjust(); //У��   
    }
#endif
	
#ifdef TFTLCD_ILI9488
	FT5206_Init();	
#endif
		
#ifdef TFTLCD_NT35510
	GT5663_Init();
#endif
	
}


//SPIд����
//������ICд��1byte����    
//num:Ҫд�������
void TOUCH_Write_Byte(u8 num)    
{  
	u8 count=0;   
	for(count=0;count<8;count++)  
	{ 	  
		if(num&0x80)TDIN=1;  
		else TDIN=0;   
		num<<=1;    
		TCLK=0; 
		delay_us(1);
		TCLK=1;		//��������Ч	        
	}		 			    
}

//SPI������ 
//�Ӵ�����IC��ȡadcֵ
//CMD:ָ��
//����ֵ:����������	   
u16 TOUCH_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;		//������ʱ�� 	 
	TDIN=0; 	//����������
	TCS=0; 		//ѡ�д�����IC
	TOUCH_Write_Byte(CMD);//����������
	delay_us(6);//ADS7846��ת��ʱ���Ϊ6us
	TCLK=0; 	     	    
	delay_us(1);    	   
	TCLK=1;		//��1��ʱ�ӣ����BUSY
	delay_us(1);    
	TCLK=0; 	     	    
	for(count=0;count<16;count++)//����16λ����,ֻ�и�12λ��Ч 
	{ 				  
		Num<<=1; 	 
		TCLK=0;	//�½�����Ч  	    	   
		delay_us(1);    
 		TCLK=1;
 		if(DOUT)Num++; 		 
	}  	
	Num>>=4;   	//ֻ�и�12λ��Ч.
	TCS=1;		//�ͷ�Ƭѡ	 
	return(Num);   
}

uint16_t TOUCH_ReadData(uint8_t cmd)
{
    uint8_t i, j;
    uint16_t readValue[TOUCH_READ_TIMES], value;
    uint32_t totalValue;

    /* ��ȡTOUCH_READ_TIMES�δ���ֵ */
    for(i=0; i<TOUCH_READ_TIMES; i++)
    {   /* ��Ƭѡ */
        TCS=0;
        /* �ڲ��ģʽ�£�XPT2046ת����Ҫ24��ʱ�ӣ�8��ʱ���������֮��1��ʱ��ȥ�� */
        /* æ�źţ��������12λת�������ʣ��3��ʱ���Ǻ���λ */    
        readValue[i]=TOUCH_Read_AD(cmd); // �������ѡ��X�����Y�� 
        
        TCS=1;
    }

    /* �˲����� */
    /* ���ȴӴ�С���� */
    for(i=0; i<(TOUCH_READ_TIMES - 1); i++)
    {
        for(j=i+1; j<TOUCH_READ_TIMES; j++)
        {
            /* ����ֵ�Ӵ�С�������� */
            if(readValue[i] < readValue[j])
            {
                value = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = value;
            }   
        }       
    }
   
    /* ȥ�����ֵ��ȥ����Сֵ����ƽ��ֵ */
    j = TOUCH_READ_TIMES - 1;
    totalValue = 0;
    for(i=1; i<j; i++)     //��y��ȫ��ֵ
    {
        totalValue += readValue[i];
    }
    value = totalValue / (TOUCH_READ_TIMES - 2);
      
    return value;
}

uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{   
    uint16_t xValue1, yValue1, xValue2, yValue2;

    xValue1 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue1 = TOUCH_ReadData(TOUCH_Y_CMD);
    xValue2 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue2 = TOUCH_ReadData(TOUCH_Y_CMD);
    
    /* �鿴������֮���ֻ����ֵ��� */
    if(xValue1 > xValue2)
    {
        *xValue = xValue1 - xValue2;
    }
    else
    {
        *xValue = xValue2 - xValue1;
    }

    if(yValue1 > yValue2)
    {
        *yValue = yValue1 - yValue2;
    }
    else
    {
        *yValue = yValue2 - yValue1;
    }

    /* �жϲ�����ֵ�Ƿ��ڿɿط�Χ�� */
	if((*xValue > TOUCH_MAX+0) || (*yValue > TOUCH_MAX+0))  
	{
		return 0xFF;
	}

    /* ��ƽ��ֵ */
    *xValue = (xValue1 + xValue2) / 2;
    *yValue = (yValue1 + yValue2) / 2;

    /* �жϵõ���ֵ���Ƿ���ȡֵ��Χ֮�� */
    if((*xValue > TOUCH_X_MAX+0) || (*xValue < TOUCH_X_MIN) 
       || (*yValue > TOUCH_Y_MAX+0) || (*yValue < TOUCH_Y_MIN))
    {                   
        return 0xFF;
    }
 
    return 0; 
}

uint8_t TOUCH_ReadAdjust(uint16_t x, uint16_t y, uint16_t *xValue, uint16_t *yValue)
{
    uint8_t i;
    uint32_t timeCont;

    /* ��ȡУ��������� */
    LCD_Clear(BACK_COLOR);
    LCD_DrowSign(x, y, RED);
    i = 0;
    while(1)
    {
        if(!TOUCH_ReadXY(xValue, yValue))
        {
            i++;
            if(i > 10)         //��ʱһ�£��Զ�ȡ���ֵ
            {
                LCD_DrowSign(x, y, BACK_COLOR);
                return 0;
            }
               
        }
		timeCont++;
        /* ��ʱ�˳� */
        if(timeCont > 0xFFFFFFFE)
        {   
                LCD_DrowSign(x, y, BACK_COLOR); 
                return 0xFF;
        } 
    }       
}

void TOUCH_Adjust(void)
{
#if	defined(TFTLCD_HX8357D)||defined(TFTLCD_HX8352C)||defined(TFTLCD_ST7793)|| \
	defined(TFTLCD_ILI9327)||defined(TFTLCD_ILI9486)
	
	uint16_t px[2], py[2], xPot[4], yPot[4];
    float xFactor, yFactor;

    /* ��ȡ��һ���� */
    if(TOUCH_ReadAdjust(LCD_ADJX_MIN, LCD_ADJY_MIN, &xPot[0], &yPot[0]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* ��ȡ�ڶ����� */
    if(TOUCH_ReadAdjust(LCD_ADJX_MIN, LCD_ADJY_MAX, &xPot[1], &yPot[1]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* ��ȡ�������� */
    if(TOUCH_ReadAdjust(LCD_ADJX_MAX, LCD_ADJY_MIN, &xPot[2], &yPot[2]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* ��ȡ���ĸ��� */
    if(TOUCH_ReadAdjust(LCD_ADJX_MAX, LCD_ADJY_MAX, &xPot[3], &yPot[3]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();
    
    /* �����ȡ�����ĸ�������ݣ����ϳɶԽǵ������� */
    px[0] = (xPot[0] + xPot[1]) / 2;
    py[0] = (yPot[0] + yPot[2]) / 2;
    px[1] = (xPot[3] + xPot[2]) / 2;
    py[1] = (yPot[3] + yPot[1]) / 2;

    /* ����������� */
    xFactor = (float)LCD_ADJ_X / (px[1] - px[0]);
    yFactor = (float)LCD_ADJ_Y / (py[1] - py[0]);  
    
    /* ���ƫ���� */
    TouchAdj.xOffset = (int16_t)LCD_ADJX_MAX - ((float)px[1] * xFactor);
    TouchAdj.yOffset = (int16_t)LCD_ADJY_MAX - ((float)py[1] * yFactor);

    /* �����������������ݴ���Ȼ�󱣴� */
    TouchAdj.xFactor = xFactor ;
    TouchAdj.yFactor = yFactor ;
    
    TouchAdj.posState = TOUCH_ADJ_OK;
    AT24CXX_Write(TOUCH_ADJ_ADDR, &TouchAdj.posState, sizeof(TouchAdj)); 
#endif

#ifdef TFTLCD_ILI9488 
	return; //���ݴ���������ҪУ׼
#endif

#ifdef TFTLCD_NT35510
	return; //���ݴ���������ҪУ׼
#endif

}

//����ֵ:��ǰ����״̬.
//1,�����޴���;0,�����д���
uint8_t TOUCH_Scan(void)
{

#if	defined(TFTLCD_HX8357D)||defined(TFTLCD_HX8352C)||defined(TFTLCD_ST7793)|| \
	defined(TFTLCD_ILI9327)||defined(TFTLCD_ILI9486)
//    if(PEN == 0)   //�鿴�Ƿ��д���
    {
        if(TOUCH_ReadXY(&TouchData.x, &TouchData.y)) //û�д���
        {
            return 0xff;    
        }
        /* ������������ֵ���������������ֵ */
        TouchData.lcdx[0] = TouchData.x * TouchAdj.xFactor + TouchAdj.xOffset;
        TouchData.lcdy[0] = TouchData.y * TouchAdj.yFactor + TouchAdj.yOffset;
        
        /* �鿴��������ֵ�Ƿ񳬹�������С */
        if(TouchData.lcdx[0] > tftlcd_data.width)
        {
            TouchData.lcdx[0] = tftlcd_data.width;
        }
        if(TouchData.lcdy[0] > tftlcd_data.height)
        {
            TouchData.lcdy[0] = tftlcd_data.height;
        }
        return 0; 
    }
 //   return 0xFF;
#endif
	
#ifdef TFTLCD_ILI9488
	return FT5206_Scan(0);	
#endif

#ifdef TFTLCD_NT35510
	return GT5663_Scan(0);
#endif
	
}
