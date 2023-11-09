#ifndef _SPI_H_
#define _SPI_H_

#include "project.h"

#include "service-buffer.h"


bool spi_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;

void spi_initialize (void) ;


#endif

