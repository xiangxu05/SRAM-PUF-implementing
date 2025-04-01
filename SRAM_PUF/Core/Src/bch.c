#include "bch.h"
/**
 * @brief BCH编码函数
 * 
 * 该函数计算输入数据的BCH校验位，并将其附加到数据中。
 * 
 * @param cw 输入数据（32位无符号整数）
 * @return uint32_t 返回编码后的数据（包含BCH校验位和奇偶校验位）
 */
uint32_t bch_encode(const uint32_t cw)
{
	uint32_t bit = 0; // 用于循环计数
	uint32_t parity = 0; // 用于计算奇偶校验位
	uint32_t local_cw = cw & 0xFFFFF800; // 掩码操作，保留数据位，清除BCH校验位和奇偶校验位
	uint32_t cw_e = local_cw;// 临时变量，用于计算BCH校验位

	// Calculate BCH bits
	for (bit = 1; bit <= 21; bit++)
	{
		if (cw_e & 0x80000000)// 检查最高位是否为1
		{
			cw_e ^= 0xED200000; // 执行多项式除法（异或操作）
		}
		cw_e <<= 1;// 左移一位，继续计算下一个位
	}
	local_cw |= (cw_e >> 21);// 将计算得到的BCH校验位附加到数据中

  // 此时local_cw包含BCH校验位，但不包含奇偶校验位

  // 计算奇偶校验位
	cw_e = local_cw;// 重新使用cw_e变量
	bit = 32;// 初始化循环计数
	while ((cw_e != 0) && (bit > 0))
	{
		if (cw_e & 1)// 检查最低位是否为1
		{
			parity++;// 累加奇偶校验计数
		}
		cw_e >>= 1;// 右移一位，继续检查下一个位
		bit--;
	}

	// apply parity bit
	return (parity % 2) ? (local_cw | 1) : local_cw;// 根据奇偶校验结果设置最低位
}

// 调试选项（可选）
// -- 启用调试
//#define BCH_REPAIR_DEBUG
// -- 启用逐步打印ECC处理过程
//#define BCH_REPAIR_DEBUG_STEPBYSTEP

/**
 * @brief BCH纠错函数
 * 
 * 该函数尝试修复输入数据中的错误，并返回修复后的数据。
 * 
 * @param cw 输入数据（32位无符号整数）
 * @param repaired_cw 输出修复后的数据
 * @return int 返回0表示修复成功，-1表示无法修复
 */
int bch_repair(const uint32_t cw, uint32_t *repaired_cw)
{
  // 计算syndrome
  // 通过重新计算BCH校验位并与输入数据的校验位异或，得到syndrome
	uint32_t syndrome = ((bch_encode(cw) ^ cw) >> 1) & 0x3FF;
	uint32_t parity_bit = (bch_encode(cw) ^ cw) & 1; // 获取奇偶校验位
	if (syndrome == 0)
	{
		// syndrome为0表示没有错误
		*repaired_cw = cw;
		return 0;
	}

#ifdef BCH_REPAIR_DEBUG
	printf("cw:%08X  syndrome:%08X\n", cw, syndrome);// 调试输出
#endif

	// --- Meggitt解码器 ---

	uint32_t result = 0;// 用于存储修复后的数据
	uint32_t damaged_cw = cw;// 输入数据的副本，用于修复过程
	uint32_t error_count = 0; //计算出错的次数
	// Calculate BCH bits
	for (uint32_t xbit = 0; xbit < 31; xbit++)
	{

#ifdef BCH_REPAIR_DEBUG_STEPBYSTEP
		printf("    xbit:%2d  synd:%08X  dcw:%08X  fixed:%08X", xbit, syndrome, damaged_cw, result);
#endif

		// 生成下一个校正位并存储在result的最高位
		result <<= 1;
		if ((syndrome == 0x3B4) || // 0x3B4: Syndrome when a single error is detected in the MSB
			(syndrome == 0x26E) || // 0x26E: Two adjacent errors
			(syndrome == 0x359) || // 0x359: Two errors, one OK bit between
			(syndrome == 0x076) || // 0x076: Two errors, two OK bits between
			(syndrome == 0x255) || // 0x255: Two errors, three OK bits between
			(syndrome == 0x0F0) || // 0x0F0: Two errors, four OK bits between
			(syndrome == 0x216) || // ... and so on
			(syndrome == 0x365) ||
			(syndrome == 0x068) ||
			(syndrome == 0x25A) ||
			(syndrome == 0x343) ||
			(syndrome == 0x07B) ||
			(syndrome == 0x1E7) ||
			(syndrome == 0x129) ||
			(syndrome == 0x14E) ||
			(syndrome == 0x2C9) ||
			(syndrome == 0x0BE) ||
			(syndrome == 0x231) ||
			(syndrome == 0x0C2) ||
			(syndrome == 0x20F) ||
			(syndrome == 0x0DD) ||
			(syndrome == 0x1B4) ||
			(syndrome == 0x2B4) ||
			(syndrome == 0x334) ||
			(syndrome == 0x3F4) ||
			(syndrome == 0x394) ||
			(syndrome == 0x3A4) ||
			(syndrome == 0x3BC) ||
			(syndrome == 0x3B0) ||
			(syndrome == 0x3B6) ||
			(syndrome == 0x3B5))
		{
			// syndrome匹配错误模式，纠正错误并调整syndrome
			syndrome ^= 0x3B4;

			result |= (~damaged_cw & 0x80000000) >> 30; // 纠正错误位

			error_count++; //统计出错的次数
			// 根据奇偶校验位判断是否需要提前停止
			if ((parity_bit == 1 && error_count >= 2) || (parity_bit == 0 && error_count >= 3))
			{
				*repaired_cw = cw;// 无法修复
				return -1;
			}

#ifdef BCH_REPAIR_DEBUG_STEPBYSTEP
			printf("  E\n"); // 表示纠正了一个错误
#endif
		}
		else
		{
			// 没有错误
			result |= (damaged_cw & 0x80000000) >> 30;

#ifdef BCH_REPAIR_DEBUG_STEPBYSTEP
			printf("   \n");
#endif
		}
		damaged_cw <<= 1;

		// 处理syndrome移位寄存器的反馈
		if (syndrome & 0x200)
		{
			syndrome <<= 1;
			syndrome ^= 0x769; // 0x769 = POCSAG生成多项式（x^10 + x^9 + x^8 + x^6 + x^5 + x^3 + 1）
		}
		else
		{
			syndrome <<= 1;
		}
		// 掩码操作，清除超出syndrome寄存器范围的位
		syndrome &= 0x3FF;

		// 可能的优化：如果syndrome为0，可以提前退出（没有更多错误需要纠正）
	}

#ifdef BCH_REPAIR_DEBUG
	printf("  orig:%08X  fixed:%08X  %s\n",
		   cw,							// 原始数据
		   result,						// 修复后的数据（不包含奇偶校验位）
		   syndrome == 0 ? "OK" : "ERR" // syndrome为0表示修复成功
	);
#endif

	// 检查纠错是否成功
	if (syndrome != 0)
	{
		// syndrome非零表示无法修复
#ifdef BCH_REPAIR_DEBUG
		printf("nonzero syndrome at end\n");
#endif
		*repaired_cw = cw;
		return -1;
	}

	// 综合症为0，修复成功
	*repaired_cw = result;
	return 0;
}

