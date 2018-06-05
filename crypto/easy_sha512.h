/*
 * Copyright (c) 2018, Jiamin Ma
 * BSD License
 */
#ifndef EASY_SHA512_H
#define EASY_SHA512_H

#include "easy_crypto.h"

#ifdef CRYPTO_DEBUG_SUPPORT
#define SHA512_DEBUG printf
#else
#define SHA512_DEBUG(fmt, ...)
#endif

/**
 * @brief   Covert the uint64_t to big endian byte array.
 * @param   input       input uint64_t data
 * @param   output      output big endian byte array
 * @param   idx         idx of the byte array.
 * @retval  void
 */
static void inline sha512_encode(uint64_t input, uint8_t *output, uint32_t idx)
{
    output[idx + 0] = (uint8_t)(input >> 56);
    output[idx + 1] = (uint8_t)(input >> 48);
    output[idx + 2] = (uint8_t)(input >> 40);
    output[idx + 3] = (uint8_t)(input >> 32);
    output[idx + 4] = (uint8_t)(input >> 24);
    output[idx + 5] = (uint8_t)(input >> 16);
    output[idx + 6] = (uint8_t)(input >>  8);
    output[idx + 7] = (uint8_t)(input >>  0);
}

/**
 * @brief   Covert big endian byte array to uint64_t data
 * @param   output      output uint64_t data
 * @param   input       input big endian byte array
 * @param   idx         idx of the byte array.
 * @retval  void
 */
static inline void sha512_decode(uint64_t *output, uint8_t *input, uint32_t idx)
{
    *output = ((uint64_t)input[idx + 0] << 56)
            | ((uint64_t)input[idx + 1] << 48)
            | ((uint64_t)input[idx + 2] << 40)
            | ((uint64_t)input[idx + 3] << 32)
            | ((uint64_t)input[idx + 4] << 24)
            | ((uint64_t)input[idx + 5] << 16)
            | ((uint64_t)input[idx + 6] <<  8)
            | ((uint64_t)input[idx + 7] <<  0);
}

typedef struct sha512_ctx_tag {

    uint32_t is_sha384;
    /*SHA512 process the data by one block:1024 bits*/
    uint8_t block[128];
    /*SHA512 will fill 128 bits length field: unit:bit*/
    uint64_t len[2];
    /*Hash values*/
    uint64_t val[8];
    /*Payload address to hash*/
    uint8_t *payload_addr;
    /*Payload length*/
    uint64_t payload_len;
} sha512_ctx_t;


#define LSR(x,n) (x >> n)
#define ROR(x,n) (LSR(x,n) | (x << (64 - n)))

#define MA(x,y,z) ((x & y) | (z & (x | y)))
#define CH(x,y,z) (z ^ (x & (y ^ z)))
#define GAMMA0(x) (ROR(x, 1) ^ ROR(x, 8) ^  LSR(x, 7))
#define GAMMA1(x) (ROR(x,19) ^ ROR(x,61) ^  LSR(x, 6))
#define SIGMA0(x) (ROR(x,28) ^ ROR(x,34) ^ ROR(x,39))
#define SIGMA1(x) (ROR(x,14) ^ ROR(x,18) ^ ROR(x,41))

#define INIT_COMPRESSOR() uint64_t tmp0 = 0, tmp1 = 0
#define COMPRESS( a,  b,  c, d,  e,  f,  g,  h, x,  k)   \
    tmp0 = h + SIGMA1(e) + CH(e,f,g) + k + x;              \
    tmp1 = SIGMA0(a) + MA(a,b,c); d += tmp0; h = tmp0 + tmp1;

#endif /*EASY_SHA512_H*/
