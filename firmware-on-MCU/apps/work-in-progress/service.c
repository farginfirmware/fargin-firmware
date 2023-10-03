
#include "service.h"

#include "service-buffer.h"


#include "files.h"
#include "gpio.h"
#include "i2c.h"
#include "spi.h"
#include "test.h"
#include "time.h"
#include "uart.h"

static RequestProcessingFunction nextLevelProcessor [] = {

    // NOTE!! these are tightly coupled to variables in main.lua

    /* 0 */     test_processRequest,
    /* 1 */     time_processRequest,
    /* 2 */     gpio_processRequest,
    /* 3 */      spi_processRequest,
    /* 4 */      i2c_processRequest,
    /* 5 */     uart_processRequest,
    /* 6 */    files_processRequest,

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

