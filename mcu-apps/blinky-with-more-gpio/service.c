
#include "service.h"

#include "service-buffer.h"


#include "LED0.h"
#include "gpio.h"
#include "time.h"

static RequestProcessingFunction nextLevelProcessor [] = {

    // NOTE!! these are tightly coupled to variables in main.lua

    /* 0 */     LED0_processRequest,
    /* 1 */     time_processRequest,
    /* 2 */     gpio_processRequest,

} ;



bool service_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    serviceBuffer_reset (response) ;

    uint8_t subLevel ;

    bool fault = ! serviceBuffer_getByte (request, & subLevel) ||
                   (subLevel >= ArrayLength (nextLevelProcessor)) ;

    if (! fault)
        fault |= ! nextLevelProcessor [subLevel] (request, response) ;
    else
    {
        // write error to response buffer       tbd
    }

    serviceBuffer_reset (request) ;

    return ! fault ;
}

