#ifndef SHA512_H
#define SHA512_H

#include "../utils/types.h"

#define SHA512_BLOCK_SIZE 128
#define SHA512_DIGEST_SIZE 64

typedef struct {
    uint64_t state[8];
    uint64_t count[2];
    uint8_t buffer[SHA512_BLOCK_SIZE];
} sha512_ctx;

void sha512_init(sha512_ctx* ctx);
void sha512_update(sha512_ctx* ctx, const uint8_t* data, size_t len);
void sha512_final(sha512_ctx* ctx, uint8_t digest[SHA512_DIGEST_SIZE]);

// Triple SHA-512 Stream Cipher
// Generates a keystream block of 64 bytes (SHA512 digest size)
// key: 128-bit key (16 bytes)
// iv: 64-bit IV (8 bytes)
// counter: 64-bit counter
// output: 64-byte keystream block
void triple_sha512_sc_block(const uint8_t* key, const uint8_t* iv, uint64_t counter, uint8_t* output);

#endif
