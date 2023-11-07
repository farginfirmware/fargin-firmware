#ifndef _TIME_H_
#define _TIME_H_

#include "project.h"

#include "service-buffer.h"


bool time_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;

void time_delayMilliseconds (uint16_t milliseconds) ;


#endif
