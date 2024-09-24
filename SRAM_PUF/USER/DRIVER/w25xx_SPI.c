/***************************** (C) COPYRIGHT ************************************
* File Name          : w25xx_SPI.c
* Author             : 星光嵌入式
* Version            : V1.0
* Date               : 08/18/2016
* Description        : STM32F1的SPI接口W25Q128驱动程序源文件
* Note               : 版权所有  严禁盗版
********************************************************************************
* 实验平台:星光STM32F103开发板 
* 淘宝店铺: https://shop148702745.taobao.com/
* 技术论坛: www.feifanembed.com
* QQ群:542830257
********************************************************************************/
#include "w25xx_SPI.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>



/*******************************************************************************
* Function Name  : W25_Flash_Init
* Description    : 初始化FLASH的spi接口
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void W25_Flash_Init(void)
{	
	//SPIx_Init(&W25_Handle);
}

/*******************************************************************************
* Function Name  : W25_Flash_ReadWriteByte
* Description    : Flash 读写接口函数
* Input          : TxData: 写入的数据
* Output         : None
* Return         : 读到的数据
* Note			 : 读写一个字节的数据
*******************************************************************************/
uint8_t W25_Flash_ReadWriteByte(uint8_t TxData)
{
	return SPI_ReadWriteByte(&W25_Handle,TxData);
}

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
uint8_t W25_Flash_ReadSR(void)   
{  
	uint8_t byte=0;  	 
	W25_SPI_FLASH_CSL();   	//使能器件   
	W25_Flash_ReadWriteByte(W25X_ReadStatusReg);    	//发送读取状态寄存器命令    
	byte = W25_Flash_ReadWriteByte(0xFF);             	//读取一个字节  
	W25_SPI_FLASH_CSH();                            	//取消片选     
	return byte;   
}

/*******************************************************************************
* Function Name  : W25_FLASH_Write_SR
* Description    : 写W25_FLASH的状态寄存器
* Input          : None
* Output         : None
* Return         : None
* Note			 : 只有SRP,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
				   所有命令的执行都在CS拉高后
*******************************************************************************/
void W25_FLASH_Write_SR(uint8_t sr)   
{  	 
	W25_SPI_FLASH_CSL();                            	//使能器件   
	W25_Flash_ReadWriteByte(W25X_WriteStatusReg);		//发送写取状态寄存器命令    
	W25_Flash_ReadWriteByte(sr);               			//写入一个字节  
	W25_SPI_FLASH_CSH();                           		//取消片选     	      
} 

/*******************************************************************************
* Function Name  : W25_FLASH_Write_Enable
* Description    : W25_FLASH写使能, 将WEL置位
* Input          : None
* Output         : None
* Return         : None
* Note			 : 所有命令的执行都在CS拉高后
*******************************************************************************/ 
void W25_FLASH_Write_Enable(void)   
{
	W25_SPI_FLASH_CSL();                           //使能器件   
    W25_Flash_ReadWriteByte(W25X_WriteEnable); 	   //发送写使能  
	W25_SPI_FLASH_CSH();                           //取消片选     	      
} 

/*******************************************************************************
* Function Name  : W25_FLASH_Write_Disable
* Description    : W25_FLASH写禁止, 将WEL清零  
* Input          : None
* Output         : None
* Return         : None
* Note			 : 所有命令的执行都在CS拉高后
*******************************************************************************/
void W25_FLASH_Write_Disable(void)   
{  
	W25_SPI_FLASH_CSL();                            //使能器件   
    W25_Flash_ReadWriteByte(W25X_WriteDisable);  	//发送写禁止指令    
	W25_SPI_FLASH_CSH();                            //取消片选     	      
}

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
                   W25Q128  0XEF17		//16M字节
* Note			 : 所有命令的执行都在CS拉高后
*******************************************************************************/
int aa;
uint16_t W25_Flash_ReadID(void)
{
	uint16_t Temp = 0;
		  
	W25_SPI_FLASH_CSL();			    
	aa = W25_Flash_ReadWriteByte(W25X_ManufactDeviceID);	//发送读取ID命令	    
	aa = W25_Flash_ReadWriteByte(0x00); 	    
	aa = W25_Flash_ReadWriteByte(0x00); 	    
	aa = W25_Flash_ReadWriteByte(0x00); 	 			   
	Temp |= W25_Flash_ReadWriteByte(0xFF)<<8;  
	Temp |= W25_Flash_ReadWriteByte(0xFF);	 
	W25_SPI_FLASH_CSH();				    
	return Temp;
}

