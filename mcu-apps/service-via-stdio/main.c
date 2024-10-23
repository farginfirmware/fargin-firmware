
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

    static RequestProcessingFunction requestServers [] = {
        /// NOTE!! these are tightly coupled to definitions in main.lua
        /* 0 */     time_processRequest,
        /* 1 */     LED0_processRequest,
        /* 2 */     BTN0_processRequest,
    } ;


int main (void)
{
    service_initialize (requestServers, ArrayLength (requestServers)) ;

    // this example isn't really about Lua, but we're going to let it run
    // anyway in parallel with serial service
    const uint16_t LuaStackBytes =  3000 ;
    const uint32_t LuaHeapBytes  = 30000 ;
    Lua_initialize (LuaStackBytes, LuaHeapBytes) ;

    // serial service is what this example is really about
    serviceViaStdio_initialize () ;

    thread_sleep () ;

    return 0 ;
}

