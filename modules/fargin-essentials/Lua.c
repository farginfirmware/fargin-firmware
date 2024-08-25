
#include "Lua.h"

#include <errno.h>
#include <malloc.h>
#include <math.h>
#include <thread.h>

#include "lauxlib.h"
#include "lualib.h"
#include "lua_run.h"

#include "blob/main.lua.h"  // Lua startup code

#include "board.h"
#include "ff.time.h"
#include "service-buffer.h"
#include "service.h"


static void fatal_error (uint8_t blips)
{
    // provide visual error indication ;
    // there is no return from this function

    while (1)
    {
      #ifdef LED0_PIN
        uint8_t count = blips ;
        while (count --)
        {
            LED0_ON  ;  time_delayMilliseconds ( 25) ;
            LED0_OFF ;  time_delayMilliseconds (475) ;
        }
      #endif

        time_delayMilliseconds (1000) ;
    }
}


static uint8_t  requestBuffer [100] ;   // tbd size
static uint8_t responseBuffer [100] ;   // tbd size

static ServiceBuffer request, response ;


//  Any function registered with Lua must have the following prototype,
//  defined as lua_CFunction in lua.h:
//      typedef int (* lua_CFunction) (lua_State * L) ;



static bool getArg (lua_State * L, uint8_t argIndex)
{
    bool fault = false ;

    switch (lua_type (L, argIndex))
    {
        case LUA_TNUMBER :
        {
            double d = lua_tonumber (L, argIndex) ;
            double dAsInt = floor (d) ;

            if ((dAsInt == d) && (dAsInt >= 0) && (dAsInt <= UINT32_MAX))
                fault |= ! serviceBuffer_putUnsigned32 (& request, dAsInt) ;
            else
            if ((dAsInt == d) && (dAsInt >= INT32_MIN) && (dAsInt <= INT32_MAX))
                fault |= ! serviceBuffer_putSigned32 (& request, dAsInt) ;
            else
                fault |= ! serviceBuffer_putReal (& request, d) ;

            break ;
        }

        case LUA_TBOOLEAN :
            fault |= ! serviceBuffer_putBoolean (& request, lua_toboolean (L, argIndex)) ;
            break ;

        case LUA_TNIL :
            fault |= ! serviceBuffer_putNil (& request) ;
            break ;

        case LUA_TSTRING :
        {
            // tbd - this has to work for byte arrays and strings

            //    Any string that lua_tostring() returns always has a zero at its
            //    end, but it can have other zeros inside it. The lua_strlen()
            //    function returns the correct length of the string. In
            //    particular, assuming that the value at the top of the stack
            //    is a string, the following assertions are always valid:
            //
            //        const char * s = lua_tostring (L, argIndex) ;     // any Lua string
            //        size_t       l = lua_strlen   (L, argIndex) ;     // its length
            //        assert (s [l] == '\0') ;
            //        assert (strlen (s) <= l) ;

            uint8_t * dataPtr    = (uint8_t *) lua_tostring (L, argIndex) ;
            uint16_t  dataLength =             lua_rawlen   (L, argIndex) ;

            // Note!! dataLength does not include the terminating zero !

            fault |= ! serviceBuffer_putBytes (& request, dataPtr, dataLength) ;

            break ;
        }

        default :
            fault = true ;
            break ;
    }

    return ! fault ;
}



static void pushResponseToken (lua_State * L, ServiceBufferToken * token)
{
    switch (token->type)
    {
        default :
        case ServiceBuffer_Nil :        lua_pushnil     (L) ;                     break ;
        case ServiceBuffer_Boolean :    lua_pushboolean (L, token->boolean) ;     break ;
        case ServiceBuffer_Unsigned32 : lua_pushnumber  (L, token->unsigned32) ;  break ;
        case ServiceBuffer_Signed32 :   lua_pushnumber  (L, token->signed32) ;    break ;
        case ServiceBuffer_Real :       lua_pushnumber  (L, token->real) ;        break ;

        case ServiceBuffer_Bytes :
            lua_pushlstring (L, (char *) token->bytes.ptr, token->bytes.length) ;
            break ;
    }
}



