#ifndef SENTINEL_SUPPORT

#include "easy_sha512.h"

extern crypto_status_t easy_sha512_impl(UINT8 *payload, UINT64 payload_len, UINT8 output[64], UINT32 is_sha384);

/*
 * Predefined sha512 padding bytes
 */
static const UINT8 sha512_padding[128] =
{
    0x80u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
    0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
    0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
    0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
    0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
    0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
    0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u,
    0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u
};

/*
 * K byte array used for iteration
 */
static const UINT64 K[80] =
{
    0x428A2F98D728AE22ULL,  0x7137449123EF65CDULL, 0xB5C0FBCFEC4D3B2FULL,  0xE9B5DBA58189DBBCULL,
    0x3956C25BF348B538ULL,  0x59F111F1B605D019ULL, 0x923F82A4AF194F9BULL,  0xAB1C5ED5DA6D8118ULL,
    0xD807AA98A3030242ULL,  0x12835B0145706FBEULL, 0x243185BE4EE4B28CULL,  0x550C7DC3D5FFB4E2ULL,
    0x72BE5D74F27B896FULL,  0x80DEB1FE3B1696B1ULL, 0x9BDC06A725C71235ULL,  0xC19BF174CF692694ULL,
    0xE49B69C19EF14AD2ULL,  0xEFBE4786384F25E3ULL, 0x0FC19DC68B8CD5B5ULL,  0x240CA1CC77AC9C65ULL,
    0x2DE92C6F592B0275ULL,  0x4A7484AA6EA6E483ULL, 0x5CB0A9DCBD41FBD4ULL,  0x76F988DA831153B5ULL,
    0x983E5152EE66DFABULL,  0xA831C66D2DB43210ULL, 0xB00327C898FB213FULL,  0xBF597FC7BEEF0EE4ULL,
    0xC6E00BF33DA88FC2ULL,  0xD5A79147930AA725ULL, 0x06CA6351E003826FULL,  0x142929670A0E6E70ULL,
    0x27B70A8546D22FFCULL,  0x2E1B21385C26C926ULL, 0x4D2C6DFC5AC42AEDULL,  0x53380D139D95B3DFULL,
    0x650A73548BAF63DEULL,  0x766A0ABB3C77B2A8ULL, 0x81C2C92E47EDAEE6ULL,  0x92722C851482353BULL,
    0xA2BFE8A14CF10364ULL,  0xA81A664BBC423001ULL, 0xC24B8B70D0F89791ULL,  0xC76C51A30654BE30ULL,
    0xD192E819D6EF5218ULL,  0xD69906245565A910ULL, 0xF40E35855771202AULL,  0x106AA07032BBD1B8ULL,
    0x19A4C116B8D2D0C8ULL,  0x1E376C085141AB53ULL, 0x2748774CDF8EEB99ULL,  0x34B0BCB5E19B48A8ULL,
    0x391C0CB3C5C95A63ULL,  0x4ED8AA4AE3418ACBULL, 0x5B9CCA4F7763E373ULL,  0x682E6FF3D6B2B8A3ULL,
    0x748F82EE5DEFB2FCULL,  0x78A5636F43172F60ULL, 0x84C87814A1F0AB72ULL,  0x8CC702081A6439ECULL,
    0x90BEFFFA23631E28ULL,  0xA4506CEBDE82BDE9ULL, 0xBEF9A3F7B2C67915ULL,  0xC67178F2E372532BULL,
    0xCA273ECEEA26619CULL,  0xD186B8C721C0C207ULL, 0xEADA7DD6CDE0EB1EULL,  0xF57D4F7FEE6ED178ULL,
    0x06F067AA72176FBAULL,  0x0A637DC5A2C898A6ULL, 0x113F9804BEF90DAEULL,  0x1B710B35131C471BULL,
    0x28DB77F523047D84ULL,  0x32CAAB7B40C72493ULL, 0x3C9EBE0A15C9BEBCULL,  0x431D67C49C100D4CULL,
    0x4CC5D4BECB3E42B6ULL,  0x597F299CFC657E2AULL, 0x5FCB6FAB3AD6FAECULL,  0x6C44198C4A475817ULL
};

static inline void sha512_memcpy(UINT8 *src, UINT8 *dst, UINT32 size)
{
    UINT32 i = 0u;
    for (i = 0u;i < size;i++) {
        *dst++ = *src++;
    }
}

static inline void sha512_memclr(UINT8 *dst, UINT32 size)
{
    UINT32 i = 0u;
    for (i = 0u;i < size;i++) {
        *dst++ = 0;
    }
}

/**
 * @brief   Init the SHA384/SHA512 Context
 * @param   sha512_ctx      SHA384/512 context
 * @param   payload         address of the hash payload
 * @param   payload_len     length of the hash payload
 * @param   is_sha384       0:SHA512, 1:SHA384
 * @retval  crypto_status_t
 * @return  CRYPTO_FAIL if hash failed
 *          CRYPTO_SUCCESS if hash successed
 */
