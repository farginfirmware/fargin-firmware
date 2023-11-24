
#include "ff.time.h"

#include "timex.h"
#include "ztimer.h"


void time_delayMilliseconds (uint16_t milliseconds)
{
    const uint16_t MaxMilliseconds = 60 * 1000 ;

    if (milliseconds > MaxMilliseconds)
        milliseconds = MaxMilliseconds ;

    const uint32_t _1_msec = US_PER_SEC / 1000 ;
    ztimer_sleep (ZTIMER_USEC, _1_msec * milliseconds) ;
}


bool time_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    (void) response ;

    uint32_t milliseconds ;

    bool fault = ! serviceBuffer_getUnsigned32 (request, & milliseconds) ||
                   (milliseconds > UINT16_MAX) ;

    if (! fault)
        time_delayMilliseconds (milliseconds) ;

    return ! fault ;
}

