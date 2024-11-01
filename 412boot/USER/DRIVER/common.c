/*-------------------------------------------------------------------------

                            接口部分
                            
                            
-------------------------------------------------------------------------*/
#include <string.h>
#include "YModem.h"
#include "common.h"
#include "stdio.h"
#include "main.h"
#include "BspUsart1.h"
#include "BspTime2.h"
#include "BspTime3.h"
#include "stmflash.h"
#include "rtc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static FunVoidType FunReceEnter = NULL;
static FunVoidType FunReceExit = NULL;
static FunWriteType FunWrite = NULL;
static FunProcessType FunCurrentProcess = NULL;

static uint32_t m_ProgramAddr = ApplicationAddress;
static volatile SerialBuffType m_ReceData = SerialBuffDefault();

static volatile eCOM_STATUS m_Mode = eCOMFlashAddr;

static __IO uint32_t m_FlashAddress;		
static __IO uint32_t m_ExtFlashCounter = 0;       //外部FLASH擦除的扇区号

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void FLASH_ProgramStart()
{

}


uint32_t FLASH_WriteBank(uint8_t *pData, uint32_t addr, uint16_t size)
{
  STMFLASH_Write(addr,(uint32_t *)pData,size/4);
  return 0;
}

void FLASH_ProgramDone(void)
{
    
}


/*******************************************************************************
* Function Name :static void Print(uint8_t *str)
* Description   :打印消息  串口发送
* Input         :
* Output        :
* Other         :
* Date          :2013.03.01
*******************************************************************************/
void Print(uint8_t *str)
{
    uint16_t len = 0;

    len = strlen((const char *)str);

    while (BspUsart1Send(str, len) != 1);
}

void BspClose(void)
{

}

/*******************************************************************************
* Function Name :void ReceOneChar(uint8_t ReceCharacter)
* Description   :接收到一个字符
* Input         :
* Output        :
* Other         :
* Date          :2013.02.19
*******************************************************************************/
static void ReceOneChar(uint8_t ReceCharacter)
{
    if (m_ReceData.ind >= USART3_BUFF_LANGTH)
        return;
        
    if (m_ReceData.len > 0)
        return;
        
    m_ReceData.buf[m_ReceData.ind++] = ReceCharacter;
    BspTim3Open();      //定时器重新计数
}

/*******************************************************************************
* Function Name :static void TimEndHandle(void)
* Description   :接收字符超时回调函数
* Input         :
* Output        :
* Other         :
* Date          :2013.02.19
*******************************************************************************/
static void   TimEndHandle(void)
{
    //printf("TimEndHandle ==> m_ReceData.len : %d  m_ReceData.ind : %d !!!\r\n",m_ReceData.len,m_ReceData.ind);
    BspTim3Close();

    m_ReceData.len = m_ReceData.ind;
    m_ReceData.ind = 0;
}



/*******************************************************************************
* Function Name :void JumpToApp(void)
* Description   :跳转到应用程序区
* Input         :None
* Output        :None
* Other         :None
* Date          :2013.02.19
*******************************************************************************/
extern uint8_t Jump_To_App(void);
void JumpToApp(void)
{
    Print("goto App !!!\r\n");

		HAL_Delay(1);

		HAL_RTCEx_BKUPWrite(&hrtc,  RTC_BKP_DR0, 0x12345678);
		NVIC_SystemReset();
	
	
    //Jump_To_App();
    
}


