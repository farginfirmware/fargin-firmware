
#include "Lua.h"

#include "thread.h"


int main (void)
{

    // these amounts may need to be adjusted
    //  - increase for greater Lua code complexity
    //  - reduce for smaller MCUs with less flash and ram
    const uint16_t LuaStackBytes =  4096 ;
    const uint32_t LuaHeapBytes  = 30000 ;      // increased for Lua co-routines
    Lua_initialize (LuaStackBytes, LuaHeapBytes) ;

    // in this application, the only thread that needs to run is Lua
    thread_sleep () ;

    return 0 ;
}

