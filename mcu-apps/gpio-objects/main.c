
#include "Lua.h"
#include "service.h"

#include "thread.h"


    // services available to Lua

    #include "ff.time.h"
    #include "LED0.h"
    #include "BTN0.h"
    #include "ff.gpio.h"

    static RequestProcessingFunction requestServers [] = {
        /// NOTE!! these are tightly coupled to definitions in main.lua
        /* 0 */     time_processRequest,
        /* 1 */     LED0_processRequest,
        /* 2 */     BTN0_processRequest,
        /* 3 */     gpio_processRequest,
    } ;


int main (void)
{
    service_initialize (requestServers, ArrayLength (requestServers)) ;

    const uint16_t LuaStackBytes =  3000 ;
    const uint32_t LuaHeapBytes  = 30000 ;
    Lua_initialize (LuaStackBytes, LuaHeapBytes) ;

    // in this application, the only thread that needs to run is Lua
    thread_sleep () ;

    return 0 ;
}

