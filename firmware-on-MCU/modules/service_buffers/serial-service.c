
#include "serial-service.h"

#include "service-buffer.h"



/// tbd - handle checksum




#if 0

    2 modes

        server (native mcu code)

            rx command and write to request buffer
            process service request
            tx response from response buffer

        client (pc app)

            tx command from request buffer
            rx response and write to response buffer

        so, 2 functions

            convert a serial rx stream to a service buffer
            convert a service buffer to a serial tx stream


    multiple interfaces
        1 instance per interface

    each instance
        thread
            receive command
            invoke service
            transmit response

#endif


//    uX.....XX           32-bit Unsigned integer (1 to 8 hex nybbles)
//    sX.....XX           32-bit   Signed integer (1 to 8 hex nybbles
//                                                 bit 3 of first nybble is the sign bit)
//    rXXXXXXXXXXXXXXXX   64-bit Double  (16 hex nybbles exactly)
//    bXX...XXXX          Byte array (even number of hex nybbles) ; same for string ; exclude terminating 0
//
//    y                   Yes     True
//    n                   No/Nil  False (in Lua, false and nil are false - everything else is true)
//
//    cXXXX               2-byte checksum (4 hex nybbles)
//
#define Prefix_Unsigned   'u'
#define Prefix_Signed     's'
#define Prefix_Real       'r'
#define Prefix_Bytes      'b'
#define Prefix_Yes        'y'
#define Prefix_No         'n'
#define Prefix_Checksum   'c'


static const char * hexDigits = "0123456789ABCDEF" ;



bool serialService_receive (ServiceBuffer * svcBuf, RxFunctionPtr rxFn)
{
    (void) svcBuf ;
    (void) rxFn ;

    bool fault = false ;

    serviceBuffer_reset (svcBuf) ;



    // tbd



    return ! fault ;
}



static void tx_Bitfield32 (TxFunctionPtr txFn, uint32_t data)
{
    // do not tx leading 0's

    bool suppressLeadingZeroes = true ;

    uint8_t nybbleNumber = 2 * sizeof (data) ;
    while (nybbleNumber --)
    {
        uint8_t nybble = (data >> (4 * nybbleNumber)) & 0xf ;

        if (suppressLeadingZeroes && (nybble == 0) && (nybbleNumber > 0))
            continue ;

        suppressLeadingZeroes = false ;

        txFn (hexDigits [nybble]) ;
    }
}


static void tx_Unsigned32 (TxFunctionPtr txFn, uint32_t data)
{
    txFn (' ') ;
    txFn (Prefix_Unsigned) ;
    tx_Bitfield32 (txFn, data) ;
}


static void tx_Signed32 (TxFunctionPtr txFn, int32_t data)
{
    txFn (' ') ;
    txFn (Prefix_Signed) ;

    uint32_t unsignedData = (uint32_t) data ;

    uint8_t nybbleNumber = 2 * sizeof (data) ;
    while (nybbleNumber -- > 1)
    {
        uint8_t thisNybble, nextNybble ;

        thisNybble = (unsignedData >> (4 * nybbleNumber)) & 0xf ;

        if (thisNybble != 0xf)
            break ;

        nextNybble = (unsignedData >> (4 * (nybbleNumber - 1))) & 0xf ;
        if ((nextNybble & 0x8) == 0)    // if the sign bit is 0
            break ;

        // remove the leading 0xf
        unsignedData ^= 0xf << (4 * nybbleNumber) ;
    }

    tx_Bitfield32 (txFn, unsignedData) ;
}


static void tx_Real (TxFunctionPtr txFn, Real data)
{
    uint8_t * bytePtr = (uint8_t *) & data ;

    txFn (' ') ;
    txFn (Prefix_Real) ;

    uint8_t nybbleNumber = 2 * sizeof (data) ;

    while (nybbleNumber --)
    {
        txFn (hexDigits [* bytePtr >>  4]) ;
        txFn (hexDigits [* bytePtr & 0xf]) ;
        ++ bytePtr ;
    }
}


static void tx_Boolean (TxFunctionPtr txFn, bool data)
{
    txFn (' ') ;
    txFn (data ? Prefix_Yes : Prefix_No) ;
}


static void tx_Nil (TxFunctionPtr txFn)
{
    txFn (' ') ;
    txFn (Prefix_No) ;
}


static void tx_Bytes (TxFunctionPtr txFn, uint8_t * dataPtr, uint16_t dataLength)
{
    txFn (' ') ;
    txFn (Prefix_Bytes) ;

    while (dataLength --)
    {
        txFn (hexDigits [* dataPtr >>  4]) ;
        txFn (hexDigits [* dataPtr & 0xf]) ;
        ++ dataPtr ;
    }
}


static void tx_Checksum (TxFunctionPtr txFn, uint16_t checksum)
{
    (void) txFn ;
    (void) checksum ;

    txFn (' ') ;
    txFn (Prefix_Checksum) ;

    tx_Bitfield32 (txFn, checksum) ;

    txFn ('\r') ;
}



bool serialService_transmit (ServiceBuffer * svcBuf, TxFunctionPtr txFn)
{
    // this is going to be a lot easier than the complementary receive function

    bool fault = false ;

    uint16_t checksum = 0 ;

    while (! fault)
    {
        ServiceBufferToken token = serviceBuffer_getNextToken (svcBuf) ;

        switch (token.type)
        {
            default                       : fault = true ;  break ;

            case ServiceBuffer_End        : tx_Checksum   (txFn, checksum) ;            break ;

            case ServiceBuffer_Nil        : tx_Nil        (txFn) ;                      break ;

            case ServiceBuffer_Boolean    : tx_Boolean    (txFn, token.boolean != 0) ;  break ;

            case ServiceBuffer_Unsigned32 : tx_Unsigned32 (txFn, token.unsigned32) ;    break ;
            case ServiceBuffer_Signed32   : tx_Signed32   (txFn, token.  signed32) ;    break ;
            case ServiceBuffer_Real       : tx_Real       (txFn, token.      real) ;    break ;

            case ServiceBuffer_Bytes      :
                tx_Bytes (txFn, token.bytes.ptr, token.bytes.length) ;
                break ;
        }
    }

    return ! fault ;
}

