#include "sram_func.h"

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
    printf("Starting Initialize steps");

    // 直接将数据读取到最终需要的数组中
    sram_read_random(0, (uint8_t*)sramData, sizeof(sramData)/sizeof(uint8_t), DELAY_TIME);
    sram_read_random(100, (uint8_t*)randoms, sizeof(randoms)/sizeof(uint8_t), DELAY_TIME);
	
		//编码与解码（解码目的是确定编码成功）
    int n = sizeof(randoms) / sizeof(randoms[0]);
    bch_encoder(randoms, n, &new_length, codeData);
    bch_decoder(codeData, new_length, &new_length, new_randoms);
	
		//验证编码成功
    if (randoms[0] == new_randoms[0]) {
        printf("Initialize program success!\n");
    }else{
			printf("Fail to initialize try again!\n");
			return;
		}
		
		//组成helpdata，并输出
    printf("Get help data:\n");
    for (int i = 0; i < 32; i++) {
        xorData[i] = codeData[i] ^ sramData[i];
        sprintf(message + strlen(message), "%08x", xorData[i]);
    }
    usb_send((unsigned char*)message, strlen(message));
		
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
        printf("Successful store helpData!");
    } else {
        printf("Failure occurred: store program failed!");
    }
}