/*******************************************************************************
* Function Name  : SST_Flash_ReadID
* Description    : 读取芯片JedecID  
* Input          : None
* Output         : None
* Return         : 3个字节，第一个为制造商ID: 0xEF, 第二个为0x30,第三个为兼容性ID=器件ID+1
				   W25X10   0XEF10 		//128K字节
				   W25Q20 	0XEF11		//256K字节 
				   W25Q40   0XEF12 		//512K字节
				   W25Q80 	0XEF13		//1M字节 	
				   W25Q16 	0XEF14		//2M字节
				   W25Q32 	0XEF15 		//4M字节
				   W25Q64 	0XEF16		//8M字节
* Note			 : 所有命令的执行都在CS拉高后
*******************************************************************************/   
uint32_t Temp = 0;
uint32_t W25_Flash_ReadJedecID(void)
{
	
		  
	W25_SPI_FLASH_CSL();			    
	W25_Flash_ReadWriteByte(W25X_JedecDeviceID);	//发送读取ID命令	     
	Temp |= W25_Flash_ReadWriteByte(0xFF)<<16; 	 			   
	Temp |= W25_Flash_ReadWriteByte(0xFF)<<8;  
	Temp |= W25_Flash_ReadWriteByte(0xFF);	 
	W25_SPI_FLASH_CSH();				    
	return Temp;
}

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
void W25_Flash_Read(uint32_t ReadAddr, uint8_t* pBuffer, uint32_t NumByteToRead)   
{ 
 	uint16_t i;
		 
	W25_SPI_FLASH_CSL();  					   			//使能器件  										                               
	
	W25_Flash_ReadWriteByte(W25X_ReadData);			//发送读取命令
    W25_Flash_ReadWriteByte((uint8_t)((ReadAddr)>>16));  	//发送24bit地址    
    W25_Flash_ReadWriteByte((uint8_t)((ReadAddr)>>8));   
    W25_Flash_ReadWriteByte((uint8_t)ReadAddr);  
	 
    for(i = 0;i < NumByteToRead; i++)
	{ 
        pBuffer[i] = W25_Flash_ReadWriteByte(0XFF);   //循环读数  
    }
	W25_SPI_FLASH_CSH();	//禁止器件	

    //器件忙等待
	while((W25_Flash_ReadSR()&W25XFLASH_REG_BIT_BUSY)==0x01);	//器件忙，等待	
}

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
void W25_Flash_FastRead(uint32_t ReadAddr, uint8_t* pBuffer, uint32_t NumByteToRead)
{
    uint16_t i;	

    W25_SPI_FLASH_CSL();
    
    // 发送高速读命令0x0B，3字节地址,dummy字节
	W25_Flash_ReadWriteByte(W25X_FastReadData);			//发送高速读命令
    W25_Flash_ReadWriteByte(((ReadAddr)>>16));  		//发送24bit地址    
    W25_Flash_ReadWriteByte(((ReadAddr)>>8));   
    W25_Flash_ReadWriteByte(ReadAddr);
	W25_Flash_ReadWriteByte(0xFF); 						//产生8个空闲时钟脉冲

    for(i = 0;i < NumByteToRead; i++)
	{ 
        pBuffer[i] = W25_Flash_ReadWriteByte(0XFF);   //循环读数  
    }
	W25_SPI_FLASH_CSH();	//禁止器件
	
	//器件忙等待
	while(W25_Flash_ReadSR()&W25XFLASH_REG_BIT_BUSY);	//器件忙，等待
} 

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
void W25X_Flash_PageProgram(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
	uint32_t i;

    // 发送写使能命令
    W25_FLASH_Write_Enable();

    // 发送低速读命令0xAD，3字节地址,写入的字节
    W25_SPI_FLASH_CSL();

	W25_Flash_ReadWriteByte(W25X_PageProgram);
    W25_Flash_ReadWriteByte(((WriteAddr)>>16));  		//发送24bit地址    
    W25_Flash_ReadWriteByte(((WriteAddr)>>8));   
    W25_Flash_ReadWriteByte(WriteAddr);

    // 发送其它字节
	for (i = 0; i < NumByteToWrite; i++) 
	{
		//器件忙等待
		W25_Flash_ReadWriteByte(pBuffer[i]);
    }     
    W25_SPI_FLASH_CSH();	//禁止器件
	
	//器件忙等待
	while(W25_Flash_ReadSR()&W25XFLASH_REG_BIT_BUSY);	//器件忙，等待
}