uint32_t enter_ext_flash_update_cnt = 0;	//判断次数
uint8_t enter_ext_flash_update_flag = 0;	//需要从外部flash升级
uint32_t flash_code_lenth = 0;
uint32_t flash_read_addr = STORE_CODE_FLASH_SADDR;
/*******************************************************************************
* Function Name :static void AppChoose(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
* Description   :操作选择   300ms内有 收到 字符C 进入bootload区
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void judge_ext_flash_update(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
{
          *peStat = eCOMChoose;
}




/*******************************************************************************
* Function Name :static void AppChoose(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
* Description   :操作选择   300ms内有 收到 字符C 进入bootload区
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void AppChoose(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
{
    static uint8_t flg = 0;
    
		
    if (*pLen > 0)
    {
        if ((*pData == 'C') || (*pData == 'c'))
        {
            if (flg == 0)
                flg++;

            if (flg && IS_TIMEOUT_1MS(eTim1, 200))  //二次确认
                *peStat = eCOMDisplay;
        }
        *pLen = 0;
    }
    
    if (IS_TIMEOUT_1MS(eTim2, 320))
    {
        
        JumpToApp(); 
//        Print("\r\n运行失败!");
//        printf("\r\nJump to app fail  now wdog!!!\r\n");
        while (1){

        }
     }
}

/*******************************************************************************
* Function Name :static void DisplayMessage(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
* Description   :显示提示消息
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void DisplayMessage(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
{
	char info[2] = {0,0};
	*pLen = 1;
	strcpy((char *)pData, "\r*********************************************************\r\n");
	
	if(enter_ext_flash_update_flag){
		info[0] = enter_ext_flash_update_cnt + 0x30;
		if(enter_ext_flash_update_cnt){
			strcat((char *)pData,info);
			strcat((char *)pData, "秒后从外置flash更新应用区程序，按任意键取消更新\r\n");  
			strcat((char *)pData, "按1或2进入其他选项：\r\n");  
		}		
	}	
	strcat((char *)pData, "1.更新应用区程序；\r\n");  
	strcat((char *)pData, "2.运行APP程序。\r\n");
	strcat((char *)pData, "*********************************************************\r\n");
	strcat((char *)pData, "请选择:\r\n\r\n");
	
	*peStat = eCOMInput;

	Print(pData);
	*pLen = 0;
	
	if(enter_ext_flash_update_flag && enter_ext_flash_update_cnt == 0){
		//倒计时结束，进入升级程序过程
		FLASH_ProgramStart();
		*peStat = eCOMFromFlash;
		m_ProgramAddr = ApplicationAddress; 	
	}	
	
}


/*******************************************************************************
* Function Name :static void InputSelect(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
* Description   :操作选择
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void InputSelect(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
{
	if(enter_ext_flash_update_flag){
		if (IS_TIMEOUT_1MS(eTim2, 1000)){
			if(enter_ext_flash_update_flag != 0xF0){
				//更新取消后不再自动处理，等待串口指令
				enter_ext_flash_update_cnt--;
				*peStat = eCOMDisplay;		//更新一次显示
			}
		}
	}
	if (*pLen > 0){
		switch (*pData){
		case '1': 
			m_ProgramAddr = ApplicationAddress; 
			FunReceEnter = FLASH_ProgramStart;
			FunWrite = FLASH_WriteBank;
			FunReceExit = FLASH_ProgramDone;
			*peStat = eCOMReceive;
			Print("1\r\n请选择要发送文件");
			break;    

		case '2': 
			*peStat = eCOMChoose; 
			Print("2\r\n运行程序...");
			break;    

		default :
			enter_ext_flash_update_flag = 0xF0;	//	取消
			break;
		}
		*pLen = 0;
	}
}



/*******************************************************************************
* Function Name :static void ReceiveData(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
* Description   :YModem协议接收数据，并存储数据
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void ReceiveData(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
{
    uint8_t pArray[1028] = {0,};
    int len = 0;
		
		
    if(*pLen > 100)
    printf("ReceiveData ==> pLen : %d !!!\r\n",*pLen);
    //Uart_SendData(USART3,(char *)(pData),*pLen);
//    *pLen = 0;
   len = 0;
		
		
    switch (
            YmodemReceive((char *)(pData), (int *)pLen, (char *)pArray, (int *)&len))
    {
    case YM_FILE_INFO: 
	printf("ReceiveData ==> YM_FILE_INFO  pLen : %d !!!\r\n",*pLen);
        if (FunReceEnter) (*FunReceEnter)();    //开始函数
        break;
        
    case YM_FILE_DATA: 
	    printf("ReceiveData ==> YM_FILE_DATA  pLen : %d !!!\r\n",*pLen);
        if (FunWrite) (*FunWrite)(pArray, m_ProgramAddr, len);  //接收数据函数
        m_ProgramAddr += len;
        break;
        
    case YM_EXIT: 
	    printf("ReceiveData ==> YM_EXIT  pLen : %d !!!\r\n",*pLen);
        if (FunReceExit) (*FunReceExit)();      //接收完毕函数
        
        FunReceEnter = NULL;
        FunWrite = NULL;
        FunReceExit = NULL;
        *peStat = eCOMChoose;
        Print("\r\n运行程序...");
        break;
    }

}


/*******************************************************************************
* Function Name :static void recover_code_from_ext_flash(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
* Description   :外置flash中读出代码，覆盖当前应用程序
* Input         :
* Output        :
* Other         :
* Date          :2013.02.26
*******************************************************************************/
static void recover_code_from_ext_flash(uint8_t *pData, uint32_t *pLen, volatile eCOM_STATUS *peStat)
{
	
//	uint8_t recover_buf[ONE_READ_EXT_FLASH_LENTH+1] = {0};
//	uint32_t recover_lenth = (flash_code_lenth > ONE_READ_EXT_FLASH_LENTH) ? ONE_READ_EXT_FLASH_LENTH : flash_code_lenth;
//		
//	SpiFlashRead(flash_read_addr,recover_buf,recover_lenth);	//读出代码段
//	FLASH_WriteBank(recover_buf,m_ProgramAddr,recover_lenth);	//覆盖代码段
//	
//	flash_read_addr += recover_lenth;
//	m_ProgramAddr += recover_lenth;
//	flash_code_lenth -= recover_lenth;
//	
//	if(flash_code_lenth == 0){	//全部代码覆盖完毕
//		FLASH_ProgramDone();
//		SpiFlashEraseSector(0);		//擦除升级标识
//		*peStat = eCOMChoose;
//		Print("\r\n运行程序...");
//	}	
}






