#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef float Real ;

#define ArrayLength(anArray) (sizeof(anArray) / sizeof((anArray) [0]))

typedef void (* FunctionPointer) (void) ;

#if defined (BOARD_NATIVE32) || defined (BOARD_NATIVE64)
   #define   BOARD_NATIVE
#endif

#endif
