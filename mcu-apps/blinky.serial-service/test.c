
#include "test.h"

#include "character.h"

#include <stdio.h>
#include <stdio_base.h>
#include <string.h>


    // these constants are tightly coupled to Lua variables
    //
    #define   EchoString    0       // echo the string(s) in the request buffer
    #define     RxString    1       // make a local copy of the string in the request buffer
    #define ReadRxString    2       // return the local copy

static uint8_t       rxData [500] ;    // increase if necessary
static ServiceBuffer rxBuffer ;
static bool          rxInitialized = false ;


bool test_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    if (! rxInitialized)
    {
        serviceBuffer_initialize (& rxBuffer, rxData, sizeof (rxData)) ;
        rxInitialized = true ;
    }

    uint8_t subCommand ;

    bool fault = ! serviceBuffer_getByte (request, & subCommand) ;
    if (fault)
        return false ;


    switch (subCommand)
    {
        default :
            fault = true ;
            break ;


        case EchoString :
        {
            // echo all string args with the case of the 1st character toggled

            while (true)        // break if end token
            {
                ServiceBufferToken nextToken = serviceBuffer_getNextToken (request) ;

                if (nextToken.type == ServiceBuffer_End)    break ;
                if (nextToken.type != ServiceBuffer_Bytes)  continue ;

                char * aString = (char *) nextToken.bytes.ptr ;

            #if 0
                stdio_write (aString, strlen (aString)) ;
                stdio_write ("\r\n", 2) ;
            #endif

                // toggle the case of the 1st character
                if (* aString >= 'a')  * aString -= 0x20 ;
                else                   * aString += 0x20 ;

                fault = ! serviceBuffer_putString (response, aString) ;
            }

            break ;
        }


        case RxString :
        {
            // make a copy the given string

            // get it from the request buffer
            ServiceBufferToken nextToken = serviceBuffer_getNextToken (request) ;

            if (nextToken.type != ServiceBuffer_Bytes)
            {
                fault = true ;
                break ;
            }

            char * aString = (char *) nextToken.bytes.ptr ;

            // copy it to the local service buffer
            serviceBuffer_reset (& rxBuffer) ;
            fault = ! serviceBuffer_putString (& rxBuffer, aString) ;

            break ;
        }


        case ReadRxString :
        {
            // return the string that was copied in the RxString case ;
            // then clear the RxString

            // get it from the local service buffer
            ServiceBufferToken nextToken = serviceBuffer_getNextToken (& rxBuffer) ;

            if (nextToken.type != ServiceBuffer_Bytes)
            {
                fault = true ;
                break ;
            }

            char * aString = (char *) nextToken.bytes.ptr ;

            // copy it to the response buffer
            fault = ! serviceBuffer_putString (response, aString) ;

            break ;
        }

    }

    return ! fault ;
}

