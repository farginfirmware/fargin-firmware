
#include "service-buffer.h"

#include <math.h>
#include <string.h>


#if 0

    need to use a mutex?

#endif



static bool write (ServiceBuffer * svcBuf, uint8_t * dataPtr, uint16_t dataLength)
{
    // always append ServiceBuffer_End

    if (svcBuf == NULL)
        return false ;

    uint16_t bytesRemaining = svcBuf->bytesCapacity - svcBuf->bytesWritten ;

    uint8_t endMarker = ServiceBuffer_End ;

    if (dataLength + sizeof (endMarker) > bytesRemaining)
        return false ;

    memcpy (svcBuf->buffer + svcBuf->bytesWritten, dataPtr, dataLength) ;
    svcBuf->bytesWritten += dataLength ;

    * (svcBuf->buffer + svcBuf->bytesWritten) = endMarker ;

    return true ;
}



static bool serviceBuffer_put (ServiceBuffer * svcBuf,
                               uint8_t         dataType,
                               uint8_t *       dataPtr,
                               uint16_t        dataLength)
{
    if (svcBuf == NULL)
        return false ;

    uint16_t bytesWritten = svcBuf->bytesWritten ;

    bool fault = ! write (svcBuf, & dataType, sizeof (dataType)) ||
                 ! write (svcBuf, dataPtr, dataLength) ;

    if (fault)
    {
        // restore svcBuf to its state prior to calling this function
        svcBuf->bytesWritten = bytesWritten ;

        dataLength = 0 ;
        write (svcBuf, dataPtr, dataLength) ;
    }

    return ! fault ;
}


bool serviceBuffer_putNil (ServiceBuffer * svcBuf)
{
    uint8_t   dataType   = ServiceBuffer_Nil ;
    uint8_t * dataPtr    = NULL ;
    uint16_t  dataLength = 0 ;

    return serviceBuffer_put (svcBuf, dataType, dataPtr, dataLength) ;
}


bool serviceBuffer_putBoolean (ServiceBuffer * svcBuf, uint8_t data)
{
    uint8_t   dataType   = ServiceBuffer_Boolean ;
    uint8_t * dataPtr    = (uint8_t *) & data ;
    uint16_t  dataLength = sizeof (data) ;

    return serviceBuffer_put (svcBuf, dataType, dataPtr, dataLength) ;
}


bool serviceBuffer_putUnsigned32 (ServiceBuffer * svcBuf, uint32_t data)
{
    uint8_t   dataType   = ServiceBuffer_Unsigned32 ;
    uint8_t * dataPtr    = (uint8_t *) & data ;
    uint16_t  dataLength = sizeof (data) ;

    return serviceBuffer_put (svcBuf, dataType, dataPtr, dataLength) ;
}


bool serviceBuffer_putSigned32 (ServiceBuffer * svcBuf, int32_t data)
{
    uint8_t   dataType   = ServiceBuffer_Signed32 ;
    uint8_t * dataPtr    = (uint8_t *) & data ;
    uint16_t  dataLength = sizeof (data) ;

    return serviceBuffer_put (svcBuf, dataType, dataPtr, dataLength) ;
}


bool serviceBuffer_putReal (ServiceBuffer * svcBuf, Real data)
{
    uint8_t   dataType   = ServiceBuffer_Real ;
    uint8_t * dataPtr    = (uint8_t *) & data ;
    uint16_t  dataLength = sizeof (data) ;

    return serviceBuffer_put (svcBuf, dataType, dataPtr, dataLength) ;
}


bool serviceBuffer_putBytes (ServiceBuffer * svcBuf, uint8_t * dataPtr, uint16_t dataLength)
{
    /*
        This function is used to put both zero-terminated strings and byte arrays.

        Note!!  dataLength represents the actual amount of data
                i.e.
                    - either the number of bytes in the byte array or
                    - the number of bytes in the string not including the terminating zero

        This function has to append a terminating zero.
    */

    if (svcBuf == NULL)
        return false ;

    uint16_t bytesWritten = svcBuf->bytesWritten ;

    uint8_t dataType = ServiceBuffer_Bytes ;

    uint8_t  zero = 0 ;
    uint16_t dataLengthWithZero = dataLength + sizeof (zero) ;

    bool fault = ! write (svcBuf,             & dataType,           sizeof (dataType))           ||
                 ! write (svcBuf, (uint8_t *) & dataLengthWithZero, sizeof (dataLengthWithZero)) ||
                 ! write (svcBuf,               dataPtr,                    dataLength)          ||
                 ! write (svcBuf,             & zero,               sizeof (zero)) ;

    if (fault)
    {
        // restore svcBuf to its state prior to calling this function
        svcBuf->bytesWritten = bytesWritten ;

        dataLength = 0 ;
        write (svcBuf, dataPtr, dataLength) ;
    }

    return ! fault ;
}



