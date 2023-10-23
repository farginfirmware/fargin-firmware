
#include "Lua.h"
#include "thread.h"


int main (void)
{
    Lua_initialize () ;

    thread_sleep () ;

    return 0 ;
}

