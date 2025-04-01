#include "sram.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
#include "cmsis_os.h"
#include "gpio.h"
#include "stdio.h"

void sram_IO_input(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};


  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                          PCPin PCPin */
  GPIO_InitStruct.Pin = SRAM_IO4_Pin|SRAM_IO5_Pin|SRAM_IO6_Pin|SRAM_IO7_Pin
                          |SRAM_IO9_Pin|SRAM_IO8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = SRAM_IO14_Pin|SRAM_IO15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = SRAM_IO13_Pin|SRAM_IO12_Pin|SRAM_IO11_Pin|SRAM_IO10_Pin
                          |SRAM_IO0_Pin|SRAM_IO1_Pin|SRAM_IO2_Pin|SRAM_IO3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}



void sram_IO_output(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};



  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin */
  GPIO_InitStruct.Pin = SRAM_IO4_Pin|SRAM_IO5_Pin|SRAM_IO6_Pin|SRAM_IO7_Pin
                          |SRAM_IO9_Pin|SRAM_IO8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin PCPin PCPin */
  GPIO_InitStruct.Pin = SRAM_WE_Pin|SRAM_OE_Pin|SRAM_A7_Pin|SRAM_A8_Pin
                          |SRAM_A9_Pin|SRAM_A3_Pin|SRAM_A2_Pin|SRAM_A1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin
                           PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = FLASH_WP_Pin|FLASH_CS_Pin|FLASH_RST_Pin|SRAM_PWR_Pin
                          |SRAM_LB_Pin|SRAM_UB_Pin|SRAM_A10_Pin|SRAM_A4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = SRAM_IO14_Pin|SRAM_IO15_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = SRAM_A6_Pin|SRAM_A5_Pin|SRAM_CS1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin PBPin PBPin
                           PBPin PBPin PBPin PBPin */
  GPIO_InitStruct.Pin = SRAM_IO13_Pin|SRAM_IO12_Pin|SRAM_IO11_Pin|SRAM_IO10_Pin
                          |SRAM_IO0_Pin|SRAM_IO1_Pin|SRAM_IO2_Pin|SRAM_IO3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SRAM_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SRAM_A0_GPIO_Port, &GPIO_InitStruct);

}


