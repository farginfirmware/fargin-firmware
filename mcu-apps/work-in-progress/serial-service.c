
#include "serial-service.h"


#if 0

    multiple instances

    each instance
        thread
            receive command
            invoke service
            transmit response


serial ascii examples (stdio, for instance)

    uX.....XX           32-bit Unsigned integer (1 to 8 hex nybbles)
    sX.....XX           32-bit   Signed integer (1 to 8 hex nybbles
                                                 bit 3 of first nybble is the sign bit)
    dXXXXXXXXXXXXXXXX   64-bit Double  (16 hex nybbles exactly)
    bXX...XXXX          Byte array (even number of hex nybbles)


request format
    uX..XX              receiver
    arg                 any of [u|s|d|b]
    ...
    arg                 any of [u|s|d|b]
    \r                  end of request


#endif



