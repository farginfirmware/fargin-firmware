#ifndef _UART_H_
#define _UART_H_

#include "project.h"

#include "service-buffer.h"


bool uart_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;


int uart_initialize (void) ;


#endif

