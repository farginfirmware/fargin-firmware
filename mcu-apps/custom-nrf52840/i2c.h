#ifndef _I2C_H_
#define _I2C_H_

#include "project.h"

#include "service-buffer.h"

typedef uint8_t I2CDevice ;

void i2c_open      (I2CDevice) ;
void i2c_close     (I2CDevice) ;
bool i2c_sendStart (I2CDevice, uint8_t _7bitAddress, bool write_readNot) ;
bool i2c_sendStop  (I2CDevice) ;
bool i2c_writeByte (I2CDevice, uint8_t) ;
bool i2c_readByte  (I2CDevice, uint8_t *, bool withAck) ;

bool i2c_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;

void i2c_initialize (void) ;

#endif
