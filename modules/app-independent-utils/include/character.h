#ifndef _CHARACTER_INCLUDED_
#define _CHARACTER_INCLUDED_

#include "project.h"

#define Control_C        3
#define Control_D        4
#define Control_H        8
#define Control_J       10
#define Control_M       13
#define Control_Z       26
#define Escape          27
#define FileSeparator   28

#define ETX             Control_C
#define EOT             Control_D
#define Backspace       Control_H
#define Linefeed        Control_J
#define CarriageReturn  Control_M


char character_toLower (char) ;
char character_toUpper (char) ;

bool character_isPrintable    (char) ;
bool character_isWhitespace   (char) ;
bool character_isAlphanumeric (char) ;
bool character_isDecimalDigit (char) ;

char    character_hexDigitFromNybble (uint8_t) ;
bool    character_isHexDigit         (char) ;
uint8_t character_hexValue           (char) ;

#endif
