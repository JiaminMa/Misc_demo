#include <stdint.h>
#include <stdio.h>
#include "ahab_core.h"

#define AHAB_SW_STR         "AHAB_SW"
#define AHAB_SECURE_RAM_SZ  (64U * 1024U)
#define FAKE_IMG_BUFFER_SZ  (1024U * 1024U)


uint8_t fake_image_buffer[FAKE_IMG_BUFFER_SZ];
uint8_t ahab_secure_ram[AHAB_SECURE_RAM_SZ];

void ahab_sw_read_img_to_ahab_buf(const char * file_path)
{

    FILE *fp = NULL;
    uint8_t *buf = fake_image_buffer;
    fp = fopen(file_path, "r");
    while(0U != fread(buf, 1024U, 1U, fp)) {
        buf += 1024U;
    }

    fclose(fp);
}

int main(int argc, char **argv)
{
    printf("%s: %s ahab_sw emulation\n", AHAB_SW_STR, __func__);
    printf("%s: %s binary file:%s\n", AHAB_SW_STR, __func__, argv[1]);

    ahab_sw_read_img_to_ahab_buf(argv[1]);
    ahab_parse_ctnr(&fake_image_buffer[0x1000]);

    ahab_parse_ctnr(&fake_image_buffer[0x1400]);
    return 0;
}
