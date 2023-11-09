
#include "spi.h"

#include "board.h"
#include "periph/spi.h"     // PERIPH_SPI_H

#include "service-buffer.h"



bool spi_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    bool fault = false ;

    (void) request ;
    (void) response ;

    fault = true ;




    return ! fault ;
}



void spi_initialize (void)
{


}

