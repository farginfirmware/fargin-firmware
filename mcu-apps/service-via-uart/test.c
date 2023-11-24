
#include "test.h"


bool test_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    uint8_t subCommand ;

    bool fault = ! serviceBuffer_getByte (request, & subCommand) ;

    if (! fault)
        switch (subCommand)
        {
            default :   fault = true ;  break ;

            case 0 :    fault |= ! serviceBuffer_putUnsigned32 (response, 2000) ;   break ;
            case 1 :    fault |= ! serviceBuffer_putReal       (response, 0.7) ;    break ;

            case 2 :    fault |= ! serviceBuffer_putUnsigned32 (response, 1000) ;   break ;
            case 3 :    fault |= ! serviceBuffer_putReal       (response, 0.3) ;    break ;
        }

    return ! fault ;
}

