
#include "Lua.h"
#include "service.h"

#include "thread.h"

#include "file-system.h"
#include "usb.h"


    // services available to Lua

    #include "test.h"
    #include "ff.time.h"
    #include "ff.gpio.h"
    #include "ff.spi.h"
    #include "ff.i2c.h"
    #include "uart.h"
    #include "ff.file.h"

    static RequestProcessingFunction requestServers [] = {
        /// NOTE!! these are tightly coupled to definitions in main.lua
        /* 0 */     test_processRequest,
        /* 1 */     time_processRequest,
        /* 2 */     gpio_processRequest,
        /* 3 */      spi_processRequest,
        /* 4 */      i2c_processRequest,
        /* 5 */     uart_processRequest,
        /* 6 */     file_processRequest,
    } ;


int main (void)
{
    service_initialize (requestServers, ArrayLength (requestServers)) ;

    uart_initialize () ;


    // these may need to be reduced for microcontrollers with less code and data
    // space running small, simple Lua apps ("blinky" for example)
    //
    // increase these for bigger Lua apps on microcontrollers with more code and data space
    //
    const uint16_t LuaStackBytes =  4096 ;
    const uint32_t LuaHeapBytes  = 30000 ;

    Lua_initialize (LuaStackBytes, LuaHeapBytes) ;


    usb_initialize () ;

    fileSystem_initialize () ;

    thread_sleep () ;

    return 0 ;
}

