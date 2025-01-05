#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"
#include "periph_cpu.h"
#include "periph/gpio.h"

#ifdef BOARD_ADAFRUIT_METRO_RP2040

#define LED_RED  13
#define LED0_PIN        GPIO_PIN(0, LED_RED)
#define LED0_ON         do {SIO->GPIO_OUT_SET = 1UL << LED_RED;} while (0)
#define LED0_OFF        do {SIO->GPIO_OUT_CLR = 1UL << LED_RED;} while (0)
#define LED0_TOGGLE     do {SIO->GPIO_OUT_XOR = 1UL << LED_RED;} while (0)
#define LED0_NAME       "LED(red)"

#endif

#endif
