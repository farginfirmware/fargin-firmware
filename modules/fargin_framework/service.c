
#include "service.h"

#include "service-buffer.h"


static struct {
    RequestProcessingFunction * ptr ;
    uint8_t count ;
} servers = { NULL, 0 } ;


void service_initialize (RequestProcessingFunction * requestServerPtr, uint8_t numberOf)
{
    servers.ptr   = requestServerPtr ;
    servers.count = numberOf ;
}


bool service_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    serviceBuffer_reset (response) ;

    uint8_t subLevel ;

    bool fault = ! serviceBuffer_getByte (request, & subLevel) ||
                   (subLevel >= servers.count) ;

    if (! fault)
        fault |= ! servers.ptr [subLevel] (request, response) ;
    else
    {
        // write error to response buffer       tbd
    }

    serviceBuffer_reset (request) ;

    return ! fault ;
}

