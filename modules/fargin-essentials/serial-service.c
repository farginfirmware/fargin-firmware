
#include "serial-service.h"

#include "character.h"
#include "service-buffer.h"

#include <string.h>


#if 0

    2 modes

        server (native mcu code)

            rx command and write to request buffer
            process service request
            tx response from response buffer

        client (pc app)

            tx command from request buffer
            rx response and write to response buffer

    so, a function for each mode

        convert  a serial rx stream  to  a service buffer
        convert  a service buffer    to  a serial tx stream


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

#define MessagePrefix     '>'


typedef struct {
    RxFunctionPtr  rxFn ;
    TxFunctionPtr  txFn ;
    uint16_t       checksum ;
} SerialSvcTxfr ;



static void txChar (SerialSvcTxfr * txfr, char tx)
{
    txfr->txFn (tx) ;
    txfr->checksum += tx ;
}


#if 0
static void txString (SerialSvcTxfr * txfr, char * tx)
{
    while (* tx)
        txChar (txfr, * tx ++) ;
}
#endif


static char rxChar (SerialSvcTxfr * txfr)
{
    char rx = txfr->rxFn() ;
    txfr->checksum += rx ;
    return rx ;
}


static bool rxHexString (SerialSvcTxfr * txfr, char * digitString, uint8_t maxLength)
{
    bool fault = false ;

    if (maxLength < 2)
        return false ;

    uint8_t length = 0 ;

    while (! fault && (length < maxLength - 1))
    {
        char nextChar = character_toLower (rxChar (txfr)) ;

        if (nextChar == ' ')
            break ;

        fault = ! character_isHexDigit (nextChar) ;

        if (! fault)
        {
            digitString [length ++] = nextChar ;
            digitString [length   ] = 0 ;
        }
    }

    fault |= length < 1 ;

    return ! fault ;
}


static bool rxUnsigned (SerialSvcTxfr * txfr, uint32_t * data)
{
    // receive 1 to 8 hex characters followed by a space

    bool fault = false ;

    char hexString [2 * sizeof (* data) + 1] ;

    if (! rxHexString (txfr, hexString, sizeof (hexString)))
        return ! (fault = true) ;

    char *   inPtr   = hexString ;
    uint32_t inValue = 0 ;

    while (* inPtr)
        inValue = (inValue << 4) + character_hexValue (* inPtr ++) ;

    * data = inValue ;

    return ! fault ;
}


static bool rxSigned (SerialSvcTxfr * txfr, int32_t * data)
{
    // receive 1 to 8 hex characters followed by a space

    bool fault = false ;

    char hexString [2 * sizeof (* data) + 1] ;

    if (! rxHexString (txfr, hexString, sizeof (hexString)))
        return ! (fault = true) ;

    char *  inPtr   = hexString ;
    int32_t inValue = 0 ;

    // the MS bit of the 1st nybble determines the sign of the result
    if (character_hexValue (* inPtr) & 0x8)
        inValue = -1 ;

    while (* inPtr)
        inValue = (inValue << 4) + character_hexValue (* inPtr ++) ;

    * data = inValue ;

    return ! fault ;
}


static bool rxReal (SerialSvcTxfr * txfr, Real * data)
{
    // receive exactly (2 * sizeof(Real) hex characters followed by a space

    bool fault ;

    char hexString [2 * sizeof (* data) + 1] ;

    fault = ! rxHexString (txfr, hexString, sizeof (hexString)) ||
            ! (strlen (hexString) == 2 * sizeof (* data)) ;

    if (fault)
        return ! fault ;

    char *    inPtr = hexString ;
    Real      inValue ;
    uint8_t * inValuePtr = (uint8_t *) & inValue ;

    while (* inPtr)
    {
        uint8_t aByte =                character_hexValue (* inPtr ++) ;
                aByte = (aByte << 4) + character_hexValue (* inPtr ++) ;

        * inValuePtr ++ = aByte ;
    }

    * data = inValue ;

    return ! fault ;
}



static bool rxBytes (SerialSvcTxfr * txfr, ServiceBuffer * svcBuf)
{
    // rx even number of hex digits
    // minimum 2 (i.e. at least 1 byte)
    // write received data directly to svcBuf

    bool fault = false ;

    uint16_t byteCount = 0 ;

    while (! fault)
    {
        char in [2] ;

        in [0] = rxChar (txfr) ;
        if (in [0] == ' ')
            break ;
        fault = ! character_isHexDigit (in [0]) ;
        if (fault)
            break ;

        in [1] = rxChar (txfr) ;
        fault = ! character_isHexDigit (in [1]) ;
        if (fault)
            break ;

        uint8_t aByte = (character_hexValue (in [0]) << 4) +
                         character_hexValue (in [1]) ;

        if (++ byteCount == 1)
            fault = ! serviceBuffer_putBytes (svcBuf, & aByte, sizeof (aByte)) ;
        else
            fault = ! serviceBuffer_appendByte (svcBuf, aByte) ;
    }

    return ! fault ;
}



