#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph_conf.h"
#include "periph_cpu.h"
#include "periph/gpio.h"


#define LED0_PIN            GPIO_PIN(PA, 10)

#define LED_PORT            PORT->Group[PA]
#define LED0_MASK           (1 << 10)

#define LED0_ON             (LED_PORT.OUTSET.reg = LED0_MASK)
#define LED0_OFF            (LED_PORT.OUTCLR.reg = LED0_MASK)
#define LED0_TOGGLE         (LED_PORT.OUTTGL.reg = LED0_MASK)

#define APA102_PARAM_LED_NUMOF      (1)
#define APA102_PARAM_DATA_PIN       GPIO_PIN(PA, 0)
#define APA102_PARAM_CLK_PIN        GPIO_PIN(PA, 1)


#endif