/**
 * @brief BCH编码器
 * 
 * 该函数将输入数据分块编码为BCH码。
 * 
 * @param cw 输入数据指针
 * @param n 输入数据的字数
 * @param new_size 输出编码后的字数
 * @param encoded 输出编码后的数据
 */
void bch_encoder(const uint32_t *cw, int n, int *new_size,uint32_t* encoded)
{
	int bit_count = n * 32;
	int out_length = (bit_count + 20) / 21; // 计算编码后需要的字数（每21位一个字）
	*new_size = out_length;

	int bit_index = 0; // 当前处理的位索引
	for (int i = 0; i < out_length; ++i)
	{
		uint32_t temp = 0;
		for (int j = 0; j < 21; ++j)
		{
			int word_index = bit_index / 32;  // 当前位所在的字索引
			int bit_in_word = bit_index % 32; // 当前位在字中的位置
			if (bit_index < bit_count)
			{ // 如果位索引在有效范围内
				temp |= ((cw[word_index] >> (31 - bit_in_word)) & 1) << (20 - j);
			}
			bit_index++;
		}
		// encoded[i] = temp << 11; // 对21位数据进行BCH编码
		*(encoded+i) = bch_encode(temp << 11);
	}
	return ;
}

/**
 * @brief BCH解码器
 * 
 * 该函数将编码后的数据解码为原始数据，并尝试修复错误。
 * 
 * @param cw 输入编码后的数据指针
 * @param n 输入数据的字数
 * @param out_length 输出解码后的字数
 * @param repaired_cw 输出解码后的数据
 */
void bch_decoder(const uint32_t *cw, int n, int *out_length, uint32_t *repaired_cw)
{
	uint32_t decoded_cw[32];

	for (int i = 0; i < n; i++)
	{
		*out_length = bch_repair(cw[i], &decoded_cw[i]);
		if (*out_length == -1)
			return;
	}

	int bit_count = n * 21;
	*out_length = (bit_count + 31) / 32;

	uint32_t temp = 0;
	int re_i = 0, re_j = 0;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < 21; ++j)
		{
			temp |= ((decoded_cw[i] >> (31 - j)) & 1) << (31 - re_j);
			re_j++;
			if (re_j > 31)
			{
				repaired_cw[re_i] = temp;
				re_j = 0;
				re_i++;
				temp = 0;
			}
		}
	}
	// 处理剩余的位
	if (re_j > 0)
	{
		repaired_cw[re_i] = temp;
	}
}
