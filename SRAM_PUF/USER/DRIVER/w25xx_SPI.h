/***************************** (C) COPYRIGHT ************************************
* File Name          : w25xx_SPI.h
* Author             : 星光嵌入式
* Version            : V1.0
* Date               : 08/18/2016
* Description        : STM32F1的SPI接口W25Q128驱动程序头文件
* Note               : 版权所有  严禁盗版
********************************************************************************
* 实验平台:星光STM32F103开发板 
* 淘宝店铺: https://shop148702745.taobao.com/
* 技术论坛: www.feifanembed.com
* QQ群:542830257
********************************************************************************/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include "task_usb_recv.h"
#define W25_FLASH_SIZE	(1024*1024*16)

#define W25X10  0XEF10 		//128K字节
#define W25Q20 	0XEF11		//256K字节 
#define W25Q40  0XEF12 		//512K字节
#define W25Q80 	0XEF13		//1M字节 	
#define W25Q16 	0XEF14		//2M字节
#define W25Q32 	0XEF15 		//4M字节
#define W25Q64 	0XEF16		//8M字节
#define W25Q128 0XEF17		//16M字节

extern uint16_t SPI_FLASH_TYPE;		//定义我们使用的flash芯片型号

//W25Q128FV FLASH擦除操作码
#define  W25X_FLASH_ERASE_CHIP		0	//片擦除
#define  W25X_FLASH_ERASE_SECTOR	1	//扇区擦除
#define  W25X_FLASH_ERASE_BLOCK_32K	2	//32K块擦除
#define  W25X_FLASH_ERASE_BLOCK_64K	3	//64K块擦除	

//W25Q128FV FLASH读寄存器操作码
#define  W25X_FLASH_READ_STATUSREG1		0	//读寄存器1
#define  W25X_FLASH_READ_STATUSREG2		1	//读寄存器2
#define  W25X_FLASH_READ_STATUSREG3		2	//读寄存器3

//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02
#define W25X_32K_BlockErase		0x52
#define W25X_64K_BlockErase		0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


//状态寄存器位定义
#define  W25XFLASH_REG_BIT_BUSY	(1<<0)	//器件忙
#define  W25XFLASH_REG_BIT_WEL	(1<<1)	//写使能
#define  W25XFLASH_REG_BIT_BPX	(0x7<<2)
#define  W25XFLASH_REG_BIT_TB	(1<<5)	//顶部、底部块区保护
#define  W25XFLASH_REG_BIT_SRP	(1<<7)	//状态寄存器保护位


/*******************************************************************************
* Function Name  : W25_Flash_Init
* Description    : 初始化FLASH的spi接口
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void W25_Flash_Init(void);

/*******************************************************************************
* Function Name  : W25_Flash_ReadSR
* Description    : 读取SST_FLASH的状态寄存器
* Input          : None
* Output         : None
* Return         : 读到的状态寄存器数据
* Note			 : BIT7  6   5   4   3   2   1   0
				   SRP   NC  TB BP2 BP1 BP0 WEL BUSY
				   SRP:默认0,状态寄存器保护位,配合WP使用(0: 可读写 1: 只读)
				   NC :保留位,不用
				   TB: 顶部、底部块区保护位
				   BP2,BP1,BP0:FLASH区域写保护设置
				   WEL:写使能锁定
				   BUSY:忙标记位(1,忙;0,空闲)
				   默认:0x00
*******************************************************************************/
uint8_t W25_Flash_ReadSR(void);

/*******************************************************************************
* Function Name  : W25_FLASH_Write_SR
* Description    : 写W25_FLASH的状态寄存器
* Input          : None
* Output         : None
* Return         : None
* Note			 : 只有SRP,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
				   所有命令的执行都在CS拉高后
*******************************************************************************/
void W25_FLASH_Write_SR(uint8_t sr);

/*******************************************************************************
* Function Name  : W25_Flash_ReadID
* Description    : 读取芯片ID  
* Input          : None
* Output         : None
* Return         : 2个字节，第一个为制造商ID: 0xEF, 第二个为器件ID
				   W25X10   0XEF10 		//128K字节
				   W25Q20 	0XEF11		//256K字节 
				   W25Q40   0XEF12 		//512K字节
				   W25Q80 	0XEF13		//1M字节 	
				   W25Q16 	0XEF14		//2M字节
				   W25Q32 	0XEF15 		//4M字节
				   W25Q64 	0XEF16		//8M字节
* Note			 : 所有命令的执行都在CS拉高后
*******************************************************************************/   	  
uint16_t W25_Flash_ReadID(void);

