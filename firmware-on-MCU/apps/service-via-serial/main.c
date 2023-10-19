
#include "Lua.h"
#include "thread.h"
#include "uart.h"


int main (void)
{
    uart_initialize () ;

    Lua_initialize () ;

    thread_sleep () ;

    return 0 ;
}

