#ifndef BOARD_H
#define BOARD_H

#include "board_common.h"
#include "cpu.h"
#include "mtd.h"

#define LED0_PIN            GPIO_PIN(0, 30)     // red
#define LED1_PIN            GPIO_PIN(0, 29)     // green
#define LED2_PIN            GPIO_PIN(0, 31)     // blue

#define LED_PORT            (NRF_P0)

#define LED0_MASK           (1 << 30)
#define LED1_MASK           (1 << 29)
#define LED2_MASK           (1 << 31)

#define LED_MASK            (LED0_MASK | LED1_MASK | LED2_MASK)


#define LED0_ON             (LED_PORT->OUTCLR = LED0_MASK)
#define LED0_OFF            (LED_PORT->OUTSET = LED0_MASK)
#define LED0_TOGGLE         (LED_PORT->OUT   ^= LED0_MASK)

#define LED1_ON             (LED_PORT->OUTCLR = LED1_MASK)
#define LED1_OFF            (LED_PORT->OUTSET = LED1_MASK)
#define LED1_TOGGLE         (LED_PORT->OUT   ^= LED1_MASK)

#define LED2_ON             (LED_PORT->OUTCLR = LED2_MASK)
#define LED2_OFF            (LED_PORT->OUTSET = LED2_MASK)
#define LED2_TOGGLE         (LED_PORT->OUT   ^= LED2_MASK)


#define BTN0_PIN            GPIO_PIN(0, 19)
#define BTN0_MODE           GPIO_IN_PU


//  SPI NOR flash hardware configuration
//
//  a Macronix MX25R6435F is present on the board
//
#define NRF52840DK_NOR_PAGE_SIZE          (256)
#define NRF52840DK_NOR_PAGES_PER_SECTOR   (16)
#define NRF52840DK_NOR_SECTOR_COUNT       (2048)
#define NRF52840DK_NOR_FLAGS              (SPI_NOR_F_SECT_4K | SPI_NOR_F_SECT_32K)
#define NRF52840DK_NOR_SPI_DEV            SPI_DEV (0)                               // tbd
#define NRF52840DK_NOR_SPI_CLK            SPI_CLK_10MHZ
#define NRF52840DK_NOR_SPI_CS             GPIO_PIN (1, 13)
#define NRF52840DK_NOR_SPI_WP             GPIO_PIN (1, 15)
#define NRF52840DK_NOR_SPI_HOLD           GPIO_PIN (1, 12)
#define NRF52840DK_NOR_SPI_MODE           SPI_MODE_0

// Default MTD device
#define MTD_0 mtd0

// mtd flash emulation device
extern mtd_dev_t * mtd0 ;


#endif
