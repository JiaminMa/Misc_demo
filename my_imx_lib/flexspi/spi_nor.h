#ifndef SPI_NOR_H
#define SPI_NOR_H

#include <stdint.h>

typedef struct spi_nor_cmd_set_tag {

    uint8_t reset_enable;
    uint8_t reset_memory;
    uint8_t read_id;
    uint8_t read_sfdp;

    uint8_t read;
    uint8_t read_dummy_cycles;
    uint8_t read_cmd_mode;
    uint8_t read_addr_mode;
    uint8_t read_data_mode; 

    uint8_t write_enable;
    uint8_t write_disable;

    uint8_t read_status_reg;
    uint8_t read_flag_status_reg;
    uint8_t read_nvconfig_reg;
    uint8_t read_vconfig_reg;
    uint8_t read_evconfig_reg;
    uint8_t read_eadd_reg;
    uint8_t read_gp_read_reg;

    uint8_t program;
    uint8_t program_dummy_cycles;
    uint8_t program_cmd_mode;
    uint8_t program_addr_mode;
    uint8_t program_data_mode;

    uint8_t write_status_reg;
    uint8_t write_nvconfig_reg;
    uint8_t write_vconfig_reg;
    uint8_t write_evconfig_reg;
    uint8_t write_eaddr_reg;

    uint8_t clear_flag_status_reg;
    uint8_t erase_bulk;
    uint8_t erase_32KB_subsector;
    uint8_t erase_4KB_subsector;
    uint8_t erase_sector;

}spi_nor_cmd_set_t;

typedef struct spi_nor_tag {
    
    void *host;

    spi_nor_cmd_set_t cmd_set;

    uint32_t (*read_id)(struct spi_nor_tag *spi_nor, uint8_t *id_buf, uint32_t buf_size);
    uint32_t (*read_sfdp)(struct spi_nor_tag *spi_nor, uint8_t *sfdp_buf);
    uint32_t (*read)(struct spi_nor_tag *spi_nor, uint8_t *dst, uint8_t *src, uint32_t size);
    uint32_t (*write)(struct spi_nor_tag *spi_nor, uint8_t *dst, uint8_t *src, uint32_t size);
    uint32_t (*write_enable)(struct spi_nor_tag *spi_nor);
    uint32_t (*erase_sector)(struct spi_nor_tag *spi_nor, uint32_t addr);
    uint32_t (*erase_bulk)(struct spi_nor_tag *spi_nor);
    uint32_t (*read_status_reg)(struct spi_nor_tag *spi_nor, uint8_t *status_reg);
} spi_nor_t;

extern uint32_t init_spi_nor(spi_nor_t *spi_nor, uint32_t instance);

#endif /* SPI_NOR_H */
