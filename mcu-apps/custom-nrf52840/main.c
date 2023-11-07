
#include "Lua.h"
#include "service.h"

#include "thread.h"


#include "test.h"
#include "time.h"
#include "uart.h"
#include "i2c.h"

static RequestProcessingFunction requestServers [] = {
    /// NOTE!! these are tightly coupled to definitions in main.lua
    /*  0 */    test_processRequest,
    /*  1 */    time_processRequest,
    /*  2 */    uart_processRequest,
    /*  3 */     i2c_processRequest,
} ;


int main (void)
{
    uart_initialize () ;
     i2c_initialize () ;

    service_initialize (requestServers, ArrayLength (requestServers)) ;

    Lua_initialize () ;

    thread_sleep () ;

    return 0 ;
}

