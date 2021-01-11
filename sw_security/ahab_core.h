#ifndef AHAB_CORE_H
#define AHAB_CORE_H

#include <stdint.h>
#define AHAB_CORE_STR "AHAB_CORE"
#define HASH_BUF_SZ     48U
#define IV_BUF_SZ       48U


#define CTNR_SRK_SET(flag)  ((flag & 0x3U))
#define CTNR_SRK_ID(flag)   ((flag & 0x0030U) >> 4U)
#define CTNR_REVOKE_MASK(flag)  (((flag) & 0x0700U) >> 8U)
#define CTNR_FIELD_RETURE(flag) (((flag) & 0x10000U) >> 16U)

#define IMG_TYPE(flag)      ((flag) & 0xfU)
#define IMG_CORE_ID(flag)   (((flag) & 0xf0U) >> 4U)
#define IMG_HASH_TYPE(flag) (((flag) & 0x700U) >> 8U)
#define IMG_ENC_FLAG(flag)  (((flag) & 0x800U) >> 11U)

#define IMG_TYPE_EXEC       3U
#define IMG_TYPE_DATA       4U
#define IMG_TYPE_DDRI       5U
#define IMG_TYPE_SECO       6U
#define IMG_TYPE_PROV       7U
#define IMG_TYPE_DEK        8U

#define IMG_HASH_SHA256     0U
#define IMG_HASH_SHA384     1U
#define IMG_HASH_SHA512     2U

#define SRK_ENC_ALOG_ECDSA      0x27U
#define SRK_HASH_ALGO_SHA256    0x00U
#define SRK_HASH_ALOG_SHA384    0x01U
#define SRK_HASK_ALOG_SHA512    0x02U

#define SRK_FLAG_NON            0x00U
#define SRK_FLAG_CA             0x01U

#pragma pack(1)

typedef struct image_entity_tag {
    uint32_t off;
    uint32_t size;
    uint64_t load_addr;
    uint64_t entry_point;
    uint32_t flags;
    uint32_t metadata;
    uint8_t hash[HASH_BUF_SZ];
    uint8_t iv[IV_BUF_SZ];
} image_entity_t;

typedef struct ctnr_hdr_tag {
    uint8_t version;
    uint16_t length;
    uint8_t tag;
    uint32_t flags;
    uint16_t sw_version;
    uint8_t fuse_version;
    uint8_t nr_images;
    uint16_t sig_block_off;
    uint16_t reserved;
    image_entity_t img_arrary[];
} ctnr_hdr_t;

typedef struct srk_tag {
    uint8_t tag;
    uint16_t length;
    uint8_t enc_alog;
    uint8_t hash_alog;
    uint8_t key_size;
    uint8_t not_used;
    uint8_t srk_flag;
    uint16_t x_len;
    uint16_t y_len;
    uint8_t srk_payload[];
} srk_t;

typedef struct srk_table_tag {
    uint8_t tag;
    uint16_t length;
    uint8_t version;
    srk_t srk_array[];
} srk_table_t;

typedef struct sig_block_tag {

    uint8_t version;
    uint16_t length;
    uint8_t tag;
    uint16_t cert_off;
    uint16_t reserved;
    uint16_t sig_off;
    uint16_t blob_off;
    uint32_t reserved2;
    srk_table_t srk_table;
} sig_block_t;

typedef struct sig_data_tag {
    uint8_t version;
    uint16_t length;
    uint8_t tag;
    uint32_t reserved;
    uint8_t payload[];
} sig_data_t;

extern void ahab_parse_ctnr(uint8_t *ctnr_start);

#endif
