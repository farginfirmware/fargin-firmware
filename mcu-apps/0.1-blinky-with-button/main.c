
#include "Lua.h"
#include "service.h"

#include "thread.h"


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

    Lua_initialize () ;

    // in this application, the only thread that needs to run is Lua
    thread_sleep () ;

    return 0 ;
}

