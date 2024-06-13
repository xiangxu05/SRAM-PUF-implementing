/**
 * @title BCH(31,21) error correction (POCSAG algorithm).
 * @author Phil Pemberton
 *
 * This module performs BCH(31,21) error correction coding and decoding.
 * This is the same algorithm used by e.g. POCSAG. This BCH code can
 * correct up to two erroneous bits, and detect (but not correct) up to
 * three.
 */

#ifndef BCH_H
#define BCH_H

#ifdef __cplusplus


extern "C" {
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "system.h"
/**
 * Calculate the BCH(31,21) and even parity bits for a given codeword.
 *
 * The codeword contains the data bits in the most significant 21 bits.
 * The least significant 11 bits are filled in by this function.
 * This is the same as the POCSAG format, with MSB transmitted first.
 *
 * From MSB to LSB, the format is:
 *
 * <ul>
 *   <li>21 data bits</li>
 *   <li>10 BCH error correction bits</li>
 *   <li>1-bit even parity</li>
 * </ul>
 *
 * @param cw	Codeword. Only the data bits need be filled in.
 * @return		Codeword with BCH parity and even parity applied.
 */
uint32_t bch_encode(const uint32_t cw);

/**
 * BCH(31,21) error correction (Meggitt type decoder).
 *
 * Accepts a codeword in <pre>cw</pre> and attempts to correct any errors.
 * The parity bit is ignored. POCSAG uses a BCH(31,21) code which can correct
 * up to two erroneous bits.
 *
 * BCH error correction is applied if needed and the corrected codeword is
 * output in <pre>repaired_cw</pre>. If the codeword is uncorrectable,
 * <pre>repaired_cw</pre> will be equal to <pre>cw</pre>.
 *
 * @param	cw				Input codeword
 * @param	repaired_cw		Repaired output codeword
 * @returns 0 for OK (no correction needed or ECC succeeded). <br>
 * 			-1 if the CW is uncorrectable.
 */
int bch_repair(const uint32_t cw, uint32_t *repaired_cw);

/**
 * 计算一组u32的BCH(31,21)，每组均包括一位奇偶校验
 *
 * @param cw	Codewords，需要编码的值，方法中将code重新组织为21位一组，不足的末尾补零。
 * @param n 	共有n组。
 * @return		返回一组编码后的值。
 */
u32* bch_encoder(const u32 *cw, int n, int *new_size);

/**
 * 纠错一组u32的BCH(31,21)
 *
 * @param cw	Codewords，需要解码的值。
 * @param out_length 结果数组长度。
 * @param n 	共有n组。
 * @return		NULL if the CW is uncorrectable.
 *						or repaired_cw.
 */
void bch_decoder(const u32 *cw, int n, int *out_length, u32 *repaired_cw);

#ifdef __cplusplus
}
#endif

#endif // BCH_H