void sram_GPIO_DeInit(void)
{
  HAL_GPIO_DeInit(SRAM_IO0_GPIO_Port, SRAM_IO0_Pin);
  HAL_GPIO_DeInit(SRAM_IO1_GPIO_Port, SRAM_IO1_Pin);
  HAL_GPIO_DeInit(SRAM_IO2_GPIO_Port, SRAM_IO2_Pin);
  HAL_GPIO_DeInit(SRAM_IO3_GPIO_Port, SRAM_IO3_Pin);
  HAL_GPIO_DeInit(SRAM_IO4_GPIO_Port, SRAM_IO4_Pin);
  HAL_GPIO_DeInit(SRAM_IO5_GPIO_Port, SRAM_IO5_Pin);
  HAL_GPIO_DeInit(SRAM_IO6_GPIO_Port, SRAM_IO6_Pin);
  HAL_GPIO_DeInit(SRAM_IO7_GPIO_Port, SRAM_IO7_Pin);
  HAL_GPIO_DeInit(SRAM_IO8_GPIO_Port, SRAM_IO8_Pin);
  HAL_GPIO_DeInit(SRAM_IO9_GPIO_Port, SRAM_IO9_Pin);
  HAL_GPIO_DeInit(SRAM_IO10_GPIO_Port, SRAM_IO10_Pin);
  HAL_GPIO_DeInit(SRAM_IO11_GPIO_Port, SRAM_IO11_Pin);
  HAL_GPIO_DeInit(SRAM_IO12_GPIO_Port, SRAM_IO12_Pin);
  HAL_GPIO_DeInit(SRAM_IO13_GPIO_Port, SRAM_IO13_Pin);
  HAL_GPIO_DeInit(SRAM_IO14_GPIO_Port, SRAM_IO14_Pin);
  HAL_GPIO_DeInit(SRAM_IO15_GPIO_Port, SRAM_IO15_Pin);
	
	HAL_GPIO_DeInit(SRAM_A0_GPIO_Port, SRAM_A0_Pin);
  HAL_GPIO_DeInit(SRAM_A1_GPIO_Port, SRAM_A1_Pin);
  HAL_GPIO_DeInit(SRAM_A2_GPIO_Port, SRAM_A2_Pin);
  HAL_GPIO_DeInit(SRAM_A3_GPIO_Port, SRAM_A3_Pin);
  HAL_GPIO_DeInit(SRAM_A4_GPIO_Port, SRAM_A4_Pin);
  HAL_GPIO_DeInit(SRAM_A5_GPIO_Port, SRAM_A5_Pin);
  HAL_GPIO_DeInit(SRAM_A6_GPIO_Port, SRAM_A6_Pin);
  HAL_GPIO_DeInit(SRAM_A7_GPIO_Port, SRAM_A7_Pin);
  HAL_GPIO_DeInit(SRAM_A8_GPIO_Port, SRAM_A8_Pin);
  HAL_GPIO_DeInit(SRAM_A9_GPIO_Port, SRAM_A9_Pin);
  HAL_GPIO_DeInit(SRAM_A10_GPIO_Port, SRAM_A10_Pin);
	
	HAL_GPIO_DeInit(SRAM_WE_GPIO_Port, SRAM_WE_Pin);
  HAL_GPIO_DeInit(SRAM_OE_GPIO_Port, SRAM_OE_Pin);
	
  HAL_GPIO_DeInit(SRAM_CS1_GPIO_Port, SRAM_CS1_Pin);
	
	HAL_GPIO_DeInit(SRAM_LB_GPIO_Port, SRAM_LB_Pin);
  HAL_GPIO_DeInit(SRAM_UB_GPIO_Port, SRAM_UB_Pin);
}


void sram_pin_init()
{
	HAL_GPIO_WritePin(SRAM_LB_GPIO_Port,SRAM_LB_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SRAM_UB_GPIO_Port,SRAM_UB_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_WE_Pin,GPIO_PIN_SET);
}



void set_addr(uint32_t addr)
{
			HAL_GPIO_WritePin(SRAM_A0_GPIO_Port,SRAM_A0_Pin,(GPIO_PinState)(addr&(1<<0)));
			HAL_GPIO_WritePin(SRAM_A1_GPIO_Port,SRAM_A1_Pin,(GPIO_PinState)(addr&(1<<1)));
			HAL_GPIO_WritePin(SRAM_A2_GPIO_Port,SRAM_A2_Pin,(GPIO_PinState)(addr&(1<<2)));
			HAL_GPIO_WritePin(SRAM_A3_GPIO_Port,SRAM_A3_Pin,(GPIO_PinState)(addr&(1<<3)));
			HAL_GPIO_WritePin(SRAM_A4_GPIO_Port,SRAM_A4_Pin,(GPIO_PinState)(addr&(1<<4)));
			HAL_GPIO_WritePin(SRAM_A5_GPIO_Port,SRAM_A5_Pin,(GPIO_PinState)(addr&(1<<5)));
			HAL_GPIO_WritePin(SRAM_A6_GPIO_Port,SRAM_A6_Pin,(GPIO_PinState)(addr&(1<<6)));
			HAL_GPIO_WritePin(SRAM_A7_GPIO_Port,SRAM_A7_Pin,(GPIO_PinState)(addr&(1<<7)));
			HAL_GPIO_WritePin(SRAM_A8_GPIO_Port,SRAM_A8_Pin,(GPIO_PinState)(addr&(1<<8)));
			HAL_GPIO_WritePin(SRAM_A9_GPIO_Port,SRAM_A9_Pin,(GPIO_PinState)(addr&(1<<9)));
			HAL_GPIO_WritePin(SRAM_A10_GPIO_Port,SRAM_A10_Pin,(GPIO_PinState)(addr&(1<<10)));
}


