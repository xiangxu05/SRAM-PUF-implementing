#include "encoder.h"
#include "bitman.h"

uint8_t slb1(uint8_t *buf, int pos) {
    return (buf[pos] << 1) | (buf[pos + 1] >> 7);
}

void BCH_Encoder_Init(BCH_Encoder *encoder, int N, int K, const int *minimal_polynomials, int num_polynomials) {
    int i,p,j;
		int generator_degree = 1;
		int m_degree;
		encoder->N = N;
    encoder->K = K;
    encoder->NP = N - K;
    encoder->G = ((encoder->NP + 1) + 7) / 8;

    for (i = 0; i < encoder->G; ++i) {
        encoder->generator[i] = 0;
    }

    set_be_bit(encoder->generator, encoder->NP, 1);

    for (p = 0; p < num_polynomials; ++p) {
        int m = minimal_polynomials[p];
        //assert(0 < m);
				/*if(0<m){
					printf("error:0<m");
					return;
				}*/
				
        m_degree = 0;
        while (m >> m_degree) {
            ++m_degree;
        }
        --m_degree;
				
        //assert(generator_degree + m_degree <= encoder->NP + 1);
				/*if(generator_degree + m_degree <= encoder->NP + 1){
					printf("error:generator_degree + m_degree <= encoder->NP + 1");
					return;
				}*/
				
        for (i = generator_degree; i >= 0; --i) {
            if (!get_be_bit(encoder->generator, encoder->NP - i)) {
                continue;
            }
            set_be_bit(encoder->generator, encoder->NP - i, m & 1);
            for (j = 1; j <= m_degree; ++j) {
                xor_be_bit(encoder->generator, encoder->NP - (i + j), (m >> j) & 1);
            }
        }
        generator_degree += m_degree;
    }
		
    //assert(generator_degree == encoder->NP + 1);
		/*if(generator_degree == encoder->NP + 1){
			printf("error:generator_degree == encoder->NP + 1");
			return;
		}*/
    if (0) {
        fprintf(stderr, "generator =");
        for (i = 0; i <= encoder->NP; ++i) {
            fprintf(stderr, " %d", get_be_bit(encoder->generator, encoder->NP - i));
        }
        fprintf(stderr, "\n");
    }

    for (i = 0; i < encoder->NP; ++i) {
        set_be_bit(encoder->generator, i, get_be_bit(encoder->generator, i + 1));
    }
    set_be_bit(encoder->generator, encoder->NP, 0);
}

void BCH_Encoder_Encode(const BCH_Encoder *encoder, const uint8_t *data, uint8_t *parity, int data_len) {
		int l,i;
		
		//assert(0 < data_len && data_len <= encoder->K);
		/*if(0 < data_len && data_len <= encoder->K){
			printf("error:0 < data_len && data_len <= encoder->K");
			return;
		}*/
		
    for (l = 0; l <= (encoder->NP - 1) / 8; ++l) {
        parity[l] = 0;
    }

    for (i = 0; i < data_len; ++i) {
        if (get_be_bit(data, i) != get_be_bit(parity, 0)) {
            for (l = 0; l < (encoder->NP - 1) / 8; ++l) {
                parity[l] = encoder->generator[l] ^ slb1(parity, l);
            }
            parity[(encoder->NP - 1) / 8] = encoder->generator[(encoder->NP - 1) / 8] ^ (parity[(encoder->NP - 1) / 8] << 1);
        } else {
            for (l = 0; l < (encoder->NP - 1) / 8; ++l) {
                parity[l] = slb1(parity, l);
            }
            parity[(encoder->NP - 1) / 8] <<= 1;
        }
    }
}
