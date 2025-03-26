
#include "service-buffer.h"     // _SERVICE_BUFFER_H_

#include <math.h>
#include <string.h>


#if 0

    need to use a mutex?


    byte array (including strings)
        token               ServiceBuffer_Bytes
        2-byte length       excluding zero termination
        0                   zero termination
        end token

    empty byte array (including strings)
        ServiceBuffer_Bytes
        0                   length
        0                   zero termination
        ServiceBuffer_End

    5-byte array (including strings)
        ServiceBuffer_Bytes
        5                   length
        byte 1
        byte 2
        byte 3
        byte 4
        byte 5
        0                   zero termination
        ServiceBuffer_End

#endif



static bool write (ServiceBuffer * svcBuf, uint8_t * dataPtr, uint16_t dataLength)
{
    // always append ServiceBuffer_End

  #if 0
    // svcBuf is assumed not to be NULL
    if (svcBuf == NULL)
        return false ;
  #endif

    uint16_t bytesRemaining = svcBuf->bytesCapacity - svcBuf->bytesWritten ;

    uint8_t endMarker = ServiceBuffer_End ;

    if (dataLength + sizeof (endMarker) > bytesRemaining)
        return false ;

    if (dataLength > 0)
    {
        memcpy (svcBuf->buffer + svcBuf->bytesWritten, dataPtr, dataLength) ;
        svcBuf->bytesWritten += dataLength ;
    }

    svcBuf->buffer [svcBuf->bytesWritten] = endMarker ;

    return true ;
}



