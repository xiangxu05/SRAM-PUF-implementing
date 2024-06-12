#include "bch-test.h"

// 模拟一个简单的测试断言功能
void assert_equal(uint32_t expected, uint32_t actual, const char *message) {
    if (expected != actual) {
        printf("FAIL: %s Expected: 0x%08X, Got: 0x%08X\n", message, expected, actual);
    }
    else {
        printf("PASS: %s\n", message);
    }
}

// 测试BCH编码的完整性
void test_BCH_sanity(uint32_t *TEST_CWS) {
    for (size_t n = 0; n < sizeof(TEST_CWS) / sizeof(TEST_CWS[0]); n++) {
        uint32_t encoded = bch_encode(TEST_CWS[n]);
        char message[100];
        snprintf(message, sizeof(message), "BCH encoding sanity check for index %zu", n);
        assert_equal(TEST_CWS[n], encoded, message);
    }
}

// 测试BCH单比特错误修复
void test_BCH_single_bit_errors(uint32_t *TEST_CWS) {
    for (size_t n = 0; n < sizeof(TEST_CWS) / sizeof(TEST_CWS[0]); n++) {
        uint32_t OriginalCW = TEST_CWS[n];
        uint32_t mask = 0x80000000;

        while (mask > 0) {
            uint32_t damaged_cw = OriginalCW ^ mask;
            uint32_t repaired_cw;
            int result = bch_repair(damaged_cw, &repaired_cw);
            char message[100];
            snprintf(message, sizeof(message), "BCH single-bit error repair at mask 0x%08X", mask);
            if (result == 0) {
                assert_equal(OriginalCW & 0xFFFFFFFE, repaired_cw & 0xFFFFFFFE, message);
            }
            else {
                printf("FAIL: %s Repair failed.\n", message);
            }
            mask >>= 1;
        }
    }
}
