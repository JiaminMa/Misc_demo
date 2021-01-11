#include "ahab_core.h"
#include <stdio.h>


static void ahab_parse_image_array(ctnr_hdr_t *ctnr_hdr)
{
    uint8_t i = 0, j = 0;
    char *img_type_str = NULL;
    char *hash_type_str = NULL;
    image_entity_t *img = NULL;

    printf("====================================================\n");
    printf("                Image Array \n");
    for (i = 0; i < ctnr_hdr->nr_images; i++) {
        img = &ctnr_hdr->img_arrary[i];

        switch(IMG_TYPE(img->flags)) {
        case IMG_TYPE_EXEC:
            img_type_str = "EXEC";
            break;
        case IMG_TYPE_DATA:
            img_type_str = "DATA";
            break;
        case IMG_TYPE_DDRI:
            img_type_str = "DDRI";
            break;
        case IMG_TYPE_SECO:
            img_type_str = "SECO";
            break;
        case IMG_TYPE_PROV:
            img_type_str = "PROV";
            break;
        case IMG_TYPE_DEK:
            img_type_str = "DEK";
            break;
        default:
            img_type_str = "UNSP";
            break;
        }

        switch(IMG_HASH_TYPE(img->flags)) {
        case IMG_HASH_SHA256:
            hash_type_str = "SHA256";
            break;
        case IMG_HASH_SHA384:
            hash_type_str = "SHA384";
            break;
        case IMG_HASH_SHA512:
            hash_type_str = "SHA512";
            break;
        default:
            hash_type_str = "UNSPHS";
            break;
        }

        printf("----------------------------------------------------\n");
        printf("                Image %d\n", i);
        printf("    image offset:   0x%08x\n", img->off);
        printf("    image size:     0x%08x\n", img->size);
        printf("    load addr:      0x%lx\n", img->load_addr);
        printf("    entry addr:     0x%lx\n", img->entry_point);
        printf("    flags:          0x%08x\n", img->flags);
        printf("        type:       %s\n", img_type_str);
        printf("        core id:    0x%02x\n", IMG_CORE_ID(img->flags));
        printf("        hash type:  %s\n", hash_type_str);
        printf("        enc flags:  %x\n", IMG_ENC_FLAG(img->flags));
        printf("    metadata:0x%08x\n", img->metadata);
        printf("    hash\n");
        for (j = 0; j < 64; j++) {
            printf("%02x", img->hash[j]);
        }
        printf("\n");
        printf("    iv\n");
        for (j = 0; j < 64; j++) {
            printf("%02x", img->iv[j]);
        }
        printf("\n");


    }
    printf("====================================================\n");
}

