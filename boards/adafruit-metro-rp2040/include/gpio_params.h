#ifndef GPIO_PARAMS_H
#define GPIO_PARAMS_H

#include "board.h"
#include "saul/periph.h"


#ifdef BOARD_ADAFRUIT_METRO_RP2040

static const  saul_gpio_params_t saul_gpio_params[] =
{
    {
        .name = LED0_NAME,
        .pin  = LED0_PIN,
        .mode = GPIO_OUT,
    }
};

#endif

#endif
