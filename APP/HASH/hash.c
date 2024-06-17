#include "hash.h"

// 初始化HASH处理器
void HASH_Config(void) {
    // 启用HASH处理器的时钟
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_HASH, ENABLE);
    
    // 重置HASH处理器
    HASH_DeInit();
}

// 计算SHA-256哈希值
void HASH_SHA256(uint8_t *input, uint32_t length, uint8_t *output) {
    HASH_InitTypeDef HASH_InitStructure;
    uint32_t msgDigest[8];

    // 初始化HASH处理器
    HASH_Config();
    
    // 配置HASH处理器
    HASH_InitStructure.HASH_AlgoSelection = HASH_AlgoSelection_SHA256;
    HASH_InitStructure.HASH_AlgoMode = HASH_AlgoMode_HASH;
    HASH_InitStructure.HASH_DataType = HASH_DataType_8b;
    HASH_Init(&HASH_InitStructure);
    
    // 重置HASH处理器
    HASH_Reset();
    
    // 启动HASH处理器
    HASH_StartDigest();
    
    // 将数据输入到HASH处理器
    for (uint32_t i = 0; i < length; i++) {
        HASH_DataIn(input[i]);
    }
    
    // 等待HASH处理完成
    while (HASH_GetFlagStatus(HASH_FLAG_BUSY) == SET);
    
    // 读取HASH结果
    HASH_GetDigest((HASH_MsgDigest*)msgDigest);

    // 将结果存储到输出缓冲区
    for (uint32_t i = 0; i < 8; i++) {  // SHA-256 produces 8 32-bit words
        ((uint32_t*)output)[i] = __REV(msgDigest[i]);
    }
}