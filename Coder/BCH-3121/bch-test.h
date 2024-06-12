#include "system.h"
#include "usart.h"
#include "bch.h"

// ģ��һ���򵥵Ĳ��Զ��Թ���
void assert_equal(uint32_t expected, uint32_t actual, const char *message);

// ����BCH�����������
void test_BCH_sanity(uint32_t *TEST_CWS);

// ����BCH�����ش����޸�
void test_BCH_single_bit_errors(uint32_t *TEST_CWS);
