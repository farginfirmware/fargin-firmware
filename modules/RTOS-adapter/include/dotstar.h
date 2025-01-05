#ifndef _DOTSTAR_H_
#define _DOTSTAR_H_

#include "project.h"

#include "service-buffer.h"

void dotstar_initialize (void) ;

bool dotstar_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;

#endif