unsigned short read_data( uint32_t addr )
{
		unsigned short sramdata;
	
		set_addr(addr);
		HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_WE_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(SRAM_OE_GPIO_Port,SRAM_OE_Pin,GPIO_PIN_RESET);
	
		sramdata = HAL_GPIO_ReadPin(SRAM_IO0_GPIO_Port,SRAM_IO0_Pin) << 0 |
							HAL_GPIO_ReadPin(SRAM_IO1_GPIO_Port,SRAM_IO1_Pin) << 1 |
							HAL_GPIO_ReadPin(SRAM_IO2_GPIO_Port,SRAM_IO2_Pin) << 2 |
							HAL_GPIO_ReadPin(SRAM_IO3_GPIO_Port,SRAM_IO3_Pin) << 3 |
							HAL_GPIO_ReadPin(SRAM_IO4_GPIO_Port,SRAM_IO4_Pin) << 4 |
							HAL_GPIO_ReadPin(SRAM_IO5_GPIO_Port,SRAM_IO5_Pin) << 5 |
							HAL_GPIO_ReadPin(SRAM_IO6_GPIO_Port,SRAM_IO6_Pin) << 6 |
							HAL_GPIO_ReadPin(SRAM_IO7_GPIO_Port,SRAM_IO7_Pin) << 7 |
							HAL_GPIO_ReadPin(SRAM_IO8_GPIO_Port,SRAM_IO8_Pin) << 8 |
							HAL_GPIO_ReadPin(SRAM_IO9_GPIO_Port,SRAM_IO9_Pin) << 9 |
							HAL_GPIO_ReadPin(SRAM_IO10_GPIO_Port,SRAM_IO10_Pin) << 10 |
							HAL_GPIO_ReadPin(SRAM_IO11_GPIO_Port,SRAM_IO11_Pin) << 11 |
							HAL_GPIO_ReadPin(SRAM_IO12_GPIO_Port,SRAM_IO12_Pin) << 12 |
							HAL_GPIO_ReadPin(SRAM_IO13_GPIO_Port,SRAM_IO13_Pin) << 13 |
							HAL_GPIO_ReadPin(SRAM_IO14_GPIO_Port,SRAM_IO14_Pin) << 14 |
							HAL_GPIO_ReadPin(SRAM_IO15_GPIO_Port,SRAM_IO15_Pin) << 15;
							
		HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_OE_Pin,GPIO_PIN_SET);
							
	  return sramdata;
}

