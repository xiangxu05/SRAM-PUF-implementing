#ifndef BCH_ENCODER_H
#define BCH_ENCODER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "system.h"

#define MAX_G 32

typedef struct {
    int N;
    int K;
    int NP;
    int G;
    uint8_t generator[MAX_G];
} BCH_Encoder;

uint8_t slb1(uint8_t *buf, int pos);

void BCH_Encoder_Init(BCH_Encoder *encoder, int N, int K, const int *minimal_polynomials, int num_polynomials);
void BCH_Encoder_Encode(const BCH_Encoder *encoder, const uint8_t *data, uint8_t *parity, int data_len);

#endif // BCH_ENCODER_H
