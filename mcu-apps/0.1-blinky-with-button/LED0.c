
#include "LED0.h"

#include "board.h"


bool LED0_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    (void) response ;

    uint8_t state ;

    bool fault = ! serviceBuffer_getByte (request, & state) ;

    if (! fault)
        switch (state)
        {
            case 0  :  LED0_OFF ;   break ;
            default :  LED0_ON  ;   break ;     // any non-0 value turns the LED on
        }

    return ! fault ;
}

