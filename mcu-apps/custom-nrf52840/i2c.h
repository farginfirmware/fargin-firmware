#ifndef _I2C_H_
#define _I2C_H_

#include "project.h"

#include "service-buffer.h"


bool i2c_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;

void i2c_initialize (void) ;


#endif

