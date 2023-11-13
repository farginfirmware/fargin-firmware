#ifndef PERIPH_CONF_H
#define PERIPH_CONF_H

#include "periph_cpu.h"
#include "cfg_clock_32_1.h"
#include "cfg_i2c_default.h"
#include "cfg_rtt_default.h"
#include "cfg_spi_default.h"
#include "cfg_timer_default.h"

#ifdef __cplusplus
extern "C" {
#endif

static const uart_conf_t uart_config[] = {
    {
        .dev        = NRF_UARTE0,
        .rx_pin     = GPIO_PIN(0,8),
        .tx_pin     = GPIO_PIN(0,6),
#ifdef MODULE_PERIPH_UART_HW_FC
        .rts_pin    = GPIO_UNDEF,
        .cts_pin    = GPIO_UNDEF,
#endif
        .irqn       = UARTE0_UART0_IRQn,
    },
};

#define UART_0_ISR      (isr_uart0)

#define UART_NUMOF       ARRAY_SIZE(uart_config)


#if 0
i2c_config[] is configured in RIOT/boards/common/nrf52/include/cfg_i2c_default.h
static const i2c_conf_t i2c_config[] = {
    {
        .dev   = NRF_TWIM1,
        .scl   = GPIO_PIN(0, 27),
        .sda   = GPIO_PIN(0, 26),
        .speed = I2C_SPEED_NORMAL
    }
};
#define I2C_NUMOF   ARRAY_SIZE(i2c_config)
#endif

#ifdef __cplusplus
}
#endif

#endif
