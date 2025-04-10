#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef float Real ;

#define ArrayLength(anArray) (sizeof(anArray) / sizeof((anArray) [0]))

typedef void (* FunctionPointer) (void) ;

#endif