/*******************************************************************************
* Function Name  : SST_Flash_ReadID
* Description    : 读取芯片JedecID  
* Input          : None
* Output         : None
* Return         : 3个字节，第一个为制造商ID: 0EF, 第二个为0x30,第三个为兼容性ID=器件ID+1
				   W25X10   0XEF10 		//128K字节
				   W25Q20 	0XEF11		//256K字节 
				   W25Q40   0XEF12 		//512K字节
				   W25Q80 	0XEF13		//1M字节 	
				   W25Q16 	0XEF14		//2M字节
				   W25Q32 	0XEF15 		//4M字节
				   W25Q64 	0XEF16		//8M字节
* Note			 : 所有命令的执行都在CS拉高后
*******************************************************************************/   	  
uint32_t W25_Flash_ReadJedecID(void);

/*******************************************************************************
* Function Name  : W25_Flash_Read
* Description    : 读取SPI接口的W25 FLASH,在指定地址开始读取指定长度的数据
* Input          : pBuffer:数据存储区
				   ReadAddr:开始读取的地址(24bit)
				   NumByteToRead:要读取的字节数(最大65535)
* Output         : None
* Return         : None
* Note			 : 数据传输是高位在前
*******************************************************************************/
void W25_Flash_Read(uint32_t ReadAddr, uint8_t* pBuffer, uint32_t NumByteToRead);

/*******************************************************************************
* Function Name  : W25_Flash_FastRead
* Description    : 以高速方式(<=50MHZ)从SPI FLASH读数据
* Input          : pBuffer:数据存储区
				   ReadAddr:开始读取的地址(24bit)
				   NumByteToRead:要读取的字节数(最大65535)
* Output         : None
* Return         : None
* Note			 : 数据传输是高位在前
*******************************************************************************/
void W25_Flash_FastRead(uint32_t ReadAddr, uint8_t* pBuffer, uint32_t NumByteToRead);

/*******************************************************************************
* Function Name  : W25X_Flash_PageProgram
* Description    : 页编程
* Input          : WriteAddr:写入的地址
				   pBuffer:写数据存储的缓冲区
				   NumByteToWrite:写入的值
* Output         : None
* Return         : None
* Note			 : 数据传输是高位在前, 写之前必须注意到写保护的存储影响
				   每页256个字节,每次编程都是256个字节,小于256个字节的话,其它字
				   节无意义。写的字节数超过256就会自动跳到该页的开始处继续写,外
				   加其它的字节构成256个字节覆盖此页
*******************************************************************************/
void W25X_Flash_PageProgram(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite);

/*******************************************************************************
* Function Name  : W25_FLASH_Erase
* Description    : W25 FLASH的擦除操作
* Input          : Type:擦除类型
				   EraseAddr:擦除首地址
* Output         : None
* Return         : None
* Note			 : 有片擦除、4K扇区擦除、64K块擦除
*******************************************************************************/
void W25_FLASH_Erase(uint8_t Type, uint32_t EraseAddr); 

/*******************************************************************************
* Function Name  : W25_Flash_Write_NoCheck
* Description    : 无检验写SPI FLASH,在指定地址开始写入指定长度的数据,但是要确保地址不越界!
* Input          : WriteAddr:开始写入的地址(24bit)
				   pBuffer:数据存储区
				   NumByteToWrite:要写入的字节数(最大65535)
* Output         : None
* Return         : None
* Note			 : 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
				   具有自动换页功能 
*******************************************************************************/
void W25_Flash_Write_NoCheck(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite);

 /*******************************************************************************
* Function Name  : W25_Flash_Write
* Description    : 写SPI FLASH,在指定地址开始写入指定长度的数据
* Input          : WriteAddr:开始写入的地址(24bit)，此处单位是B
				   pBuffer:数据存储区
				   NumByteToWrite:要写入的字节数(最大65535)
* Output         : None
* Return         : None
* Note			 : 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
				   具有自动换页功能,该函数带擦除操作!
*******************************************************************************/ 		 
void W25_Flash_Write(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite); 

 /*******************************************************************************
 * Function Name : W25_Flash_test
 * Description : 写SPI FLASH为全0，并读出，以测试FLASH功能是否正常
 * Input : None
 * Output : 状态，-1失败，0成功
 * Note : None
 *******************************************************************************/ 	
 int W25_Flash_test();
 
 /*******************************************************************************
 * Function Name : W25_Flash_Find_User
 * Description : 找到对应的用户，如果找到则返回用户地址，找不到则在新位置创建
								一个新用户，接着返回用户地址。在内存不足时返回-1
 * Input : 用户标签uint32_t[4]
 * Output : 状态，0失败，成功uint32_t ADD
 * Note : None
 *******************************************************************************/ 
 uint32_t W25_Flash_Find_User(uint32_t Label[4]);
 
 /*******************************************************************************
 * Function Name : W25_Flash_Find_File
 * Description : 找到对应的文件，如果找到则返回文件地址，找不到则在新位置创建
								一个新文件，接着返回文件地址。在内存不足时返回-1
 * Input : 文件地址uint32_t firstADD
 * Output : 状态，0失败，成功uint32_t ADD
 * Note : None
 *******************************************************************************/ 
 uint32_t W25_Flash_Find_File(uint32_t firstADD);