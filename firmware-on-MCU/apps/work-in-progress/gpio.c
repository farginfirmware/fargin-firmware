
#include "gpio.h"

#include "board.h"


bool gpio_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    (void) response ;

    uint8_t led ;
    uint8_t state ;

    bool fault = ! serviceBuffer_getByte (request, & led  ) ||
                 ! serviceBuffer_getByte (request, & state) ;

    if (! fault)
        switch (led)
        {
            case 0  :  state ? LED0_ON : LED0_OFF ;   break ;
            case 1  :  state ? LED1_ON : LED1_OFF ;   break ;
            case 2  :  state ? LED2_ON : LED2_OFF ;   break ;
            default :                                 break ;
        }

    return ! fault ;
}

