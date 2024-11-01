#ifndef _DrvUsart1_h_
#define _DrvUsart1_h_


#define USART3_BUFF_LANGTH     1048




typedef struct {
	unsigned int len;
	unsigned short ind;	
	unsigned char  buf[USART3_BUFF_LANGTH];
}SerialBuffType;		//·¢ËÍÔÝ´æÇø

#define SerialBuffDefault() {\
	{0,},\
	0,\
	0,\
}


void BspUsart1Init(void);
void BspUsart1Close(void);

unsigned short BspUsart1Send(unsigned char *buf, unsigned short len);
unsigned short BspUsart1Receive(unsigned char *buf);

unsigned char Usart1ReceiveByte(void);
void BspUsart1IRQCallBack(void *fun);
void USART1_IRQHandler_cbk(void);
     

void BspUsart1Init(void);
unsigned short BspUsart1Send(unsigned char *buf, unsigned short len);

#endif
/********************** END ***************************************************/


