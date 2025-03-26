
#include "Lua.h"
#include "service.h"
#include "service-via-stdio.h"

#include "thread.h"


    // services available to:
    //   Lua
    //   serviceViaStdio

    #include "ff.time.h"
    #include "LED0.h"
    #include "BTN0.h"
    #include "ff.gpio.h"
    #include "test.h"

    static RequestProcessingFunction requestServers [] = {
        /// NOTE!! these are tightly coupled to definitions in main.lua
        /* 0 */     time_processRequest,
        /* 1 */     LED0_processRequest,
        /* 2 */     BTN0_processRequest,
        /* 3 */     gpio_processRequest,
        /* 4 */     test_processRequest,
    } ;


int main (void)
{
    service_initialize (requestServers, ArrayLength (requestServers)) ;

    const uint16_t LuaStackBytes =  3000 ;
    const uint32_t LuaHeapBytes  = 30000 ;
    Lua_initialize (LuaStackBytes, LuaHeapBytes) ;

    // serial service enables development and test from a PC
    serviceViaStdio_initialize () ;

    thread_sleep () ;

    return 0 ;
}

