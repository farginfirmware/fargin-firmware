
#include "Lua.h"
#include "service.h"

#include "thread.h"

#include "file-system.h"


    // services available to Lua

    #include "test.h"
    #include "ff.time.h"
    #include "ff.gpio.h"
    #include "ff.i2c.h"
    #include "uart.h"
    #include "ff.file.h"

    static RequestProcessingFunction requestServers [] = {
        /// NOTE!! these are tightly coupled to definitions in main.lua
        /* 0 */     test_processRequest,
        /* 1 */     time_processRequest,
        /* 2 */     gpio_processRequest,
        /* 3 */      i2c_processRequest,
        /* 4 */     uart_processRequest,
        /* 5 */     file_processRequest,
    } ;


int main (void)
{
    service_initialize (requestServers, ArrayLength (requestServers)) ;

    uart_initialize () ;


    const uint16_t  LuaStackBytes =  4096 ;
    const uint32_t  LuaHeapBytes  = 30000 ;
    Lua_initialize (LuaStackBytes, LuaHeapBytes) ;


    fileSystem_initialize () ;

    thread_sleep () ;

    return 0 ;
}

