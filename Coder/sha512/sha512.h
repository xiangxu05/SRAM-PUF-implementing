#ifndef _SHA512_H
#define _SHA512_H

#include <stdint.h>
#include <stddef.h>

#define SHA512_BLOCK_SIZE 64  // SHA512 outputs a 64 byte digest

typedef struct {
    uint8_t data[128];
    uint32_t datalen;
    uint64_t bitlen[2];
    uint64_t state[8];
} SHA512_CTX;

void sha512_init(SHA512_CTX *ctx);
void sha512_update(SHA512_CTX *ctx, const uint8_t data[], size_t len);
void sha512_final(SHA512_CTX *ctx, uint8_t hash[]);
void sha1024(uint8_t *input, uint32_t length, uint8_t *output);
#endif
