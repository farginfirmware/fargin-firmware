#ifndef _FILE_H_
#define _FILE_H_

#include "project.h"

#include "service-buffer.h"


bool file_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;

void file_initialize (void) ;

#endif
