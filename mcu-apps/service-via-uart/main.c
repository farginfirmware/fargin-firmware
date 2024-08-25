
#include "Lua.h"
#include "thread.h"
#include "uart.h"


#if 0

#include "service-buffer.h"
#include "serial-service.h"

typedef char (* RxFunctionPtr) (void) ;
typedef void (* TxFunctionPtr) (char) ;

bool serialService_receive  (ServiceBuffer *, RxFunctionPtr) ;
bool serialService_transmit (ServiceBuffer *, TxFunctionPtr) ;

static uint8_t  requestBuffer [100] ;   // tbd size
static uint8_t responseBuffer [100] ;   // tbd size

static ServiceBuffer request, response ;
void serviceBuffer_initialize (ServiceBuffer *, uint8_t * buffer, uint16_t capacity) ;


    fault |= ! service_processRequest (& request, & response) ;

#endif


    // services available to Lua

    #include "ff.time.h"
    #include "ff.gpio.h"

    static RequestProcessingFunction requestServers [] = {
        /// NOTE!! these are tightly coupled to definitions in main.lua
        /* 0 */     time_processRequest,
        /* 1 */     gpio_processRequest,
    } ;


int main (void)
{
    uart_initialize () ;


    const uint16_t LuaStackBytes =  4096 ;
    const uint32_t LuaHeapBytes  = 30000 ;
    Lua_initialize (LuaStackBytes, LuaHeapBytes) ;


    thread_sleep () ;

    return 0 ;
}

