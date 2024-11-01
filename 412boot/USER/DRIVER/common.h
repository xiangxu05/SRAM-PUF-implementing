/*-------------------------------------------------------------------------

                            接口头文件
                            
-------------------------------------------------------------------------*/



#ifndef _COMMON_H_
#define _COMMON_H_

//FLASH起始地址
//#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 

////FLASH 扇区的起始地址
//#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
//#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
//#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
//#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
//#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
//#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
//#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
//#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
//#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
//#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
//#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
//#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  




/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE         128

#define ApplicationAddress      0x8020000       //APP程序首地址

#define ALL_ERAZE_SECTORS	6			//app程序代码空间 6 sectors

#define ApplicationSize         (ADDR_FLASH_SECTOR_7 - ADDR_FLASH_SECTOR_1)         //程序预留空间	368k 避免擦除原有参数	

#define  STORE_CODE_FLASH_SADDR	4096		//外部flash存储代码的起始地址
#define  STORE_INFO_FLASH_ADDR	0x10	
#define  APP_NEED_UPDATE_FLAG		0x55504150 	//"UPAP"

#define  ONE_READ_EXT_FLASH_LENTH  1024

//#define STM32F10X_HD                            //所选择芯片为大容量芯片
//#define ApplicationSize			((*(vu16 *)(0x1FFFF7E0)) * 1000) //芯片存储空间


//#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
// #define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
// #define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
//#elif defined STM32F10X_CL
// #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
// #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
//#elif defined STM32F10X_HD
// #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
// #define FLASH_SIZE                        (0x80000)  /* 512 KBytes */
//#elif defined STM32F10X_XL
// #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
// #define FLASH_SIZE                        (0x100000) /* 1 MByte */
//#else 
// #error "Please select first the STM32 device"    
//#endif


#define ADU_LENGTH 0x400




/* Exported types 
------------------------------------------------------------*/
typedef enum {
	eCOMFlashAddr = 0,	//judge enter ext flash update //2017.09.15 move here  
	eCOMChoose,
	eCOMDisplay,    
	eCOMInput,
	eCOMReceive,
	eCOMFromFlash,	//read code from ext flash then cover current app	//2017.09.15 start use
}eCOM_STATUS;

typedef  void (*FunVoidType)(void);
typedef unsigned int (*FunWriteType)(unsigned char*, unsigned int, unsigned short);
typedef void (*FunProcessType)(unsigned char *, unsigned int *, volatile eCOM_STATUS *);

typedef struct
{
    unsigned int UpgrageOrNot;			 // 1为有升级   0为没有要升级
    unsigned int OldOrNew;
    struct FileInfo
    {
        unsigned int BinLength;			 //文件长度
        unsigned int CRCValue;			 //CRC校验	
        unsigned int Version;			 //更新时判断的版本
        unsigned int BinIndex;
//        unsigned char Firmware_Version[32]; //固件版本
    }BinInfo[2];
}Update_Set_TypeDef;

/* Exported constants --------------------------------------------------------*/

void CommonInit(void);
void JumpToApp(void);
void CommonExec(void);
void Print(unsigned char *str);

#endif
/*********************************** END **************************************/