bool serviceBuffer_appendByte (ServiceBuffer * svcBuf, uint8_t aByte)
{
    // this assumes that serviceBuffer_putBytes() was called immediately prior

    // overwrite the preceeding zero with aByte
    // append a new terminating zero
    // increment the data length

    if (svcBuf == NULL)
        return false ;

    bool fault = false ;

#if 1
    (void) aByte ;
#else

    // tbd




typedef struct {
    uint8_t  *  buffer ;
    uint16_t    bytesCapacity ;
    uint16_t    bytesWritten ;
    uint16_t    bytesRead ;
} ServiceBuffer ;





    if (fault)
    {
        // restore svcBuf to its state prior to calling this function
        svcBuf->bytesWritten = bytesWritten ;

        dataLength = 0 ;
        write (svcBuf, dataPtr, dataLength) ;
    }

#endif


    return ! fault ;
}



bool serviceBuffer_putString (ServiceBuffer * svcBuf, char * dataPtr)
{
    return serviceBuffer_putBytes (svcBuf, (uint8_t *) dataPtr, strlen (dataPtr)) ;
    // ... exclude the terminating zero
}



ServiceBufferToken serviceBuffer_getNextToken (ServiceBuffer * svcBuf)
{
    ServiceBufferToken data ;

    if (svcBuf == NULL)
    {
        data.type = ServiceBuffer_End ;
        return data ;
    }

    data.type = * (svcBuf->buffer + svcBuf->bytesRead) ;

    if (data.type == ServiceBuffer_End)
        return data ;

    ++ svcBuf->bytesRead ;

    uint8_t * dataPtr ;
    uint16_t  dataLength ;

    switch (data.type)
    {
        default :
            data.type = ServiceBuffer_End ;
            return data ;

        case ServiceBuffer_Unsigned32 :
            dataPtr    = (uint8_t *) & data.unsigned32 ;
            dataLength =       sizeof (data.unsigned32) ;
            memcpy (dataPtr, svcBuf->buffer + svcBuf->bytesRead, dataLength) ;
            svcBuf->bytesRead += dataLength ;
            break ;

        case ServiceBuffer_Signed32 :
            dataPtr    = (uint8_t *) & data.signed32 ;
            dataLength =       sizeof (data.signed32) ;
            memcpy (dataPtr, svcBuf->buffer + svcBuf->bytesRead, dataLength) ;
            svcBuf->bytesRead += dataLength ;
            break ;

        case ServiceBuffer_Real :
            dataPtr    = (uint8_t *) & data.real ;
            dataLength =       sizeof (data.real) ;
            memcpy (dataPtr, svcBuf->buffer + svcBuf->bytesRead, dataLength) ;
            svcBuf->bytesRead += dataLength ;
            break ;

        case ServiceBuffer_Bytes :          // tbd handle zero termination
            dataPtr    = (uint8_t *) & data.bytes.length ;
            dataLength =       sizeof (data.bytes.length) ;
            memcpy (dataPtr, svcBuf->buffer + svcBuf->bytesRead, dataLength) ;
            svcBuf->bytesRead += dataLength ;

            data.bytes.ptr = svcBuf->buffer + svcBuf->bytesRead ;
            svcBuf->bytesRead += data.bytes.length ;
          #if 1
            -- data.bytes.length ;          // exclude zero termination!!
          #endif

            break ;
    }

    return data ;
}



