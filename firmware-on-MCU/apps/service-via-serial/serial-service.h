#ifndef _SERIAL_SERVICE_IO_H_
#define _SERIAL_SERVICE_IO_H_

#include "project.h"

#include "service-buffer.h"

typedef char (* RxFunctionPtr) (void) ;
typedef void (* TxFunctionPtr) (char) ;

bool serialService_receive  (ServiceBuffer *, RxFunctionPtr) ;
bool serialService_transmit (ServiceBuffer *, TxFunctionPtr) ;

#endif

