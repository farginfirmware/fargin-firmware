
#include "test.h"

#include "character.h"

#include <stdio.h>
#include <stdio_base.h>
#include <string.h>


// _SERVICE_BUFFER_H_


bool test_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    uint8_t subCommand ;

    bool fault = ! serviceBuffer_getByte (request, & subCommand) ;
    if (fault)
        return false ;


    switch (subCommand)
    {
        default :
            fault = true ;
            break ;

        case 0 :
        {
            // test string args and results

            while (true)        // break if end token
            {
                ServiceBufferToken nextToken = serviceBuffer_getNextToken (request) ;

                if (nextToken.type == ServiceBuffer_End)    break ;
                if (nextToken.type != ServiceBuffer_Bytes)  continue ;

                char * aString = (char *) nextToken.bytes.ptr ;

                stdio_write (aString, strlen (aString)) ;
                stdio_write ("\r\n", 2) ;

                // toggle the case of the 1st character
                if (* aString >= 'a')  * aString -= 0x20 ;
                else                   * aString += 0x20 ;

                fault = ! serviceBuffer_putString (response, aString) ;
            }

            break ;
        }

    }

    return ! fault ;
}