bool serviceBuffer_getUnsigned32 (ServiceBuffer * svcBuf, uint32_t * result)
{
    bool fault ;

    ServiceBufferToken nextToken = serviceBuffer_getNextToken (svcBuf) ;

    switch (nextToken.type)
    {
        default :
            fault = true ;
            break ;

        case ServiceBuffer_Unsigned32 :
            fault = false ;
            * result = nextToken.unsigned32 ;
            break ;

        case ServiceBuffer_Signed32 :
            fault = (nextToken.signed32 < 0) ;
            if (! fault)
                * result = nextToken.signed32 ;
            break ;

        case ServiceBuffer_Real :
            fault = (floor (nextToken.real) != nextToken.real) ||
                    (nextToken.real < 0) || (nextToken.real > UINT32_MAX) ;
            if (! fault)
                * result = nextToken.real ;
            break ;
    }

    return ! fault ;
}


bool serviceBuffer_getByte (ServiceBuffer * svcBuf, uint8_t * result)
{
    uint32_t unsigned32 ;
    bool fault = ! serviceBuffer_getUnsigned32 (svcBuf, & unsigned32) ||
                   (unsigned32 > UINT8_MAX) ;
    if (! fault)
        * result = unsigned32 ;

    return ! fault ;
}


bool serviceBuffer_getSigned32 (ServiceBuffer * svcBuf, int32_t * result)
{
    bool fault ;

    ServiceBufferToken nextToken = serviceBuffer_getNextToken (svcBuf) ;

    switch (nextToken.type)
    {
        default :
            fault = true ;
            break ;

        case ServiceBuffer_Unsigned32 :
            fault = nextToken.unsigned32 > INT32_MAX ;
            if (! fault)
                * result = nextToken.unsigned32 ;
            break ;

        case ServiceBuffer_Signed32 :
            fault = false ;
            * result = nextToken.signed32 ;
            break ;

        case ServiceBuffer_Real :
            fault = (floor (nextToken.real) != nextToken.real) ||
                    (nextToken.real < INT32_MIN) || (nextToken.real > INT32_MAX) ;
            if (! fault)
                * result = nextToken.real ;
            break ;
    }

    return ! fault ;
}


bool serviceBuffer_getReal (ServiceBuffer * svcBuf, Real * result)
{
    bool fault = false ;

    ServiceBufferToken nextToken = serviceBuffer_getNextToken (svcBuf) ;

    switch (nextToken.type)
    {
        default :                         fault = true ;                    break ;
        case ServiceBuffer_Unsigned32 : * result = nextToken.unsigned32 ;   break ;
        case ServiceBuffer_Signed32   : * result = nextToken.  signed32 ;   break ;
        case ServiceBuffer_Real       : * result = nextToken.real ;         break ;
    }

    return ! fault ;
}



bool serviceBuffer_getString (ServiceBuffer * svcBuf, char * * addressOfCharPtr)
{
    bool fault = false ;

    ServiceBufferToken nextToken = serviceBuffer_getNextToken (svcBuf) ;

    switch (nextToken.type)
    {
        default :
            fault = true ;
            break ;

        case ServiceBuffer_Bytes :
            * addressOfCharPtr = (char *) nextToken.bytes.ptr ;
            break ;
    }

    return ! fault ;
}



bool serviceBuffer_getBytesCopy (ServiceBuffer * svcBuf, uint8_t * dataPtr, uint16_t dataLength)
{
    bool fault = false ;

    ServiceBufferToken nextToken = serviceBuffer_getNextToken (svcBuf) ;

    switch (nextToken.type)
    {
        default :
            fault = true ;
            break ;

        case ServiceBuffer_Bytes :
        {
            ServiceBuffer_ByteArray bytes = nextToken.bytes ;

            fault = ! (bytes.length == dataLength) ;

            if (! fault)
                memcpy (dataPtr, bytes.ptr, dataLength) ;

            break ;
        }
    }

    return ! fault ;
}



void serviceBuffer_reset (ServiceBuffer * svcBuf)
{
    if (svcBuf == NULL)
        return ;

    svcBuf->bytesWritten = 0 ;
    svcBuf->bytesRead    = 0 ;

    uint8_t * dataPtr = NULL ;
    uint8_t   dataLength = 0 ;
    write (svcBuf, dataPtr, dataLength) ;
}



void serviceBuffer_initialize (ServiceBuffer * svcBuf, uint8_t * buffer, uint16_t capacity)
{
    svcBuf->buffer        = buffer ;
    svcBuf->bytesCapacity = capacity ;

    serviceBuffer_reset (svcBuf) ;
}