static crypto_status_t sha512_init(sha512_ctx_t *sha512_ctx, UINT8 *payload_addr, UINT64 payload_len, UINT32 is_sha384)
{
    crypto_status_t ret = CRYPTO_FAIL;

    SHA512_DEBUG("%s\n", __func__);
    if (payload_len == 0u || payload_addr == NULL) {
        SHA512_DEBUG("%s parameter illegal\n", __func__);
    }
    else {
        sha512_memclr((UINT8 *)sha512_ctx, (UINT32)sizeof(sha512_ctx_t));
        if (1u == is_sha384) {
            SHA512_DEBUG("%s SHA384\n", __func__);
            sha512_ctx->val[0] = 0xCBBB9D5DC1059ED8ULL;
            sha512_ctx->val[1] = 0x629A292A367CD507ULL;
            sha512_ctx->val[2] = 0x9159015A3070DD17ULL;
            sha512_ctx->val[3] = 0x152FECD8F70E5939ULL;
            sha512_ctx->val[4] = 0x67332667FFC00B31ULL;
            sha512_ctx->val[5] = 0x8EB44A8768581511ULL;
            sha512_ctx->val[6] = 0xDB0C2E0D64F98FA7ULL;
            sha512_ctx->val[7] = 0x47B5481DBEFA4FA4ULL;
        } else {
            SHA512_DEBUG("%s SHA512\n", __func__);
            sha512_ctx->val[0] = 0x6A09E667F3BCC908ULL;
            sha512_ctx->val[1] = 0xBB67AE8584CAA73BULL;
            sha512_ctx->val[2] = 0x3C6EF372FE94F82BULL;
            sha512_ctx->val[3] = 0xA54FF53A5F1D36F1ULL;
            sha512_ctx->val[4] = 0x510E527FADE682D1ULL;
            sha512_ctx->val[5] = 0x9B05688C2B3E6C1FULL;
            sha512_ctx->val[6] = 0x1F83D9ABFB41BD6BULL;
            sha512_ctx->val[7] = 0x5BE0CD19137E2179ULL;
        }

        sha512_ctx->is_sha384 = is_sha384;
        sha512_ctx->payload_addr = payload_addr;
        sha512_ctx->payload_len = (UINT64)payload_len;
        sha512_ctx->len[0] = payload_len << 3u;
        sha512_ctx->len[1] = payload_len >> 61u;
        ret = CRYPTO_SUCCESS;
    }
    return ret;
}

/**
 * @brief   SHA384/512 iteration compression
 * @param   sha512_ctx        context of the sha384/512
 * @param   data              hash block data, 1024 bits.
 * @retval  crypto_status_t
 * @return  CRYPTO_FAIL if failed
 *          CRYPTO_SUCCESS if successed
 */
static crypto_status_t sha512_hash_factory(sha512_ctx_t *ctx, UINT8 data[128])
{
    UINT32 i = 0u;
    UINT64 W[80];
    /* One iteration vectors
     * v[0] --> A
     * ...
     * v[7] --> H
     * */
    UINT64 v[8];

    INIT_COMPRESSOR();
    SHA512_DEBUG("%s\n", __func__);

    /* 1. Calculate the W[80] */
    for(i = 0u; i < 16u; i++) {
        sha512_decode(&W[i], data, i << 3u);
    }

    for(i = i; i < 80u; i++) {
        W[i] = GAMMA1(W[i -  2u]) + W[i -  7u] + GAMMA0(W[i - 15u]) + W[i - 16u];
    }

    /* 2.Init the vectors */
    for (i = 0u; i < 8u; i++) {
        v[i] = ctx->val[i];
    }

    /* 3. Iteration to do the SHA-2 family compression. */
    for(i = 0u; i < 80u;) {
        COMPRESS(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], W[i], K[i] ); i++;
        COMPRESS(v[7], v[0], v[1], v[2], v[3], v[4], v[5], v[6], W[i], K[i] ); i++;
        COMPRESS(v[6], v[7], v[0], v[1], v[2], v[3], v[4], v[5], W[i], K[i] ); i++;
        COMPRESS(v[5], v[6], v[7], v[0], v[1], v[2], v[3], v[4], W[i], K[i] ); i++;
        COMPRESS(v[4], v[5], v[6], v[7], v[0], v[1], v[2], v[3], W[i], K[i] ); i++;
        COMPRESS(v[3], v[4], v[5], v[6], v[7], v[0], v[1], v[2], W[i], K[i] ); i++;
        COMPRESS(v[2], v[3], v[4], v[5], v[6], v[7], v[0], v[1], W[i], K[i] ); i++;
        COMPRESS(v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[0], W[i], K[i] ); i++;

    }

    /* 4. Move the vectors to hash output */
    for (i = 0u; i < 8u; i++) {
        ctx->val[i] += v[i];
    }

    return CRYPTO_SUCCESS;
}

/**
 * @brief   SHA384/512 stage1
 * @param   sha512_ctx        context of the sha384/512
 * @param   output            output of hash value
 * @retval  crypto_status_t
 * @return  CRYPTO_FAIL if failed
 *          CRYPTO_SUCCESS if successed
 */
