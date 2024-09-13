#include "bch.h"

uint32_t bch_encode(const uint32_t cw)
{
	uint32_t bit = 0;
	uint32_t parity = 0;
	uint32_t local_cw = cw & 0xFFFFF800; // mask off BCH parity and even parity
	uint32_t cw_e = local_cw;

	// Calculate BCH bits
	for (bit = 1; bit <= 21; bit++)
	{
		if (cw_e & 0x80000000)
		{
			cw_e ^= 0xED200000;
		}
		cw_e <<= 1;
	}
	local_cw |= (cw_e >> 21);

	// At this point local_cw contains a codeword with BCH but no parity

	// Calculate parity bit
	cw_e = local_cw;
	bit = 32;
	while ((cw_e != 0) && (bit > 0))
	{
		if (cw_e & 1)
		{
			parity++;
		}
		cw_e >>= 1;
		bit--;
	}

	// apply parity bit
	return (parity % 2) ? (local_cw | 1) : local_cw;
}

// Debug options for error correction
// -- Enable debug
//#define BCH_REPAIR_DEBUG
// -- Enable printing the output of the ECC process step-by-step
//#define BCH_REPAIR_DEBUG_STEPBYSTEP

int bch_repair(const uint32_t cw, uint32_t *repaired_cw)
{
	// calculate syndrome
	// We do this by recalculating the BCH parity bits and XORing them against the received ones

	// mask off data bits and parity, leaving the error syndrome in the LSB
	uint32_t syndrome = ((bch_encode(cw) ^ cw) >> 1) & 0x3FF;
	uint32_t parity_bit = (bch_encode(cw) ^ cw) & 1; // 获取奇偶校验位
	if (syndrome == 0)
	{
		// Syndrome of zero indicates no repair required
		*repaired_cw = cw;
		return 0;
	}

#ifdef BCH_REPAIR_DEBUG
	printf("cw:%08X  syndrome:%08X\n", cw, syndrome);
#endif

	// --- Meggitt decoder ---

	uint32_t result = 0;
	uint32_t damaged_cw = cw;
	uint32_t error_count = 0; //计算出错的次数
	// Calculate BCH bits
	for (uint32_t xbit = 0; xbit < 31; xbit++)
	{

#ifdef BCH_REPAIR_DEBUG_STEPBYSTEP
		printf("    xbit:%2d  synd:%08X  dcw:%08X  fixed:%08X", xbit, syndrome, damaged_cw, result);
#endif

		// produce the next corrected bit in the high bit of the result
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
			// Syndrome matches an error in the MSB
			// Correct that error and adjust the syndrome to account for it
			syndrome ^= 0x3B4;

			result |= (~damaged_cw & 0x80000000) >> 30;

			error_count++; //统计出错的次数
						   // 根据奇偶校验位判断是否需要提前停止
			if ((parity_bit == 1 && error_count >= 2) || (parity_bit == 0 && error_count >= 3))
			{
				*repaired_cw = cw;
				return -1;
			}

#ifdef BCH_REPAIR_DEBUG_STEPBYSTEP
			printf("  E\n"); // indicate that an error was corrected in this bit
#endif
		}
		else
		{
			// no error
			result |= (damaged_cw & 0x80000000) >> 30;

#ifdef BCH_REPAIR_DEBUG_STEPBYSTEP
			printf("   \n");
#endif
		}
		damaged_cw <<= 1;

		// Handle Syndrome shift register feedback
		if (syndrome & 0x200)
		{
			syndrome <<= 1;
			syndrome ^= 0x769; // 0x769 = POCSAG generator polynomial -- x^10 + x^9 + x^8 + x^6 + x^5 + x^3 + 1
		}
		else
		{
			syndrome <<= 1;
		}
		// mask off bits which fall off the end of the syndrome shift register
		syndrome &= 0x3FF;

		// XXX Possible optimisation: Can we exit early if the syndrome is zero? (no more errors to correct)
	}

#ifdef BCH_REPAIR_DEBUG
	printf("  orig:%08X  fixed:%08X  %s\n",
		   cw,							/* original codeword */
		   result,						/* corrected codeword sans parity bit */
		   syndrome == 0 ? "OK" : "ERR" /* syndrome == 0 if error was corrected */
	);
#endif

	// Check if error correction was successful
	if (syndrome != 0)
	{
		// Syndrome nonzero at end indicates uncorrectable errors
#ifdef BCH_REPAIR_DEBUG
		printf("nonzero syndrome at end\n");
#endif
		*repaired_cw = cw;
		return -1;
	}

	// Syndrome is zero -- that means we must have succeeded!
	*repaired_cw = result;
	return 0;
}

void bch_encoder(const uint32_t *cw, int n, int *new_size,uint32_t* encoded)
{
	int bit_count = n * 32;
	int out_length = (bit_count + 20) / 21; // �����µ����鳤��
	*new_size = out_length;

	/*// �����µ�����
	uint32_t *encoded = (uint32_t *)malloc(out_length * sizeof(uint32_t));
	if (!encoded) {
	  return NULL; // �����ڴ�ʧ��
	}*/
	//uint32_t encoded[49];

	int bit_index = 0; // ��ǰ�����ı���λ��
	for (int i = 0; i < out_length; ++i)
	{
		uint32_t temp = 0;
		for (int j = 0; j < 21; ++j)
		{
			int word_index = bit_index / 32;  // ��Ӧ�� cw ���������
			int bit_in_word = bit_index % 32; // ��Ӧ�� bit �� word �е�λ��
			if (bit_index < bit_count)
			{ // ������Ч���ط�Χ�ڽ��в���
				temp |= ((cw[word_index] >> (31 - bit_in_word)) & 1) << (20 - j);
			}
			bit_index++;
		}
		// encoded[i] = temp << 11; // �� 21 λ�洢���� 11 λȡ 0
		*(encoded+i) = bch_encode(temp << 11);
	}
	return ;
}

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
	// ����ʣ��� bits
	if (re_j > 0)
	{
		repaired_cw[re_i] = temp;
	}
}
