
#include "LED0.h"   // _LED0_H_

#include "board.h"


bool LED0_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    (void) response ;

  #if ! defined LED0_PIN
    return false ;
  #endif

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

