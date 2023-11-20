
#include "character.h"


static const char * hexDigits = "0123456789abcdef" ;


char character_toLower (char c)
{
    if (('A' <= c) && (c <= 'Z'))
        return c - 'A' + 'a' ;
    else
        return c ;
}


char character_toUpper (char c)
{
    if (('a' <= c) && (c <= 'z'))
        return c - 'a' + 'A' ;
    else
        return c ;
}


bool character_isPrintable (char c)
{
    return (c >= '!') && (c <= '~') ;
}


bool character_isWhitespace (char c)
{
    return (c != 0) && ! character_isPrintable (c) ;
}


bool character_isAlphanumeric (char c)
{
    c = character_toLower (c) ;

    return (('0' <= c) && (c <= '9')) ||
           (('a' <= c) && (c <= 'z')) ;
}


bool character_isDecimalDigit (char c)
{
    return (('0' <= c) && (c <= '9')) ;
}


char character_hexDigitFromNybble (uint8_t nybble)
{
    if (nybble > 15)
        return 0 ;

    return hexDigits [nybble] ;
}


bool character_isHexDigit (char c)
{
    char digit = character_toLower (c) ;

    return (('0' <= digit) && (digit <= '9')) ||
           (('a' <= digit) && (digit <= 'f')) ;
}


uint8_t character_hexValue (char c)
{
    uint8_t digit ;

    digit = c ;
    if (('0' <= digit) && (digit <= '9'))
        return digit - '0' ;

    digit = character_toLower (digit) ;
    if (('a' <= digit) && (digit <= 'f'))
        return digit - 'a' + 10 ;

    return 0 ;
}

