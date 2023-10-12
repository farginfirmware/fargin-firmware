
#include "gpio.h"

#include "board.h"
#include "periph/gpio.h"

#include <string.h>


// these constants are tightly coupled to Lua variables

    #define Service_getHandle   0
    #define Service_configure   1
    #define Service_read        2
    #define Service_write       3

    #define InputType_noPull                    0
    #define InputType_pullUp                    1
    #define InputType_pullDown                  2
    #define OutputType_pushPull                 3
    #define OutputType_openDrain                4
    #define OutputType_openDrainPullupEnabled   5



static bool getGpio (ServiceBuffer * request, gpio_t * gpioPtr)
{
    return serviceBuffer_getBytesCopy (request, (uint8_t *) gpioPtr, sizeof (gpio_t)) ;
}



bool gpio_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    uint8_t requestedService ;

    bool fault = ! serviceBuffer_getByte (request, & requestedService) ;

    if (! fault)
        switch (requestedService)
        {
            default :
                fault = true ;
                break ;


            case Service_getHandle :
            {
                // respond with a gpio_t for a given port/bit
                uint8_t port ;
                uint8_t bit ;
                gpio_t  gpio ;

                fault = ! serviceBuffer_getByte (request, & port) ||
                        ! serviceBuffer_getByte (request, & bit) ;

                if (fault)
                    break ;

                gpio = GPIO_PIN (port, bit) ;
                fault = ! serviceBuffer_putBytes (response, (uint8_t *) & gpio, sizeof (gpio)) ;

                break ;
            }


            case Service_configure :
            {
                gpio_t      gpio ;
                uint8_t     ioType ;
                gpio_mode_t mode ;

                fault = ! getGpio (request, & gpio) ||
                        ! serviceBuffer_getByte (request, & ioType) ;

                if (fault)
                    break ;

                switch (ioType)
                {
                    default : fault = true ; break ;

                    case InputType_noPull                   :   mode = GPIO_IN    ;    break ;
                    case InputType_pullUp                   :   mode = GPIO_IN_PU ;    break ;
                    case InputType_pullDown                 :   mode = GPIO_IN_PD ;    break ;

                    case OutputType_pushPull                :   mode = GPIO_OUT   ;    break ;
                    case OutputType_openDrain               :   mode = GPIO_OD    ;    break ;
                    case OutputType_openDrainPullupEnabled  :   mode = GPIO_OD_PU ;    break ;
                }

                if (! fault)
                    gpio_init (gpio, mode) ;

                break ;
            }


            case Service_read :
            {
                gpio_t  gpio ;
                uint8_t readValue ;

                fault = ! getGpio (request, & gpio) ;

                if (fault)
                    break ;

                readValue = gpio_read (gpio) ;

                fault = ! serviceBuffer_putUnsigned32 (response, readValue) ;

                break ;
            }


            case Service_write :
            {
                gpio_t  gpio ;
                uint8_t writeValue ;

                fault = ! getGpio (request, & gpio) ||
                        ! serviceBuffer_getByte (request, & writeValue) ;

                if (! fault)
                    gpio_write (gpio, writeValue) ;

                break ;
            }

        }

    return ! fault ;
}



#if 0

    GPIO_IN         // without pull
    GPIO_IN_PD      // with pull-down
    GPIO_IN_PU      // with pull-up

    GPIO_OUT        // push-pull
    GPIO_OD         // open-drain mode without pull resistor
    GPIO_OD_PU      // open-drain mode with    pull resistor

#endif

