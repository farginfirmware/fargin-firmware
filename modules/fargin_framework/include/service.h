#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "project.h"

#include "service-buffer.h"


typedef bool (* RequestProcessingFunction) (ServiceBuffer * request, ServiceBuffer * response) ;


// this is the top level function for processing ServiceBuffer request/response
//
bool service_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;


void service_initialize (RequestProcessingFunction *, uint8_t numberOf) ;


#endif