unsigned short write_data( uint32_t addr ,unsigned short data)
{
		unsigned short sramdata;
	
		set_addr(addr);
	

		HAL_GPIO_WritePin(SRAM_IO0_GPIO_Port,SRAM_IO0_Pin,  (GPIO_PinState)(data&(1<<0)));
		HAL_GPIO_WritePin(SRAM_IO1_GPIO_Port,SRAM_IO1_Pin,  (GPIO_PinState)(data&(1<<1)));
		HAL_GPIO_WritePin(SRAM_IO2_GPIO_Port,SRAM_IO2_Pin,  (GPIO_PinState)(data&(1<<2)));
		HAL_GPIO_WritePin(SRAM_IO3_GPIO_Port,SRAM_IO3_Pin,  (GPIO_PinState)(data&(1<<3)));
		HAL_GPIO_WritePin(SRAM_IO4_GPIO_Port,SRAM_IO4_Pin,  (GPIO_PinState)(data&(1<<4)));
		HAL_GPIO_WritePin(SRAM_IO5_GPIO_Port,SRAM_IO5_Pin,  (GPIO_PinState)(data&(1<<5)));
		HAL_GPIO_WritePin(SRAM_IO6_GPIO_Port,SRAM_IO6_Pin,  (GPIO_PinState)(data&(1<<6)));
		HAL_GPIO_WritePin(SRAM_IO7_GPIO_Port,SRAM_IO7_Pin,  (GPIO_PinState)(data&(1<<7)));
		HAL_GPIO_WritePin(SRAM_IO8_GPIO_Port,SRAM_IO8_Pin,  (GPIO_PinState)(data&(1<<8)));
		HAL_GPIO_WritePin(SRAM_IO9_GPIO_Port,SRAM_IO9_Pin,  (GPIO_PinState)(data&(1<<9)));
		HAL_GPIO_WritePin(SRAM_IO10_GPIO_Port,SRAM_IO10_Pin,  (GPIO_PinState)(data&(1<<10)));
		HAL_GPIO_WritePin(SRAM_IO11_GPIO_Port,SRAM_IO11_Pin,  (GPIO_PinState)(data&(1<<11)));
		HAL_GPIO_WritePin(SRAM_IO12_GPIO_Port,SRAM_IO12_Pin,  (GPIO_PinState)(data&(1<<12)));
		HAL_GPIO_WritePin(SRAM_IO13_GPIO_Port,SRAM_IO13_Pin,  (GPIO_PinState)(data&(1<<13)));
		HAL_GPIO_WritePin(SRAM_IO14_GPIO_Port,SRAM_IO14_Pin,  (GPIO_PinState)(data&(1<<14)));
		HAL_GPIO_WritePin(SRAM_IO15_GPIO_Port,SRAM_IO15_Pin,  (GPIO_PinState)(data&(1<<15)));
	
	
	  HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_WE_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(SRAM_OE_GPIO_Port,SRAM_OE_Pin,GPIO_PIN_RESET);
	
		HAL_GPIO_WritePin(SRAM_CS1_GPIO_Port,SRAM_CS1_Pin,GPIO_PIN_SET);
	  HAL_GPIO_WritePin(SRAM_WE_GPIO_Port,SRAM_OE_Pin,GPIO_PIN_SET);
							
	  return sramdata;
}


void sram_test()
{
	unsigned short buf[512];
	unsigned short buf_read[512]; 
	
	for(int i = 0;i<512;i++){
			buf[i] = i;
	}
		
	sram_write(0,buf,512);
	sram_read(0,buf_read,512);
	
	for(int i = 0;i<512;i++){
		printf("%d\r\n",(int)buf_read[i]);
	}
}


void sram_read(int p, unsigned short* buf,int len)
{
		sram_IO_input();
		for(int i = 0;i<len ;i++){
			buf[i] =  read_data( p+((i*8)+3)%1024 );
		}
}


void sram_write(int p,unsigned short* buf,int len)
{
		sram_IO_output();
		for(int i = 0;i<len ;i++){
			 //write_data( p+i ,buf[i]);
			write_data( p+i ,i%5);
		}
}


//获取随机数
// int p                 读取内存位置
// unsigned char* buf    读取数据存放位置
// int len               读取长度
// int time              断电~通电时间
void sram_read_random(int p, unsigned short* buf,int len,int time)
{
		//sram断电
		SRAM_PWR_L();
		sram_GPIO_DeInit();
		osDelay(time);
	
		//sram上电
		SRAM_PWR_H();
	  MX_GPIO_Init();

		sram_read(p, buf,len);
		//for(int i = 0;i<len;i++){
			//printf("%d\r\n",(int)buf[i]);
		//}
		SRAM_PWR_L();
		sram_GPIO_DeInit();
}

//这个方法用来输出源值
void sram_source_Sram(int p, unsigned short* buf,int len,int time)
{
		//sram断电
		SRAM_PWR_L();
		sram_GPIO_DeInit();
		osDelay(time);
	
		//sram上电
		SRAM_PWR_H();
	  MX_GPIO_Init();
		
		sram_read(p, buf,len);
		for(int i = 0;i<len;i++){
			printf("%d",(int)buf[i]);
		}
		SRAM_PWR_L();
		sram_GPIO_DeInit();
		
}

