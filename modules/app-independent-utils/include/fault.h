#ifndef _FAULTS_H_
#define _FAULTS_H_

#include "project.h"

typedef struct {
    uint8_t latched : 1 ;
    uint8_t count   : 7 ;
} Fault ;

uint8_t fault_count       (Fault *) ;

void    fault_set         (Fault *) ;   // setting the latch increments it, saturates at 127
bool    fault_get         (Fault *) ;
bool    fault_getAndReset (Fault *) ;   // simultaneously get/reset the latch

void    fault_clear       (Fault *) ;   // reset latch, clear count

void    fault_initialize  (Fault *) ;

#endif
