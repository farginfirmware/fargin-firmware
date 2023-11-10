
#include "Lua.h"
#include "service.h"

#include "thread.h"


#include "test.h"
#include "time.h"
#include "uart.h"
#include "i2c.h"
#include "spi.h"
#include "io-expander.h"

static RequestProcessingFunction requestServers [] = {
    /// NOTE!! these are tightly coupled to definitions in main.lua
    /*  0 */          test_processRequest,
    /*  1 */          time_processRequest,
    /*  2 */          uart_processRequest,
    /*  3 */           i2c_processRequest,
    /*  4 */           spi_processRequest,
    /*  5 */    ioExpander_processRequest,
} ;


int main (void)
{
    uart_initialize () ;
     i2c_initialize () ;
     spi_initialize () ;

    service_initialize (requestServers, ArrayLength (requestServers)) ;

    // must be preceded by i2c_initialize() and service_initialize()
    ioExpander_initialize () ;

    Lua_initialize () ;

    while (1)
    {
        uint8_t led_RED  = 13 ;
        ioExpander_setBit (IOExpander_U22, led_RED, 1) ;    time_delayMilliseconds ( 25) ;
        ioExpander_setBit (IOExpander_U22, led_RED, 0) ;    time_delayMilliseconds (475) ;
    }

    thread_sleep () ;

    return 0 ;
}