static void ahab_parse_sig_block(uint8_t *sig_block_start)
{
    uint8_t i = 0, j = 0;
    uint8_t *key = NULL;
    sig_block_t *sig_block = (sig_block_t *)sig_block_start;
    sig_data_t *sig_data = (sig_data_t *)(sig_block_start + sig_block->sig_off);
    srk_t *srk = NULL;
    char *hash_alog_str = NULL;
    uint32_t srk_off = 0;

    printf("====================================================\n");
    printf("                Sig block \n");
    printf("    version:    0x%02x\n", sig_block->version);
    printf("    length:     0x%02x\n", sig_block->length);
    printf("    tag:        0x%02x\n", sig_block->tag);
    printf("    cert off:   0x%04x\n", sig_block->cert_off);
    printf("    reserved:   0x%04x\n", sig_block->reserved);
    printf("    sig off:    0x%04x\n", sig_block->sig_off);
    printf("    blob off:   0x%04x\n", sig_block->blob_off);
    printf("----------------------------------------------------\n");
    printf("                    SRK Table \n");
    printf("            version:    0x%02x\n", sig_block->srk_table.version);
    printf("            length:     0x%04x\n", sig_block->srk_table.length);
    printf("            tag:        0x%02x\n", sig_block->srk_table.tag);
    printf("----------------------------------------------------\n");

    
    for (i = 0; i < 4; i++) {
        srk = (srk_t *)((uint8_t *)&sig_block->srk_table.srk_array[0] + srk_off); 
        
        switch(srk->hash_alog) {
        case SRK_HASH_ALGO_SHA256:
            hash_alog_str = "SHA256";
            break;
        case SRK_HASH_ALOG_SHA384:
            hash_alog_str = "SHA384";
            break;
        default:
            hash_alog_str = "SHA512";
            break;
        }
        printf("                SRK %d\n", i);
        printf("            tag:0x%02x\n", srk->tag);
        printf("            length:0x%02x\n", srk->length);
        printf("            enc_alog:%s\n", srk->enc_alog == SRK_ENC_ALOG_ECDSA ? "ECDSA" : "RSA");
        printf("            hash_alog:%s\n", hash_alog_str);
        printf("            x_len:0x%04x\n", srk->x_len);
        printf("            y_len:0x%04x\n", srk->y_len);
        key = &srk->srk_payload[0];
        printf("            X Comp\n");
        for (j = 0; j < srk->x_len; j++) {
            printf("%02x", key[j]);
        }
        printf("\n");

        key = key + srk->x_len;
        printf("            Y Comp\n");
        for (j = 0; j < srk->y_len; j++) {
            printf("%02x", key[j]);
        }
        printf("\n");
        printf("----------------------------------------------------\n");
        srk_off += srk->length;
    }
    printf("----------------------------------------------------\n");
    printf("                    Sig Data \n");
    printf("            version:0x%02x\n", sig_data->version);
    printf("            length:0x%04x\n", sig_data->length);
    printf("            tag:0x%02x\n", sig_data->tag);
    printf("            Sig payload:\n");
    key = &sig_data->payload[0];
    for (j = 0; j < sig_data->length - 4; j++) {
        printf("%02x", key[j]);
    }
    printf("\n");
    printf("----------------------------------------------------\n");

    
    printf("====================================================\n");
}

void ahab_parse_ctnr(uint8_t *ctnr_start)
{
    ctnr_hdr_t *ctnr_hdr = (ctnr_hdr_t *)ctnr_start;
    sig_block_t *sig_block = (sig_block_t *)(ctnr_start + ctnr_hdr->sig_block_off);
    const char *srk_set_str[4] = {
                        "Container not auth",
                        "NXP",
                        "OEM",
                        "Reserved"};

    printf("%s: %s entry\n", AHAB_CORE_STR, __func__);

    printf("====================================================\n");
    printf("                Container header \n");
    printf("    tag:    0x%02x\n", ctnr_hdr->tag);
    printf("    length: 0x%02x\n", ctnr_hdr->length);
    printf("    ver:    0x%02x\n", ctnr_hdr->version);
    printf("    flags:  0x%08x\n", ctnr_hdr->flags);
    printf("        srk_set:%s\n", srk_set_str[CTNR_SRK_SET(ctnr_hdr->flags)]);
    printf("        srk id: 0x%02x\n", CTNR_SRK_ID(ctnr_hdr->flags));
    printf("        revok_m:0x%02x\n", CTNR_REVOKE_MASK(ctnr_hdr->flags));
    printf("        field_r:0x%02x\n", CTNR_FIELD_RETURE(ctnr_hdr->flags));
    printf("    sw_v:   0x%04x\n", ctnr_hdr->sw_version);
    printf("    fuse_v: 0x%02x\n", ctnr_hdr->fuse_version);
    printf("    nr_imgs:0x%02x\n", ctnr_hdr->nr_images);
    printf("    sig_off:0x%02x\n", ctnr_hdr->sig_block_off);
    printf("====================================================\n");

    ahab_parse_image_array(ctnr_hdr);
    ahab_parse_sig_block((uint8_t *)sig_block);
}
