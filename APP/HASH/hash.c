#include "hash.h"

// ��ʼ��HASH������
void HASH_Config(void) {
    // ����HASH��������ʱ��
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_HASH, ENABLE);
    
    // ����HASH������
    HASH_DeInit();
}

// ����SHA-256��ϣֵ
void HASH_SHA256(uint8_t *input, uint32_t length, uint8_t *output) {
    HASH_InitTypeDef HASH_InitStructure;
    uint32_t msgDigest[8];

    // ��ʼ��HASH������
    HASH_Config();
    
    // ����HASH������
    HASH_InitStructure.HASH_AlgoSelection = HASH_AlgoSelection_SHA256;
    HASH_InitStructure.HASH_AlgoMode = HASH_AlgoMode_HASH;
    HASH_InitStructure.HASH_DataType = HASH_DataType_8b;
    HASH_Init(&HASH_InitStructure);
    
    // ����HASH������
    HASH_Reset();
    
    // ����HASH������
    HASH_StartDigest();
    
    // ���������뵽HASH������
    for (uint32_t i = 0; i < length; i++) {
        HASH_DataIn(input[i]);
    }
    
    // �ȴ�HASH�������
    while (HASH_GetFlagStatus(HASH_FLAG_BUSY) == SET);
    
    // ��ȡHASH���
    HASH_GetDigest((HASH_MsgDigest*)msgDigest);

    // ������洢�����������
    for (uint32_t i = 0; i < 8; i++) {  // SHA-256 produces 8 32-bit words
        ((uint32_t*)output)[i] = __REV(msgDigest[i]);
    }
}