//SRAM PUF初始化操作
void SPI_SRAM_PUF_Init(void) {
    uint32_t randoms[21];   // 直接读取到 randoms
    uint32_t sramData[32];  // 直接读取到 sramData
    uint32_t xorData[32];		//存储异或后的数组
    uint32_t codeData[32];	//存储编码后的数组
    uint32_t new_randoms[32];	//验证是否正确
    char message[128];	//消息缓存
    int new_length;

    memset(message, 0, sizeof(message)); //缓存置零
    output("Starting Initialize steps");

    // 直接将数据读取到最终需要的数组中
    sram_read_random(0, (uint16_t*)sramData, sizeof(sramData)/sizeof(uint16_t), DELAY_TIME);
    sram_read_random(100, (uint16_t*)randoms, sizeof(randoms)/sizeof(uint16_t), DELAY_TIME);
	
		//编码与解码（解码目的是确定编码成功）
    int n = sizeof(randoms) / sizeof(randoms[0]);
    bch_encoder(randoms, n, &new_length, codeData);
    bch_decoder(codeData, new_length, &new_length, new_randoms);
		
		//组成helpdata，并输出
    output("Get help data:\n");
    for (int i = 0; i < 32; i++) {
        xorData[i] = codeData[i] ^ sramData[i];
        sprintf(message + strlen(message), "%08x", xorData[i]);
    }
    usb_send((unsigned char*)message, strlen(message));
		
		//验证编码成功
    if (randoms[0] == new_randoms[0]) {
        output("\nInitialize program success!");
    }
		
		//清除第一块sector(4KB)，用于存储关键信息，目前已经使用512B
    W25_FLASH_Erase(0, 0);
    W25_Flash_Write(0, (uint8_t*)xorData, 128);
    W25_Flash_Write(128, (uint8_t*)randoms, 128);
    W25_Flash_Write(256, (uint8_t*)randoms, 128);
    W25_Flash_Write(384, (uint8_t*)randoms, 128);
		uint32_t curPoint = userInfoStartAdd;
		W25_Flash_Write(512, (uint8_t*)&curPoint, 4);
		curPoint = fileInfoStartADD;
		W25_Flash_Write(516, (uint8_t*)&curPoint, 4);//重置两个地址
		
		//验证保存在FLASH中的信息没错。
    uint32_t tmp1[32]={0};
    uint32_t tmp2[32]={0};
    W25_Flash_Read(0, (uint8_t*)tmp1, 128);
    W25_Flash_Read(128, (uint8_t*)tmp2, 128);

    if (tmp1[0] == xorData[0] && tmp2[0] == randoms[0]) {
        output("Successful store helpData!");
    } else {
        output("Failure occurred: store program failed!");
    }
}

//读取SRAM PUF稳定输出
void SPI_SRAM_PUF_Stable_Output(uint32_t *Data){
	unsigned short tmp_sramData[64]; //读内存单元值
	uint32_t sramData[32];
	uint32_t tmp[32];
	uint32_t xorData[32];
	uint32_t codeData[32];
	int new_length = -1;
	
	sram_read_random(0, tmp_sramData,sizeof(tmp_sramData)/sizeof(tmp_sramData[0]),DELAY_TIME);
	for(int i = 0; i<sizeof(tmp_sramData)/sizeof(tmp_sramData[0]);i=i+2){
		sramData[i/2] = (uint32_t)tmp_sramData[i]<<16 | (uint32_t)tmp_sramData[i+1];
	}//读新的sram值
	W25_Flash_Read(0,(uint8_t *) tmp , 128);//读helpData
	for(int i=0;i<32;i++){
		xorData[i]=tmp[i] ^ sramData[i];//可能存在错误的随机值
	}
	bch_decoder(xorData,32,&new_length,codeData);//纠正后的随机值
	bch_encoder(codeData,32,&new_length,xorData);//重新编码存在xorData中
	for (int i = 0; i < 32; i++) {
        Data[i] = tmp[i] ^ xorData[i];
    }
}
	
