#ifndef _SERIAL_SERVICE_IO_H_
#define _SERIAL_SERVICE_IO_H_


#if 1
    #include "project.h"
#else
    #include <limits.h>
    #include <stdbool.h>
    #include <stdint.h>
    typedef double Real ;
#endif


#include "service-buffer.h"

typedef char (* RxFunctionPtr) (void) ;
typedef void (* TxFunctionPtr) (char) ;

bool serialService_receive  (ServiceBuffer *, RxFunctionPtr) ;
bool serialService_transmit (ServiceBuffer *, TxFunctionPtr, bool serviceResult) ;

#endif
