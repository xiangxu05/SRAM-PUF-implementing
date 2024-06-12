#include "system.h"
#include "usart.h"
#include "bch.h"

// 模拟一个简单的测试断言功能
void assert_equal(uint32_t expected, uint32_t actual, const char *message);

// 测试BCH编码的完整性
void test_BCH_sanity(uint32_t *TEST_CWS);

// 测试BCH单比特错误修复
void test_BCH_single_bit_errors(uint32_t *TEST_CWS);
