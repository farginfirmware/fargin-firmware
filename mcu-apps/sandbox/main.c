
#include "Lua.h"
#include "service.h"

#include "thread.h"

// RequestProcessingFunction
#include "test.h"
#include "ff.time.h"
#include "gpio.h"
#include "ff.spi.h"
#include "ff.i2c.h"
#include "uart.h"
#include "ff.file.h"

#include "file-system.h"
#include "usb.h"


static RequestProcessingFunction requestServers [] = {
    /// NOTE!! these are tightly coupled to definitions in main.lua
    /*  0 */          test_processRequest,
    /*  1 */          time_processRequest,
    /*  2 */          gpio_processRequest,
    /*  3 */           spi_processRequest,
    /*  4 */           i2c_processRequest,
    /*  5 */          uart_processRequest,
    /*  6 */          file_processRequest,
} ;


int main (void)
{
    service_initialize (requestServers, ArrayLength (requestServers)) ;

    uart_initialize () ;

    Lua_initialize () ;

    usb_initialize () ;

    fileSystem_initialize () ;

    thread_sleep () ;

    return 0 ;
}

