#ifdef MODULE_MTD

#include "board.h"
#include "mtd.h"
#include "mtd_spi_nor.h"
#include "periph_conf.h"
#include "timex.h"

static const mtd_spi_nor_params_t makerdiary_nrf52840_m2_nor_params = {
    .opcode            = & mtd_spi_nor_opcode_default,
    .wait_chip_erase   =   50LU * US_PER_SEC,
    .wait_32k_erase    =  240LU * US_PER_MS,
    .wait_sector_erase =   40LU * US_PER_MS,
    .wait_chip_wake_up =   35LU * US_PER_MS,
    .clk  =  NRF52840DK_NOR_SPI_CLK,
    .flag =  NRF52840DK_NOR_FLAGS,
    .spi  =  NRF52840DK_NOR_SPI_DEV,
    .mode =  NRF52840DK_NOR_SPI_MODE,
    .cs   =  NRF52840DK_NOR_SPI_CS,
  #if 1
    .wp   =  NRF52840DK_NOR_SPI_WP,
    .hold =  NRF52840DK_NOR_SPI_HOLD,
  #else
    .wp   =  GPIO_UNDEF,
    .hold =  GPIO_UNDEF,
  #endif
};

static mtd_spi_nor_t makerdiary_nrf52840_m2_nor_dev = {
    .base = {
        .driver           = & mtd_spi_nor_driver,
        .page_size        = NRF52840DK_NOR_PAGE_SIZE,
        .pages_per_sector = NRF52840DK_NOR_PAGES_PER_SECTOR,
        .sector_count     = NRF52840DK_NOR_SECTOR_COUNT,
    },
    .params = & makerdiary_nrf52840_m2_nor_params,
};

mtd_dev_t * mtd0 = (mtd_dev_t *) & makerdiary_nrf52840_m2_nor_dev ;

#ifdef MODULE_VFS_DEFAULT
  #include "vfs_default.h"
  VFS_AUTO_MOUNT (littlefs2, VFS_MTD (makerdiary_nrf52840_m2_nor_dev), VFS_DEFAULT_NVM (0), 0) ;
#endif

#endif
