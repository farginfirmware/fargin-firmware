
#include "gpio.h"

#include "board.h"
#include "periph/gpio.h"

#include <string.h>


// these constants are tightly coupled to Lua variables

    #define Service_getHandle   0
    #define Service_configure   1
    #define Service_read        2
    #define Service_write       3

    #define InputType_noPull    0
    #define InputType_pullUp    1
    #define InputType_pullDown  2

    #define OutputType_pushPull                 0
    #define OutputType_openDrain                1
    #define OutputType_openDrainPullupEnabled   2



static bool getGpio (ServiceBuffer * request, gpio_t * gpioPtr)
{
  #if 1
    return serviceBuffer_getBytes (request, (uint8_t *) gpioPtr, sizeof (gpio_t)) ;
  #else
    ServiceBufferToken token = serviceBuffer_getNextToken (request) ;

    bool fault = ! (token.type == ServiceBuffer_Bytes) ||
                 ! (token.bytes.length == sizeof (* gpioPtr)) ;

    if (! fault)
        memcpy (gpioPtr, token.bytes.ptr, sizeof (* gpioPtr)) ;

    return ! fault ;
  #endif
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


                // tbd


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

