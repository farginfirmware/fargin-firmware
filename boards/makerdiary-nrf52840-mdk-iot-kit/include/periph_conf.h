#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#include "periph_cpu.h"
#include "cfg_clock_32_1.h"
#include "cfg_i2c_default.h"
#include "cfg_rtt_default.h"
// #include "cfg_spi_default.h"
#include "cfg_timer_default.h"


static const spi_conf_t spi_config[] = {
    // 64Mb NOR flash MX25R6435F
    //     CS#              P1.13
    //     SCLK             P1.11
    //     SI               P1.10   SIO0
    //     SO               P1.14   SIO1
    //     WP#              P1.15   SIO2
    //     RESET# / HOLD#   P1.12   SIO3
    {
        .dev  = NRF_SPIM0,
        .sclk = GPIO_PIN (1,  3),
        .mosi = GPIO_PIN (1, 10),
        .miso = GPIO_PIN (1, 14),
    },
    // external bus
    {
        .dev  = NRF_SPIM2,
        .sclk = GPIO_PIN (0, 11),
        .mosi = GPIO_PIN (0, 12),
        .miso = GPIO_PIN (1,  8),
    }
};
#define SPI_NUMOF  ARRAY_SIZE (spi_config)


static const uart_conf_t uart_config[] = {
    {
        .dev        = NRF_UARTE0,
        .rx_pin     = GPIO_PIN(0,15),
        .tx_pin     = GPIO_PIN(0,16),
    #ifdef MODULE_PERIPH_UART_HW_FC
        .rts_pin    = GPIO_UNDEF,
        .cts_pin    = GPIO_UNDEF,
    #endif
        .irqn       = UARTE0_UART0_IRQn,
    },
};

#define UART_0_ISR  (isr_uart0)

#define UART_NUMOF  ARRAY_SIZE(uart_config)

#endif
