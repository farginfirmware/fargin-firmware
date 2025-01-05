
#include "ff.spi.h"     // _SPI_H_

#include "board.h"
#include "periph/spi.h"     // PERIPH_SPI_H


bool spi_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    bool fault ;

    (void) request ;
    (void) response ;

    fault = true ;

    // tbd

    return ! fault ;
}



void spi_initialize (void)
{
//  SPIDevice device = 0 ;
//  spi_init (device) ;         // is this done by the boot process??

    // tbd

}

