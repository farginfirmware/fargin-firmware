#ifndef BOARD_H
#define BOARD_H

#include "cpu.h"
#include "periph/gpio.h"


#define LED_BLUE_PIN  13

#define LED0_PIN            GPIO_PIN(PA, LED_BLUE_PIN)

#define LED_PORT            PORT->Group[PA]
#define LED0_MASK           (1 << LED_BLUE_PIN)

#define LED0_ON             (LED_PORT.OUTSET.reg = LED0_MASK)
#define LED0_OFF            (LED_PORT.OUTCLR.reg = LED0_MASK)
#define LED0_TOGGLE         (LED_PORT.OUTTGL.reg = LED0_MASK)

/**
 * @name APA102 / DotStar configuration
 * @{
 */
#define APA102_PARAM_LED_NUMOF      (1)
#define APA102_PARAM_DATA_PIN       GPIO_PIN(PB, 3)
#define APA102_PARAM_CLK_PIN        GPIO_PIN(PB, 2)

/**
 * @name MTD configuration
 * @{
 */
#define MTD_0 mtd_dev_get(0)

/**
 * @name    Xtimer configuration
 * @{
 */
#define XTIMER_WIDTH                (32)
#define XTIMER_HZ                   (1000000ul)


#endif
