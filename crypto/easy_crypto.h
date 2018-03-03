/*
 * Copyright (c) 2018, Jiamin Ma
 * BSD License
 */
#ifndef EASY_CRYPTO_H
#define EASY_CRYPTO_H

#include <stdint.h>

#ifdef CRYPTO_DEBUG_SUPPORT
#include <stdio.h>
#endif

typedef uint32_t crypto_status_t;
#define CRYPTO_FAIL             0x5A5A5A5AUL
#define CRYPTO_SUCCESS          0xA5A5A5A5UL

extern crypto_status_t easy_sha512(uint8_t *payload, uint64_t payaload_len, uint8_t hash[64]);
extern crypto_status_t easy_sha384(uint8_t *payload, uint64_t payaload_len, uint8_t hash[64]);

#endif /*EASY_CRYPTO_H*/

