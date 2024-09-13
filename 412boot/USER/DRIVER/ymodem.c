/*
********************************************************************************
*                           
*                              YModem 协议部分
*
* File          : YModem.C
* Version       : V1.0
* By            : whq
*
* For           : Stm32f10x
* Mode          : Thumb2
* Toolchain     : 
*                   RealView Microcontroller Development Kit (MDK)
*                   Keil uVision
* Description   : 
*
*
* Date          : 2013.2.20
*******************************************************************************/

#include <string.h>
#include "YModem.h"
#include "common.h"
#include "stdio.h"
#include "BspUsart1.h"
#include "BspTime2.h"
#include "BspTime3.h"
#define SendString(a, b)    BspUsart1Send(a, b)    //串口发送函数 a:字符串 b:长度


/*******************************************************************************
* Function Name :unsigned short YModemCrc(char c, unsigned short crc)
* Description   :Ymodem crc单字节效验
* Input         :
* Output        :
* Other         :
* Date          :2013.02.20
*******************************************************************************/
static unsigned short YModemCrc(char *pData, unsigned short sLen)
{  
   unsigned short int crc = 0;    
   unsigned short int i = 0;
   
   while (sLen--)  //len是所要计算的长度
   {
       crc = crc^(int)(*pData++) << 8; //    
       for (i=8; i!=0; i--) 
       {
           if (crc & 0x8000)   
               crc = crc << 1 ^ 0x1021;    
           else
               crc = crc << 1;
       }    
   }
   return crc;
}

/*******************************************************************************
* Function Name :static char ReceivePacket (char *pRece, unsigned short sReceLen, char *pData, unsigned short *sLen, char cNum)
* Description   :解析数据包
* Input         :pData:数据包， sLen:总长度，cNum:当前编号
* Output        :
* Other         :
* Date          :2013.02.20
*******************************************************************************/
static int ReceivePacket (char *pRece, int sReceLen, char *pData, int *sLen, char cNum)
{
    int len = 0;            //数据的长度
    int tmp = 0;

    if (pRece[0] == SOH)
    {
        len = PACKET_SIZE;
    } 
    else if (pRece[0] == STX)
    {
        len = PACKET_1K_SIZE;
    }
    else if (pRece[0] == EOT)   
    {
        return 0;           //接收到结束标志
    }
    else if (pRece[0] == ETX)   
    {
        return 0;           //接收到中止标志 ctrl + c
    }
    else if (pRece[0] == CA)    //接收到中止标志
    {
        if (pRece[1] == CA)     //确认中止标志
            return 0; 
        else
            return -6;          //接收到中止错误标志
    }
    else
    {
        return -1;              //数据头错误
    }

    if (pRece[1] != cNum)
        return -2;              //数据编号错误
     
    if (pRece[2] != (char)(~cNum))
        return -3;              //数据编号补码错误

    if (sReceLen != (len + PACKET_OVERHEAD))
        return -4;              //数据总长度错误

    // YModem只效验数据部分
    tmp = YModemCrc(pRece + PACKET_HEADER, len);
    if (((pRece[sReceLen - 2] << 8) |pRece[sReceLen - 1]) != tmp)
        return -5;              //crc效验错误

    memcpy(pData, &pRece[PACKET_HEADER], len);  //保存数据
    *sLen = len;

    return 1;
}


/*******************************************************************************
* Function Name :int YmodemSendChar(unsigned char ch, eYM_STAT *stat, int *errCount)
* Description   :发送一个字符 并判断 错误计数次数
* Input         :
* Output        :
* Other         :
* Date          :2013.02.24
*******************************************************************************/
void YmodemSendChar(unsigned char ch, eYM_STAT *stat, int *errCount)
{
    while(1 != SendString(&ch, 1));
 
    if (ch == ACK)          //错误计数器清零
    {
        *errCount = 0;
    }
    else if (ch == NAK)     //错误计数器累加
    {
        (*errCount)++;
//        if ((*errCount) > MAX_ERRORS)  //超过最大错误次数
//        {
//            *stat = eYM_END;
//        }
    }
}

