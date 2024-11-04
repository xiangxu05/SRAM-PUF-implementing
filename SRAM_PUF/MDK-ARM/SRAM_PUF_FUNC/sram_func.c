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

void SPI_SRAM_PUF_STRONG(unsigned char* Messages,int len){//Strong PUF模式
		SHA512_CTX ctx;
		uint8_t hash[SHA512_BLOCK_SIZE];
		uint32_t hash1[16];
		// 计算前512位的哈希值
		sha512_init(&ctx);
		sha512_update(&ctx, Messages, len);
		sha512_final(&ctx, hash);
		uint32_t data[32];
		
		SPI_SRAM_PUF_Stable_Output(data);  // 读稳定输出
		printf("Response:\n");
		for (int i = 0; i < 16; i++) {
			hash1[i] = ((uint32_t)hash[4*i] << 24) | ((uint32_t)hash[4*i+1] << 16) |
			((uint32_t)hash[4*i+2] << 8) | (uint32_t)hash[4*i+3];
		}
		for (int i = 0; i < 32; i++) {
			printf("%08x", data[i]^hash1[i%16]);
		}
}

void SPI_SRAM_PUF_Stable_Output(uint32_t *Data){//读取SRAM PUF稳定输出
	uint32_t current_sramData[32]; //读内存单元值
	uint32_t helpData[32];//取helpData值
	uint32_t xorData[32];
	uint32_t codeData[32];
	
	int new_length = -1;
	int count = 0;
	while(new_length == -1 && count < 4){
		sram_read_random(0, (uint8_t*)current_sramData, sizeof(current_sramData)/sizeof(uint8_t), DELAY_TIME);//读新的sram值
		W25_Flash_Read(0,(uint8_t *)helpData , 128);//读helpData
	
		for(int i=0;i<32;i++){
				xorData[i]=helpData[i] ^ current_sramData[i];//可能存在错误的随机值
		}
		bch_decoder(xorData,32,&new_length,codeData);//纠正后的随机值
		bch_encoder(codeData,32,&new_length,xorData);//重新编码存在xorData中
		for (int i = 0; i < 32; i++) {
        Data[i] = helpData[i] ^ xorData[i];
		}
		if(new_length!=-1){
			return;
		}
		count++;
	}
	printf("error: Decode fail!");
	
}

void SPI_SRAM_PUF_FILE(unsigned char* Infos,int len){//文件管理模式	
		if(len<32){
			printf("Invalid inputs!");
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
	
			sram_read_random(0, (uint8_t*)sramData, sizeof(sramData)/sizeof(uint8_t), DELAY_TIME);//读新的sram值
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
	
					sram_read_random(0, (uint8_t*)sramData, sizeof(sramData)/sizeof(uint8_t), DELAY_TIME);//读新的sram值
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
	
				sram_read_random(0, (uint8_t*)sramData, sizeof(sramData)/sizeof(uint8_t), DELAY_TIME);//读新的sram值
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

void SPI_SRAM_PUF_FILE_LABEL(uint32_t *Files,int len,uint32_t *helpData){//对文件的标签进行操作
		//定义文件处理的相关数组
		uint32_t randoms[21];   // 直接读取到 randoms
    uint32_t sramData[32];  // 直接读取到 sramData
    uint32_t codeData[32];	//存储编码后的数组
    int new_length;

    // 直接将数据读取到最终需要的数组中
    sram_read_random(0, (uint8_t*)sramData, sizeof(sramData)/sizeof(uint8_t), DELAY_TIME);
    sram_read_random(100, (uint8_t*)randoms, sizeof(randoms)/sizeof(uint8_t), DELAY_TIME);
	
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