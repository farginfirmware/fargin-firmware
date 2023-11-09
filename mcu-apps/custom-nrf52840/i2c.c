
#include "i2c.h"

#include "board.h"
#include "periph/i2c.h"     // PERIPH_I2C_H


#if 0

int i2c_read_byte   (i2c_t dev, uint16_t addr, void *  data, uint8_t flags) ;
int i2c_write_byte  (i2c_t dev, uint16_t addr, uint8_t data, uint8_t flags) ;

int i2c_read_bytes  (i2c_t dev, uint16_t addr, void *  data, size_t len, uint8_t flags) ;
int i2c_write_bytes (i2c_t dev, uint16_t addr, void *  data, size_t len, uint8_t flags) ;

typedef enum {
    I2C_ADDR10      use 10-bit device addressing
    I2C_REG16       use 16-bit register addressing, big-endian
    I2C_NOSTOP      do not issue a STOP condition after transfer
    I2C_NOSTART     skip START sequence, ignore address field
} i2c_flags_t;

#endif



void i2c_open (I2CDevice device)
{
    if (device != 0)
        return ;

    i2c_acquire (device) ;
}


void i2c_close (I2CDevice device)
{
    if (device != 0)
        return ;

    i2c_release (device) ;
}



bool i2c_sendStart (I2CDevice device, uint8_t _7bitAddress, bool write_readNot)
{
    bool fault = (device != 0) ;

    uint8_t startByte = (_7bitAddress << 1) | (write_readNot ? 0 : 1) ;

    uint8_t flags = I2C_NOSTOP ;

    uint8_t * dataPtr    = NULL ;
    uint8_t   dataLength = 0 ;

    fault |= ! (0 == i2c_write_bytes (device, startByte, dataPtr, dataLength, flags)) ;

    return ! fault ;
}


bool i2c_sendStop (I2CDevice device)
{
    bool fault = (device != 0) ;

    uint8_t flags   = I2C_NOSTART ;
    uint8_t address = 0 ;   // this is n/a

    uint8_t * dataPtr    = NULL ;
    uint8_t   dataLength = 0 ;

    fault |= ! (0 == i2c_write_bytes (device, address, dataPtr, dataLength, flags)) ;

    return ! fault ;
}


bool i2c_writeByte (I2CDevice device, uint8_t aByte)
{
    bool fault = (device != 0) ;

    uint8_t flags   = I2C_NOSTART | I2C_NOSTOP ;

    uint8_t address = 0 ;   // this is n/a

    uint8_t *  dataPtr    = & aByte ;
    uint8_t    dataLength = sizeof (aByte) ;

    fault |= ! (0 == i2c_write_bytes (device, address, dataPtr, dataLength, flags)) ;

    return ! fault ;
}


bool i2c_readByte (I2CDevice device, uint8_t * bytePtr, bool withAck)
{
    bool fault = (device != 0) ;

    uint8_t flags = I2C_NOSTART ;
    if (withAck)
        flags |= I2C_NOSTOP ;

    uint8_t address = 0 ;   // this is n/a

    uint8_t *  dataPtr    = bytePtr ;
    uint8_t    dataLength = sizeof (* bytePtr) ;

    fault |= ! (0 == i2c_read_bytes (device, address, dataPtr, dataLength, flags)) ;

    return ! fault ;
}



bool i2c_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    bool fault ;

    (void) request ;
    (void) response ;

    fault = true ;

    // tbd

    return ! fault ;
}



void i2c_initialize (void)
{
    I2CDevice device = 0 ;
    i2c_init (device) ;         // this is done by the boot process??

    // tbd

}