/*******************************************************************************
* Function Name :char YmodemReceive(char *buff, unsigned short *len)
* Description   :YModem 处理程序
* Input         :pRece:接收到的数据帧   pData:帧中的数据部分
* Output        :res < 0: 接收结束 res>0 :接收到有效数据
* Other         :
* Date          :2013.02.19
*******************************************************************************/
int YmodemReceive(char *pRece, int *sReceLen, char *pData, int *sResLen)
{
    int cRes = YM_VOIDER;
    static eYM_STAT stat= eYM_INIT;          //当前状态
    
    static char affirmCount = 0;                //确认次数    
    static int iNumber = 0;                     //数据包编号
    static int sErrorCount = 0;                 //错误包次数

    char cTmp = 0;
	
	
    
    switch (stat)
    {
    case eYM_INIT:
        iNumber = 0;
        sErrorCount = 0;   
        affirmCount = 0;
        stat = eYM_RECE_HEAD_PACKET;
        IS_TIMEOUT_1MS(eTimYModem, 0);          //清超时计数器
        break;

    case eYM_RECE_HEAD_PACKET:
        if (IS_TIMEOUT_1MS(eTimYModem, NAK_TIMEOUT))            //若等待超时 
        {         
            YmodemSendChar(CRC16, &stat, &sErrorCount);         //发送 'C'
            stat = eYM_INIT;
            break;
        }
            
        if (*sReceLen == 0)
            break;
  
        cTmp = ReceivePacket(pRece, *sReceLen, pData, sResLen, iNumber & 0xFF);
        *sReceLen = 0;                  //读完数据后，允许接收新数据
        switch (cTmp)
        {
        case 1:                      //接收正确
            if (pData[PACKET_HEADER] == 0)                      //无文件发送
                stat = eYM_END;
            else
            {
                stat = eYM_RECE_DATA_START;                     //有文件发送
                cRes = YM_FILE_INFO;    //返回接收头文件
                iNumber++;
                affirmCount = 0;           
            }
            break;

        case 0:                         //接收到结束标志
            if (affirmCount)
            {
                iNumber = 0;  
                YmodemSendChar(ACK, &stat, &sErrorCount);       //正确应答
            }
            else
            {       
                  
              
              
            }
            affirmCount++;
            break;

        default:                        //接收数据有误  
            //printf("YmodemReceive ==> sReceLen : %d !!!\r\n",*sReceLen);
            affirmCount = 0;
            YmodemSendChar(NAK, &stat, &sErrorCount);           //错误应答
            break;
        }
        IS_TIMEOUT_1MS(eTimYModem, 0);                          //清超时计数器
        break;
        
    case eYM_RECE_DATA_START:
        stat = eYM_RECE_DATA;
        YmodemSendChar(ACK, &stat, &sErrorCount);               //正确应答
        YmodemSendChar(CRC16, &stat, &sErrorCount);             //发送 'C' 
        break;
                
    case eYM_RECE_DATA:
        if (IS_TIMEOUT_1MS(eTimYModem, NAK_TIMEOUT))            //若等待超时 
        {
            stat = eYM_END;
            break;
        }
            
        if (*sReceLen == 0)
            break;
  
        cTmp = ReceivePacket(pRece, *sReceLen, pData, sResLen, iNumber & 0xFF);
        *sReceLen = 0;                  //读完数据后，允许接收新数据
        switch (cTmp)
        {
        case 1:                      //接收正确
            cRes = YM_FILE_DATA;        //返回接收数据正确
            iNumber++;                  //
            affirmCount = 0;
            YmodemSendChar(ACK, &stat, &sErrorCount);        //正确应答
            break;

        case 0:                         //接收到结束标志
            if (affirmCount)
            {
                iNumber = 0;
                stat = eYM_RECE_HEAD_PACKET;
                YmodemSendChar(ACK, &stat, &sErrorCount);    //正确应答
                YmodemSendChar(CRC16, &stat, &sErrorCount);    //正确应答
            }
            else
            {
                YmodemSendChar(NAK, &stat, &sErrorCount);    //错误应答
            }
            affirmCount++;
            break;

        default:                        //接收数据有误  
            affirmCount = 0;
            YmodemSendChar(NAK, &stat, &sErrorCount);       //错误应答
            break;
        } 
        IS_TIMEOUT_1MS(eTimYModem, 0);                      //清超时计数器        
        break;
        
    case eYM_END:
        YmodemSendChar(ACK, &stat, &sErrorCount);           //中止
        YmodemSendChar(CA, &stat, &sErrorCount);            //中止
        YmodemSendChar(CA, &stat, &sErrorCount);            //中止
        cRes = YM_EXIT;
        stat = eYM_INIT;
        break;
    }
    
    return cRes;
}


/********************** END ***************************************************/


