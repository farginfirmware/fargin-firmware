
#include "Lua.h"
#include "file-system.h"
#include "thread.h"
#include "uart.h"
#include "usb.h"


int main (void)
{
    uart_initialize () ;

    Lua_initialize () ;

    usb_initialize () ;

    fileSystem_initialize () ;

    thread_sleep () ;

    return 0 ;
}