/*******************************************************************************
* Function Name  : W25_FLASH_Erase
* Description    : W25 FLASH的擦除操作
* Input          : Type:擦除类型
				   EraseAddr:擦除首地址
* Output         : None
* Return         : None
* Note			 : 有片擦除、4K扇区擦除、64K块擦除
                   EraseAddr必须是一个扇区或一个块的起始地址
                   整片擦除时间有点长要十几秒
*******************************************************************************/
void W25_FLASH_Erase(uint8_t Type, uint32_t EraseAddr)
{
	// 发送写使能命令
    W25_FLASH_Write_Enable();

	switch(Type)
	{
		case W25X_FLASH_ERASE_CHIP:			//整片擦除
			W25_SPI_FLASH_CSL();
    		W25_Flash_ReadWriteByte(W25X_ChipErase);
    		W25_SPI_FLASH_CSH();	//禁止器件
			break;
		case W25X_FLASH_ERASE_SECTOR:		//4K扇区擦除
			W25_SPI_FLASH_CSL();
    		W25_Flash_ReadWriteByte(W25X_SectorErase);
			W25_Flash_ReadWriteByte(((EraseAddr)>>16));  		//发送24bit地址    
    		W25_Flash_ReadWriteByte(((EraseAddr)>>8));   
    		W25_Flash_ReadWriteByte(EraseAddr);
    		W25_SPI_FLASH_CSH();	//禁止器件
			break;
		case W25X_FLASH_ERASE_BLOCK_32K:		//32K块擦除
			W25_SPI_FLASH_CSL();
    		W25_Flash_ReadWriteByte(W25X_32K_BlockErase);
			W25_Flash_ReadWriteByte(((EraseAddr)>>16));  		//发送24bit地址    
    		W25_Flash_ReadWriteByte(((EraseAddr)>>8));   
    		W25_Flash_ReadWriteByte(EraseAddr);
    		W25_SPI_FLASH_CSH();	//禁止器件
			break;
		case W25X_FLASH_ERASE_BLOCK_64K:		//64K块擦除
			W25_SPI_FLASH_CSL();
    		W25_Flash_ReadWriteByte(W25X_64K_BlockErase);
			W25_Flash_ReadWriteByte(((EraseAddr)>>16));  		//发送24bit地址    
    		W25_Flash_ReadWriteByte(((EraseAddr)>>8));   
    		W25_Flash_ReadWriteByte(EraseAddr);
    		W25_SPI_FLASH_CSH();	//禁止器件
			break;
	  	default: break;
	}
	int wait=0;
	//器件忙等待
	if(Type == W25X_FLASH_ERASE_CHIP)
		output("\r\nWaiting for Flash Erase!\r\n");
	while(W25_Flash_ReadSR()&W25XFLASH_REG_BIT_BUSY){//器件忙，等待
		if(Type == W25X_FLASH_ERASE_CHIP){
			wait++;
			if(wait == 1000000){
				output("\r\nWaiting for Flash Erase!\r\n");
				wait = 0;
			}
		}
	}	
}

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
void W25_Flash_Write_NoCheck(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	 
	  
	pageremain = 256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite;//不大于256个字节

	while(1)
	{	   
		W25X_Flash_PageProgram(WriteAddr,pBuffer,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 

uint8_t W25SPI_FLASH_BUFFER[4096];
/*******************************************************************************
* Function Name  : W25_Flash_Write
* Description    : 写SPI FLASH,在指定地址开始写入指定长度的数据
* Input          : WriteAddr:开始写入的地址(24bit)
				   pBuffer:数据存储区
				   NumByteToWrite:要写入的字节数(最大65535)
* Output         : None
* Return         : None
* Note			 : 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
				   具有自动换页功能,该函数带擦除操作!
*******************************************************************************/ 		 
void W25_Flash_Write(uint32_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;			//扇区地址
	uint16_t secoff;			//扇区内偏移
	uint16_t secremain;		//扇区剩余空间	   
 	uint16_t i;    
	uint8_t * SPI_FLASH_BUF;	
	  
   	SPI_FLASH_BUF = W25SPI_FLASH_BUFFER;			//可以通过申请内存的方式实现	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   

 	if(NumByteToWrite <= secremain) secremain = NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25_Flash_FastRead(secpos*4096, SPI_FLASH_BUF, 4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25_FLASH_Erase(W25X_FLASH_ERASE_SECTOR,secpos*4096);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			W25_Flash_Write_NoCheck(secpos*4096,SPI_FLASH_BUF,4096);//写入整个扇区  
		}
		else 
		{
			W25_Flash_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 
		}				   
		if(NumByteToWrite == secremain)
		{
			break;//写入结束了
		}
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	}	 
}

int W25_Flash_test(){
	int status = 0; 
	unsigned char Buffer[64];
	W25_Flash_Read(0,Buffer,64);
	W25_FLASH_Erase(3,0); //做全盘擦除
	for(int i = 0 ; i < 64 ; i++){
		Buffer[i] = '0';
	}
	W25_Flash_Write_NoCheck(0,Buffer,64);
	W25_Flash_Read(0,Buffer,64);
	for(int i = 0 ; i < 64 ; i++){
		if(Buffer[i] != '0')
			status = -1 ;
	}
	return status;
 }