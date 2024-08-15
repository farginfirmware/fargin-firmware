
#include "Lua.h"
#include "service.h"

#include "thread.h"


#include "test.h"
#include "ff.time.h"
#include "gpio.h"

static RequestProcessingFunction requestServers [] = {
    /// NOTE!! these are tightly coupled to definitions in main.lua
    /*  0 */          test_processRequest,
    /*  1 */          time_processRequest,
    /*  2 */          gpio_processRequest,
} ;


int main (void)
{
    service_initialize (requestServers, ArrayLength (requestServers)) ;

    Lua_initialize () ;

    thread_sleep () ;

    return 0 ;
}