static bool serviceBuffer_put (ServiceBuffer * svcBuf,
                               uint8_t         dataType,
                               uint8_t *       dataPtr,
                               uint16_t        dataLength)
{
    // put any token except ServiceBuffer_Bytes

    if (svcBuf == NULL)
        return false ;

    uint16_t previousBytesWritten = svcBuf->bytesWritten ;
    svcBuf->lastTokenOffset = previousBytesWritten ;    // set this every time a new token is added

    bool fault = ! write (svcBuf, & dataType, sizeof (dataType)) ||
                 ! write (svcBuf,   dataPtr,          dataLength) ;

    if (fault)
    {
        // restore svcBuf to its state prior to calling this function
        svcBuf->bytesWritten = previousBytesWritten ;
        write (svcBuf, NULL, 0) ;
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



static bool createEmptyByteArray (ServiceBuffer * svcBuf)
{
    // put a ServiceBuffer_Bytes token with a byte array length of 0
    // i.e. an empty byte array

    // there should be a ServiceBuffer_End token at this offset
    uint16_t previousBytesWritten = svcBuf->bytesWritten ;
    svcBuf->lastTokenOffset = previousBytesWritten ;

    uint8_t  dataType        = ServiceBuffer_Bytes ;
    uint16_t dataLength      = 0 ;
    uint8_t  terminatingZero = 0 ;

    bool fault = ! write (svcBuf,             & dataType,         sizeof (dataType))   ||
                 ! write (svcBuf, (uint8_t *) & dataLength,       sizeof (dataLength)) ||
                 ! write (svcBuf,             & terminatingZero,  sizeof (terminatingZero)) ;

    if (fault)
    {
        // restore svcBuf to its state prior to calling this function
        svcBuf->bytesWritten = previousBytesWritten ;
        write (svcBuf, NULL, 0) ;   // rewrite the ServiceBuffer_End token
    }

    return ! fault ;
}



static bool appendBytes (ServiceBuffer * svcBuf, uint8_t * dataPtr, uint16_t dataLength)
{
    if (dataLength == 0)
        return true ;

    // fail if the last token was not ServiceBuffer_Bytes

    uint16_t lastTokenOffset = svcBuf->lastTokenOffset ;
    uint8_t  lastToken       = svcBuf->buffer [lastTokenOffset] ;

    if (lastToken != ServiceBuffer_Bytes)
        return false ;

    // fail if not enough space to append data
    uint16_t bytesRemaining = svcBuf->bytesCapacity - svcBuf->bytesWritten ;
    uint8_t endMarker = ServiceBuffer_End ;
    if (dataLength + sizeof (endMarker) > bytesRemaining)
        return false ;

    // overwrite starting at previous terminating zero

    svcBuf->bytesWritten -= 1 ;
    uint8_t zero = 0 ;

    bool fault = ! write (svcBuf, dataPtr, dataLength) ||
                 ! write (svcBuf, & zero, sizeof (zero)) ;

    if (! fault)
    {
        // update the byte array length
        uint16_t  length ;
        uint8_t * lengthPtr = svcBuf->buffer + lastTokenOffset + 1 ;
        memcpy ((uint8_t *) & length, lengthPtr, sizeof (length)) ;     // read it
        length += dataLength ;
        memcpy (lengthPtr, (uint8_t *) & length, sizeof (length)) ;     // write it
    }

    return ! fault ;
}



bool serviceBuffer_appendBytes (ServiceBuffer * svcBuf, uint8_t * dataPtr, uint16_t dataLength)
{
    if (svcBuf == NULL)
        return false ;

    return appendBytes (svcBuf, dataPtr, dataLength) ;
}



bool serviceBuffer_putBytes (ServiceBuffer * svcBuf, uint8_t * dataPtr, uint16_t dataLength)
{
    /*
        This function is used to put both strings and byte arrays - both of
        which will be terminated with a 0

        Note!!  dataLength represents the actual amount of data
                i.e.
                    - either the number of bytes in the byte array or
                    - the number of bytes in the string not including the terminating zero
    */

    if (svcBuf == NULL)
        return false ;

    bool fault = ! createEmptyByteArray (svcBuf) ||
                 ! appendBytes (svcBuf, dataPtr, dataLength) ;

    return ! fault ;
}



bool serviceBuffer_printf (ServiceBuffer * svcBuf, char * formatSpecifier, ...)
{

  #if 1
    (void) svcBuf ;
    (void) formatSpecifier ;

    return false ;

  #else

    tbd

    allocate the rest of svcBuf
    use as much of the allocation as needed to printf the string
    free up the unused remainder of the allocation

    void serialPort_printf (SerialPort * port, String formatSpecifier, ...)
    {
        va_list argptr ;
        char localCopy [MaxLineSize] ;

        va_start  (argptr, formatSpecifier) ;
        vsnprintf (localCopy, sizeof (localCopy), formatSpecifier, argptr) ;
        va_end    (argptr) ;

        char * txPtr = localCopy ;
        (* port->txString) (txPtr) ;
    }

  #endif
}



bool serviceBuffer_putString (ServiceBuffer * svcBuf, char * dataPtr)
{
    return serviceBuffer_putBytes (svcBuf, (uint8_t *) dataPtr, strlen (dataPtr)) ;
    // ... exclude the terminating zero from the length
}



ServiceBufferToken serviceBuffer_getNextToken (ServiceBuffer * svcBuf)
{
    ServiceBufferToken data ;

    if (svcBuf == NULL)
    {
        data.type = ServiceBuffer_End ;
        return data ;
    }

    data.type = svcBuf->buffer [svcBuf->bytesRead] ;

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

        case ServiceBuffer_Bytes :

            dataPtr    = (uint8_t *) & data.bytes.length ;
            dataLength =       sizeof (data.bytes.length) ;
            memcpy (dataPtr, svcBuf->buffer + svcBuf->bytesRead, dataLength) ;
            svcBuf->bytesRead += dataLength ;

            data.bytes.ptr = svcBuf->buffer + svcBuf->bytesRead ;
            svcBuf->bytesRead += data.bytes.length ;

            // increment past the terminating zero
            ++ svcBuf->bytesRead ;
            // now, svcBuf->buffer [svcBuf->bytesRead] should be the next token

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
        {
            fault = (nextToken.real < 0) ||
                    (nextToken.real > UINT32_MAX) ;
            if (! fault)
                * result = floor (nextToken.real) ;
            break ;
        }
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



bool serviceBuffer_getByteArray (ServiceBuffer * svcBuf,  ServiceBuffer_ByteArray * byteArray)
{
    bool fault = false ;

    ServiceBufferToken nextToken = serviceBuffer_getNextToken (svcBuf) ;

    switch (nextToken.type)
    {
        default :
            fault = true ;
            break ;

        case ServiceBuffer_Bytes :
            * byteArray = nextToken.bytes ;
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



bool serviceBuffer_getString (ServiceBuffer * svcBuf, char * * addressOfCharPtr)
{
    ServiceBuffer_ByteArray byteArray ;

    bool fault = ! serviceBuffer_getByteArray (svcBuf, & byteArray) ;

    if (! fault)
        * addressOfCharPtr = (char *) (byteArray.ptr) ;

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

