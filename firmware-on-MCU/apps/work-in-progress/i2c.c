
#include "i2c.h"

#include "board.h"
#include "periph/i2c.h"

#include "service-buffer.h"



bool i2c_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    bool fault = false ;


    (void) request ;
    (void) response ;


    fault = true ;






    return ! fault ;
}

