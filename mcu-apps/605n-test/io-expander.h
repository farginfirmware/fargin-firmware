#ifndef _IO_EXPANDER_H_
#define _IO_EXPANDER_H_

#include "project.h"

#include "service-buffer.h"


typedef enum {
    IOExpander_U2,
    IOExpander_U22,
    NumberOfIOExpanders
} IOExpander ;


uint8_t ioExpander_getBit (IOExpander, uint8_t bitNumber) ;
void    ioExpander_setBit (IOExpander, uint8_t bitNumber, uint8_t _0_or_1) ;

bool ioExpander_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;

void ioExpander_initialize (void) ;

#endif
