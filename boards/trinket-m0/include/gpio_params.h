#ifndef GPIO_PARAMS_H
#define GPIO_PARAMS_H

#include "board.h"
#include "saul/periph.h"

static const  saul_gpio_params_t saul_gpio_params[] =
{
  {
    .name = "LED(Red)",
    .pin  =  LED0_PIN,
    .mode =  GPIO_OUT
  },
};

#endif