bool serialService_receive (ServiceBuffer * svcBuf, RxFunctionPtr rxFnPtr)
{
    SerialSvcTxfr txfr ;
    txfr.rxFn = rxFnPtr ;

    bool fault = false ;

    serviceBuffer_reset (svcBuf) ;

    // wait for the message prefix
    while (1)
    {
        txfr.checksum = 0 ;
        if (rxChar (& txfr) == MessagePrefix)
            break ;
    }


    while (! fault)
    {
        char prefix = character_toLower (rxChar (& txfr)) ;

        switch (prefix)
        {
            default  :  fault = true ;  break ;
            case ' ' :                  break ;

            case Prefix_Unsigned :
            {
                uint32_t data ;
                fault = ! rxUnsigned (& txfr, & data) ||
                        ! serviceBuffer_putUnsigned32 (svcBuf, data) ;
                break ;
            }

            case Prefix_Signed :
            {
                int32_t data ;
                fault = ! rxSigned (& txfr, & data) ||
                        ! serviceBuffer_putSigned32 (svcBuf, data) ;
                break ;
            }

            case Prefix_Real :
            {
                Real data ;
                fault = ! rxReal (& txfr, & data) ||
                        ! serviceBuffer_putReal (svcBuf, data) ;
                break ;
            }

            case Prefix_Bytes :
                fault = ! rxBytes (& txfr, svcBuf) ;
                break ;

            case Prefix_Yes :
                fault = ! serviceBuffer_putBoolean (svcBuf, true) ||
                        ! (rxChar (& txfr) == ' ') ;
                break ;

            case Prefix_No :
                fault = ! serviceBuffer_putBoolean (svcBuf, false) ||
                        ! (rxChar (& txfr) == ' ') ;
                break ;

            case Prefix_Checksum :
            {

                // tbd

                break ;
            }
        }

    }


    // tbd


    return ! fault ;
}



static void tx_Bitfield32 (SerialSvcTxfr * txfr, uint32_t data)
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

        txChar (txfr, character_hexDigitFromNybble (nybble)) ;
    }
}


static void tx_Unsigned32 (SerialSvcTxfr * txfr, uint32_t data)
{
    txChar (txfr, ' ') ;
    txChar (txfr, Prefix_Unsigned) ;
    tx_Bitfield32 (txfr, data) ;
}


static void tx_Signed32 (SerialSvcTxfr * txfr, int32_t data)
{
    txChar (txfr, ' ') ;
    txChar (txfr, Prefix_Signed) ;

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

    tx_Bitfield32 (txfr, unsignedData) ;
}


static void tx_Real (SerialSvcTxfr * txfr, Real data)
{
    uint8_t * bytePtr = (uint8_t *) & data ;

    txChar (txfr, ' ') ;
    txChar (txfr, Prefix_Real) ;

    uint8_t nybbleNumber = 2 * sizeof (data) ;

    while (nybbleNumber --)
    {
        txChar (txfr, character_hexDigitFromNybble (* bytePtr >>  4)) ;
        txChar (txfr, character_hexDigitFromNybble (* bytePtr & 0xf)) ;
        ++ bytePtr ;
    }
}


static void tx_Boolean (SerialSvcTxfr * txfr, bool data)
{
    txChar (txfr, ' ') ;
    txChar (txfr, data ? Prefix_Yes : Prefix_No) ;
}


static void tx_Nil (SerialSvcTxfr * txfr)
{
    txChar (txfr, ' ') ;
    txChar (txfr, Prefix_No) ;
}


static void tx_Bytes (SerialSvcTxfr * txfr, uint8_t * dataPtr, uint16_t dataLength)
{
    txChar (txfr, ' ') ;
    txChar (txfr, Prefix_Bytes) ;

    while (dataLength --)
    {
        txChar (txfr, character_hexDigitFromNybble (* dataPtr >>  4)) ;
        txChar (txfr, character_hexDigitFromNybble (* dataPtr & 0xf)) ;
        ++ dataPtr ;
    }
}


static void tx_Checksum (SerialSvcTxfr * txfr)
{
    txChar (txfr, ' ') ;
    txChar (txfr, Prefix_Checksum) ;

    uint16_t checksumCopy = txfr->checksum ;

    tx_Bitfield32 (txfr, checksumCopy) ;

    txChar (txfr, ' ') ;
    txChar (txfr, '\r') ;
}



bool serialService_transmit (ServiceBuffer * svcBuf, TxFunctionPtr txFnPtr)
{
    // this is going to be a lot easier than the complementary receive function

    SerialSvcTxfr txfr ;
    txfr.txFn = txFnPtr ;
    txfr.checksum = 0 ;

    bool fault = false ;

    txChar (& txfr, MessagePrefix) ;

    while (! fault)
    {
        ServiceBufferToken token = serviceBuffer_getNextToken (svcBuf) ;

        switch (token.type)
        {
            default                       : fault = true ;                                break ;

            case ServiceBuffer_End        : tx_Checksum   (& txfr) ;                      break ;

            case ServiceBuffer_Nil        : tx_Nil        (& txfr) ;                      break ;

            case ServiceBuffer_Boolean    : tx_Boolean    (& txfr, token.boolean != 0) ;  break ;

            case ServiceBuffer_Unsigned32 : tx_Unsigned32 (& txfr, token.unsigned32) ;    break ;
            case ServiceBuffer_Signed32   : tx_Signed32   (& txfr, token.  signed32) ;    break ;
            case ServiceBuffer_Real       : tx_Real       (& txfr, token.      real) ;    break ;

            case ServiceBuffer_Bytes      :
                tx_Bytes (& txfr, token.bytes.ptr, token.bytes.length) ;
                break ;
        }
    }

    return ! fault ;
}