/*******************************************************************************
* Function Name :void CommonInit(void)
* Description   :接口初始化
* Input         :
* Output        :
* Other         :
* Date          :2013.02.19
*******************************************************************************/
void CommonInit(void)
{
	BspTim3SetIRQCallBack((void*)TimEndHandle);
	BspUsart1IRQCallBack((void*)ReceOneChar);
}


/*******************************************************************************
* Function Name :void CommonExec(void)
* Description   :接口函数
* Input         :
* Output        :
* Other         :
* Date          :2013.02.20
*******************************************************************************/
void CommonExec(void)
{
	if (IS_TIMEOUT_1MS(eWdog, 500)){
		//Print("Now wdog !!!\r\n");
	}


	switch (m_Mode){
		
	case 	eCOMFlashAddr:	//2017.09.15 add
		FunCurrentProcess = judge_ext_flash_update;
		break;
	
	case eCOMChoose:    //判断进入 IAP程序 还是APP程序
		FunCurrentProcess = AppChoose;
		break;

	case eCOMDisplay:   //IAP操作显示
		FunCurrentProcess = DisplayMessage;
		break;

	case eCOMInput:     //IAP操作选择
		FunCurrentProcess = InputSelect;
		break;

	case eCOMReceive:   //YMODEM 接收数据
		FunCurrentProcess = ReceiveData;
		break;

	case eCOMFromFlash:
		FunCurrentProcess = recover_code_from_ext_flash;
		break;
	
	default:
		m_Mode = eCOMChoose;
		break;
	}
	(*FunCurrentProcess)((uint8_t *)(m_ReceData.buf), (uint32_t *)&(m_ReceData.len), &m_Mode);
    
}




/*********************************** END **************************************/