static crypto_status_t sha512_stage1(sha512_ctx_t *sha512_ctx)
{
    SHA512_DEBUG("%s\n", __func__);

    while (sha512_ctx->payload_len >= 128u) {
        sha512_hash_factory(sha512_ctx, sha512_ctx->payload_addr);
        sha512_ctx->payload_addr += 128u;
        sha512_ctx->payload_len -= 128u;
        SHA512_DEBUG("%x, %x\n", (UINT32) sha512_ctx->payload_addr, (UINT32) sha512_ctx->payload_len);
    }

    return CRYPTO_SUCCESS;
}


/**
 * @brief   SHA384/512 stage2:Do padding and digest the fianl bytes
 * @param   sha512_ctx        context of the sha384/512
 * @param   output            output of hash value
 * @retval  crypto_status_t
 * @return  CRYPTO_FAIL if failed
 *          CRYPTO_SUCCESS if successed
 */
static crypto_status_t sha512_stage2(sha512_ctx_t *sha512_ctx,
        UINT8 output[64])
{

    UINT32 block_pos = (UINT32)sha512_ctx->payload_len;
    UINT32 padding_bytes = 0u;
    UINT8 temp_data[128] = {0u};
    UINT8 *temp_data_p = (UINT8 *)&temp_data[0];
    UINT8 len_be[16] = {0u};
    UINT32 i = 0u;

    SHA512_DEBUG("%s\n", __func__);

    /*Copy the last byte to the temp buffer*/
    sha512_memcpy(sha512_ctx->payload_addr, temp_data_p, (UINT32)sha512_ctx->payload_len);
    padding_bytes = 112u - block_pos;
    temp_data_p += block_pos;

    /*Copy the padding byte to the temp buffer*/
    sha512_memcpy((UINT8 *)sha512_padding, temp_data_p, padding_bytes);
    temp_data_p += padding_bytes;

    /*Append the length*/
    sha512_encode(sha512_ctx->len[1], len_be, 0u);
    sha512_encode(sha512_ctx->len[0], len_be, 8u);
    sha512_memcpy(len_be, temp_data_p, 16u);
    sha512_hash_factory(sha512_ctx, temp_data);

    /*encode the hash val to big endian byte array*/
    for (i = 0u; i < 6u; i++) {
        sha512_encode(sha512_ctx->val[i], output, i * 8u);
    }

    /*No need to encode the last 16 bytes for SHA384*/
    for (i = i ;(i < 8u) && (sha512_ctx->is_sha384 == 0u); i++) {
        sha512_encode(sha512_ctx->val[i], output, i * 8u);
    }

    return CRYPTO_SUCCESS;
}

/**
 * @brief   SHA384/512 implementation function
 * @param   payload         address of the hash payload
 * @param   payload_len     length of the hash payload
 * @param   hash            output of hash value
 * @param   is_sha384       0:SHA512, 1:SHA384
 * @retval  crypto_status_t
 * @return  CRYPTO_FAIL if hash failed
 *          CRYPTO_SUCCESS if hash successed
 */
crypto_status_t easy_sha512_impl(UINT8 *payload, UINT64 payload_len,
        UINT8 output[64], UINT32 is_sha384)
{

    crypto_status_t ret = CRYPTO_FAIL;

    sha512_ctx_t g_sha512_ctx;
    SHA512_DEBUG("%s payload_addr:%x, payload_len:%x\n", __func__, payload, payload_len);
    /* coverity[value_overwrite] */
    ret = sha512_init(&g_sha512_ctx, payload, payload_len, is_sha384);
    if (ret == CRYPTO_SUCCESS) {
        /* coverity[uninit_use_in_call] */
        ret = sha512_stage1(&g_sha512_ctx);
        if (ret == CRYPTO_SUCCESS) {
            ret = sha512_stage2(&g_sha512_ctx, output);
        }
    }
    return ret;
}

/**
 * @brief   API for SHA512
 * @param   payload         address of the hash payload
 * @param   payload_len     length of the hash payload
 * @param   hash            output of hash value
 * @retval  crypto_status_t
 * @return  CRYPTO_FAIL if hash failed
 *          CRYPTO_SUCCESS if hash successed
 */
crypto_status_t easy_sha512(UINT8 *payload, UINT64 payload_len, UINT8 hash[64])
{
    return easy_sha512_impl(payload, payload_len, hash, 0u);
}

/**
 * @brief   API for SHA384
 * @param   payload         address of the hash payload
 * @param   payload_len     length of the hash payload
 * @param   hash            output of hash value
 * @retval  crypto_status_t
 * @return  CRYPTO_FAIL if hash failed
 *          CRYPTO_SUCCESS if hash successed
 */
crypto_status_t easy_sha384(UINT8 *payload, UINT64 payload_len, UINT8 hash[64])
{
    return easy_sha512_impl(payload, payload_len, hash, 1u);
}

#endif
