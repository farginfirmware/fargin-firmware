
#include "fault.h"  // _FAULTS_H_


static const uint8_t MaxFaultCount = 127 ;



uint8_t fault_count (Fault * fault)
{
    return fault->count ;
}


void fault_set (Fault * fault)
{
    // set the latch if it is not already set ;
    // increment the count only if the fault latch was not already set

    if (fault->latched)
        return ;

    fault->latched = true ;

    if (fault->count < MaxFaultCount)
        ++ fault->count ;
}


bool fault_get (Fault * fault)
{
    return fault->latched ;
}


bool fault_getAndReset (Fault * fault)
{
    // return the state of the fault latch prior to resetting it
    bool wasSet = fault->latched ;
    fault->latched = false ;
    return wasSet ;
}


void fault_clear (Fault * fault)
{
    fault->latched = false ;
    fault->count   = 0 ;
}


void fault_initialize (Fault * fault)
{
    fault_clear (fault) ;
}

