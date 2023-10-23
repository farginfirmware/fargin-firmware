#ifndef _SERIAL_SERVICE_IO_H_
#define _SERIAL_SERVICE_IO_H_

#include "project.h"

#include "service-buffer.h"


typedef struct {

    uint8_t  requestBuffer [1000] ;      // tbd size
    uint8_t responseBuffer [1000] ;      // tbd size

    ServiceBuffer request ;
    ServiceBuffer response ;

    char (* rxFnPtr) (void) ;
    void (* txFnPtr) (char) ;

} SerialServiceObject ;


bool serialService_initialize (SerialServiceObject *) ; // SerialServiceObject must be static!!


#endif

