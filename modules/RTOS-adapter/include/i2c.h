#ifndef _I2C_H_
#define _I2C_H_

#include "project.h"

#include "service-buffer.h"


typedef uint8_t I2CDevice ;
typedef uint8_t I2CAddressNumBits ;


void i2c_open  (I2CDevice) ;
void i2c_close (I2CDevice) ;

bool i2c_startThenWrite (I2CDevice, I2CAddressNumBits, uint16_t address, uint8_t * dataPtr, uint8_t dataLength, bool stop) ;
bool i2c_startThenRead  (I2CDevice, I2CAddressNumBits, uint16_t address, uint8_t * dataPtr, uint8_t dataLength, bool stop) ;

bool i2c_write          (I2CDevice,                                      uint8_t * dataPtr, uint8_t dataLength, bool stop) ;
bool i2c_read           (I2CDevice,                                      uint8_t * dataPtr, uint8_t dataLength, bool stop) ;


bool i2c_processRequest (ServiceBuffer * request, ServiceBuffer * response) ;

void i2c_initialize (void) ;

#endif