static int service_request (lua_State * L)
{
    bool fault = false ;
    int  numberOfResults = 0 ;

    serviceBuffer_reset (& request) ;


    int numElementsOnStack = lua_gettop (L) ;
    int argIndex = 1 ;

    while (numElementsOnStack -- > 0)
        fault |= ! getArg (L, argIndex ++) ;

    lua_settop (L, 0) ;     // clear the stack


    fault |= ! service_processRequest (& request, & response) ;


    // push the result of the call to service_processRequest()
    lua_pushboolean (L, fault ? 0 : 1) ;
    ++ numberOfResults ;


    // push all elements of the response
    while (1)
    {
        ServiceBufferToken token = serviceBuffer_getNextToken (& response) ;

        if ((token.type == ServiceBuffer_End) ||
            (token.type >= ServiceBuffer_numberOfEntryTypes))
            break ;

        pushResponseToken (L, & token) ;
        ++ numberOfResults ;
    }


    return numberOfResults ;
}


static const struct {
    const char *   name ;
    lua_CFunction  functionPtr ;
} c_api [] = {
  //{  Lua function name,  C function name }
    { "service_request",   service_request },
} ;


static void C_from_Lua_initialize (lua_State * L)
{
    int index = ArrayLength (c_api) ;

    while (index --)
    {
        lua_pushcfunction (L, c_api[index].functionPtr) ;
        lua_setglobal     (L, c_api[index].name);
    }
}



static void * lua_thread (void * luaMemSizeArg)
{
    serviceBuffer_initialize (&  request,  requestBuffer, sizeof ( requestBuffer)) ;
    serviceBuffer_initialize (& response, responseBuffer, sizeof (responseBuffer)) ;


    uint32_t luaMemSize = (uint32_t) luaMemSizeArg ;

    char * lua_mem = malloc (luaMemSize) ;
    if (lua_mem == NULL)
        fatal_error (3) ;

    lua_State * L = lua_riot_newstate (lua_mem, luaMemSize, NULL) ;

    if  (L == NULL)
        // cannot create state: not enough memory
        fatal_error (4) ;


    lua_riot_openlibs (L,
                    // LUAR_LOAD_ALL     |
                       LUAR_LOAD_CORO    |      // coroutines
                    // LUAR_LOAD_DEBUG   |
                    // LUAR_LOAD_IO      |
                       LUAR_LOAD_MATH    |
                    // LUAR_LOAD_OS      |
                    // LUAR_LOAD_PACKAGE |
                       LUAR_LOAD_STRING  |
                       LUAR_LOAD_TABLE   |
                    // LUAR_LOAD_UTF8    |
                       LUAR_LOAD_BASE    ) ;


    C_from_Lua_initialize (L) ;


    luaL_loadbuffer (L, (const char *) main_lua, main_lua_len, "Lua startup code") ;

    if  (lua_pcall (L, 0, 0, 0) != LUA_OK)
        // Lua script running failed
        fatal_error (5) ;

    lua_close (L) ;

    // unexpected Lua termination
    fatal_error (6) ;

    return NULL ;
}


int Lua_initialize (uint16_t stackBytes, uint32_t heapBytes)
{
    uint8_t             priority =  THREAD_PRIORITY_IDLE - 1 ;  // lowest possible priority
    int                 flags    =  THREAD_CREATE_STACKTEST ;
    thread_task_func_t  task     =  lua_thread ;
    void *              arg      =  (void *) heapBytes ;
    const char *        name     = "Lua" ;

    char * stack = malloc (stackBytes) ;
    if (stack == NULL)
    {
        fatal_error (2) ;
        return -1 ;
    }

    kernel_pid_t pid = thread_create (stack, stackBytes, priority, flags, task, arg, name) ;

    return pid_is_valid (pid) ;
}