//输出强PUF功能
void SPI_SRAM_PUF_STRONG(unsigned char* Messages,int len){
	SHA512_CTX ctx;
	uint8_t hash[SHA512_BLOCK_SIZE];
	uint32_t hash1[16];
  // 计算前512位的哈希值
	sha512_init(&ctx);
  sha512_update(&ctx, Messages, len);
  sha512_final(&ctx, hash);
	uint32_t data[32];
		
  SPI_SRAM_PUF_Stable_Output(data);  // 读稳定输出
	output("Response:\n");
	for (int i = 0; i < 16; i++) {
		hash1[i] = ((uint32_t)hash[4*i] << 24) | ((uint32_t)hash[4*i+1] << 16) |
		((uint32_t)hash[4*i+2] << 8) | (uint32_t)hash[4*i+3];
	}
  for (int i = 0; i < 32; i++) {
    printf("%08x", data[i]^hash1[i%16]);
  }
}

void SPI_SRAM_PUF_FILE(unsigned char* Infos,int len){
		statuMessage.data.status = 6;
		if(len<32){
			output("Invalid inputs!");
			return;
		}
		struct userInfo User; //存储用户信息
		struct fileInfo File; //存储文件信息
		
		//信息解析
		for(int i = 0; i < 4; i++) {
			// 组合 4 个字节为 1 个 uint32_t，赋给 User.userLabel[i]
			User.userLabel[i] = (Infos[i * 4] << 24) | (Infos[i * 4 + 1] << 16) | (Infos[i * 4 + 2] << 8) | Infos[i * 4 + 3];
			statuMessage.data.userLabel[i] = User.userLabel[i];
			// 组合 4 个字节为 1 个 uint32_t，赋给 File.fileLabel[i]
			File.fileLabel[i] = (Infos[i * 4 + 16] << 24) | (Infos[i * 4 + 17] << 16) | (Infos[i * 4 + 18] << 8) | Infos[i * 4 + 19];
			statuMessage.data.fileLabel[i] = File.fileLabel[i];
		}
		//取当前指针地址
		uint32_t curUserPoint = 0;
		uint32_t curFilePoint = 0;
		W25_Flash_Read(512, (uint8_t*)&curUserPoint, 4);
		W25_Flash_Read(516, (uint8_t*)&curFilePoint, 4);
		int maxFile = MAXFILEADD;
		int maxUser = MAXUSERADD;
		double userSpace = ((double)curUserPoint-520)/((double)maxUser-520)*100;
		double fileSpace = ((double)curFilePoint-4096)/((double)maxFile-4096)*100;
		statuMessage.data.userSpace = userSpace;
		statuMessage.data.fileSpace = fileSpace;
		//printf("当前存储空间：用户空间%f％，文件空间%f％\r\n",userSpace,fileSpace);
		if((curFilePoint + 148) > maxFile || (curUserPoint+20)>maxUser){ //检查是否越界
			//output("Store space is full!");
			statuMessage.data.status = 8;
			usb_send(statuMessage.bytes,sizeof(statuMessage.bytes));
			return;
		}
		//printf("用户标签：");
		//for(int i = 0 ; i < 4 ; i++){
			//printf("%08x",User.userLabel[i]);
		//}
		//printf("\r\n文件标签：");
		//for(int i = 0 ; i < 4 ; i++){
			//printf("%08x",File.fileLabel[i]);
		//}
		//查找用户
		int flag = 0;
		uint32_t startPoint = userInfoStartAdd;
		for(;startPoint<curUserPoint;startPoint+=20){
			uint32_t tempUser[4];
			W25_Flash_Read(startPoint,(uint8_t*)tempUser,16);
			flag = 0;
			for(int i = 0 ; i < 4 ; i++){
				if(tempUser[i] == User.userLabel[i])
					flag++;
			}
			if(flag == 4){
				//printf("\r\n找到已存在用户！\r\n");
				break;
			}
		}

		//查找文件
		if(flag !=4){ //没找到用户，创建用户，并保存文件
			//printf("\r\n未找到已存在用户，正在创建相关信息。\r\n");
			User.firstAdd = curFilePoint;
			W25_Flash_Write(curUserPoint, (uint8_t*)&User, sizeof(User)); // 将当前用户信息保存
			
			SPI_SRAM_PUF_FILE_LABEL(File.fileLabel,sizeof(File.fileLabel),(uint32_t *)&File.helpData); //生成helpdata，输出密钥
			File.nextAdd = 0;
			//usb_send((unsigned char*)File.helpData, 128);//输出处理后的helpdata
			
			W25_Flash_Write(curFilePoint, (uint8_t*)&File, sizeof(File));
			//更新指针，保存指针
			curFilePoint += 148;
			W25_Flash_Write(516, (uint8_t*)&curFilePoint, 4);
			curUserPoint +=	20;
			W25_Flash_Write(512, (uint8_t*)&curUserPoint, 4);//更新当前指针
			userSpace = ((double)curUserPoint-520)/((double)maxUser-520)*100;
			fileSpace = ((double)curFilePoint-4096)/((double)maxFile-4096)*100;
			statuMessage.data.userSpace = userSpace;
			statuMessage.data.fileSpace = fileSpace;
			
			uint32_t sramData[32];
			uint32_t xorData[32];
			uint32_t codeData[32];
			uint32_t Data[32];
			int new_length = -1;
	
			sram_read_random(0, (uint16_t*)sramData, sizeof(sramData)/sizeof(uint16_t), DELAY_TIME);//读新的sram值
			for(int i=0;i<32;i++){
					xorData[i]=File.helpData[i] ^ sramData[i];//可能存在错误的随机值
			}
			bch_decoder(xorData,32,&new_length,codeData);//纠正后的随机值
			bch_encoder(codeData,32,&new_length,xorData);//重新编码存在xorData中
			for (int i = 0; i < 32; i++) {
					Data[i] = File.helpData[i] ^ xorData[i];
					statuMessage.data.key[i] = Data[i];
					//printf("%08x",Data[i]);
			}
			//printf("当前存储空间：用户空间%f％，文件空间%f％\r\n",userSpace,fileSpace);
		}
		else{ //找到用户，就查找文件，存在则直接取helpdata，不存在则生成helpdata，接着都生成密钥
			W25_Flash_Read(startPoint,(uint8_t*)&User, sizeof(User)); //取查找到的用户信息
			struct fileInfo tmpFile;
			int curtmpFile = User.firstAdd;
			W25_Flash_Read(User.firstAdd,(uint8_t*)&tmpFile,sizeof(tmpFile));
			while(tmpFile.nextAdd !=0 && tmpFile.fileLabel != File.fileLabel){ //查找是否存在该文件
				curtmpFile = tmpFile.nextAdd;
				W25_Flash_Read(tmpFile.nextAdd,(uint8_t*)&tmpFile,sizeof(tmpFile));
			}
			int flag = 1;
			for(int i = 0 ; i < 4 ; i++){
				if(tmpFile.fileLabel[i] != File.fileLabel[i])
					flag =0;
			}
			if(flag){
					//printf("\r\n查找到相同文件，正在恢复密钥。\r\n");
					uint32_t sramData[32];
					uint32_t xorData[32];
					uint32_t codeData[32];
					uint32_t Data[32];
					int new_length = -1;
	
					sram_read_random(0, (uint16_t*)sramData, sizeof(sramData)/sizeof(uint16_t), DELAY_TIME);//读新的sram值
					for(int i=0;i<32;i++){
							xorData[i]=tmpFile.helpData[i] ^ sramData[i];//可能存在错误的随机值
					}
					bch_decoder(xorData,32,&new_length,codeData);//纠正后的随机值
					bch_encoder(codeData,32,&new_length,xorData);//重新编码存在xorData中
					for (int i = 0; i < 32; i++) {
							Data[i] = tmpFile.helpData[i] ^ xorData[i];
							statuMessage.data.key[i] = Data[i];
							//printf("%08x",Data[i]);
					}
			}
			else{
				//printf("\r\n未查找到文件，正在创建相关信息。\r\n");
				SPI_SRAM_PUF_FILE_LABEL(File.fileLabel,sizeof(File.fileLabel),(uint32_t *)&File.helpData); //生成helpdata，输出密钥
				File.nextAdd = 0;
			
				W25_Flash_Write(curFilePoint, (uint8_t*)&File, sizeof(File));
				tmpFile.nextAdd = curFilePoint;
				W25_Flash_Write(curtmpFile,(uint8_t*)&tmpFile, sizeof(tmpFile)); //更新最后一个文件的nextadd
				
				//更新指针，保存指针
				curFilePoint += 148;
				W25_Flash_Write(516, (uint8_t*)&curFilePoint, 4);//更新当前指针
				userSpace = ((double)curUserPoint-520)/((double)maxUser-520)*100;
				fileSpace = ((double)curFilePoint-4096)/((double)maxFile-4096)*100;
				statuMessage.data.userSpace = userSpace;
				statuMessage.data.fileSpace = fileSpace;
				//printf("当前存储空间：用户空间%f％，文件空间%f％\r\n",userSpace,fileSpace);
				
				uint32_t sramData[32];
				uint32_t xorData[32];
				uint32_t codeData[32];
				uint32_t Data[32];
				int new_length = -1;
	
				sram_read_random(0, (uint16_t*)sramData, sizeof(sramData)/sizeof(uint16_t), DELAY_TIME);//读新的sram值
				for(int i=0;i<32;i++){
					xorData[i]=File.helpData[i] ^ sramData[i];//可能存在错误的随机值
				}
				bch_decoder(xorData,32,&new_length,codeData);//纠正后的随机值
				bch_encoder(codeData,32,&new_length,xorData);//重新编码存在xorData中
				for (int i = 0; i < 32; i++) {
					Data[i] = File.helpData[i] ^ xorData[i];
					statuMessage.data.key[i] = Data[i];
					//printf("%08x",Data[i]);
				}
			}
		}
		usb_send(statuMessage.bytes,sizeof(statuMessage.bytes));
}

//对文件的标签进行操作
void SPI_SRAM_PUF_FILE_LABEL(uint32_t *Files,int len,uint32_t *helpData){
			//定义文件处理的相关数组
		uint32_t randoms[21];   // 直接读取到 randoms
    uint32_t sramData[32];  // 直接读取到 sramData
    uint32_t codeData[32];	//存储编码后的数组
    int new_length;

    // 直接将数据读取到最终需要的数组中
    sram_read_random(0, (uint16_t*)sramData, sizeof(sramData)/sizeof(uint16_t), DELAY_TIME);
    sram_read_random(100, (uint16_t*)randoms, sizeof(randoms)/sizeof(uint16_t), DELAY_TIME);
	
    int n = sizeof(randoms) / sizeof(randoms[0]);
		for(int i = 0 ; i<4;i++){
			randoms[i] ^= *(Files+i);
		}
    bch_encoder(randoms, n, &new_length, codeData);
		
    for (int i = 0; i < 32; i++) {
        *(helpData+i) = codeData[i] ^ sramData[i];
				//statuMessage.data.key[i] = sramData[i];
				//printf("%08x",sramData[i]);
    }
		
}
