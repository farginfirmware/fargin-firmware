#ifndef _LUA_H_
#define _LUA_H_

#include "project.h"


// notify Lua that there was an event that it may need to handle ;
// for example:
//    clock tick
//    timer expiration
//    gpio interrupt
//    uart rx
//    etc, etc
void Lua_notifyEvent (void) ;


int Lua_initialize (uint16_t stackBytes, uint32_t heapBytes) ;

#endif
