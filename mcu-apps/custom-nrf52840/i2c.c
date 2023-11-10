
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



bool i2c_txStart (I2CDevice device, I2CAddressNumBits numAddressBits, uint16_t address, bool write)
{
    bool fault = (device != 0) ;

    uint8_t flags = I2C_NOSTOP ;

    switch (numAddressBits)
    {
        default :   return false ;
        case  7 :   break ;
        case 10 :   flags |= I2C_ADDR10 ;   break ;
    }

    address = (address << 1) | (write ? 0 : 1) ;

    uint8_t * dataPtr    = NULL ;
    uint8_t   dataLength = 0 ;

    // write the start sequence and the address ... no stop sequence
    fault |= ! (0 == i2c_write_bytes (device, address, dataPtr, dataLength, flags)) ;

    return ! fault ;
}



bool i2c_write (I2CDevice device, uint8_t * dataPtr, uint8_t dataLength, bool stop)
{
    bool fault = (device != 0) ;

    uint8_t flags   = I2C_NOSTART ;     // ignore address field
    uint8_t address = 0 ;

    if (! stop)
        flags |= I2C_NOSTOP ;

    fault |= ! (0 == i2c_write_bytes (device, address, dataPtr, dataLength, flags)) ;

    return ! fault ;
}



bool i2c_read (I2CDevice device, uint8_t * dataPtr, uint8_t dataLength, bool stop)
{
    bool fault = (device != 0) ;

    uint8_t flags   = I2C_NOSTART ;     // ignore address field
    uint8_t address = 0 ;

    if (! stop)
        flags |= I2C_NOSTOP ;

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
//  I2CDevice device = 0 ;
//  i2c_init (device) ;         // is this done by the boot process??

    // tbd

}

