
#include "service-via-stdio.h"

#include "serial-service.h"
#include "service-buffer.h"
#include "service.h"

#include "ff.time.h"

#include <stdio.h>
#include <stdio_base.h>
#include <string.h>
#include <thread.h>


static uint8_t  requestBuffer [400] ;   // tbd size
static uint8_t responseBuffer [400] ;   // tbd size

static ServiceBuffer request, response ;


static char rxChar (void)
{
    while (stdio_available () <= 0)
        time_delayMilliseconds (1) ;

    char rx ;
    stdio_read (& rx, sizeof (rx)) ;

    return rx ;
}


static void rxClear (void)
{
    char rx ;
    while (stdio_available () > 0)
        stdio_read (& rx, sizeof (rx)) ;
}


static void txChar (char tx)
{
    stdio_write (& tx, sizeof (tx)) ;
}


static void * stdio_thread (void * arg)
{
    (void) arg ;

    serviceBuffer_initialize (&  request,  requestBuffer, sizeof ( requestBuffer)) ;
    serviceBuffer_initialize (& response, responseBuffer, sizeof (responseBuffer)) ;


    while (1)
    {
        rxClear () ;

        bool fault = ! serialService_receive (& request,  rxChar) ;

        if (fault)
        {
            printf ("serialService_receive() error\n") ;
            continue ;
        }

        bool result = service_processRequest (& request, & response) ;

        serialService_transmit (& response, txChar, result) ;
    }


#if 0
    time_delayMilliseconds (3000) ;

    while (1)
    {
        int value = getchar();
        printf("You entered '%c'\n", (char)value);
    }
#endif

    return NULL ;
}



int serviceViaStdio_initialize (void)
{
    static char         stack [2048] ;
    uint8_t             priority =  THREAD_PRIORITY_IDLE - 1 ;  // lowest possible priority
    int                 flags    =  THREAD_CREATE_STACKTEST ;
    thread_task_func_t  task     =  stdio_thread ;
    void *              arg      =  NULL ;
    const char *        name     = "STDIO SVC" ;

    kernel_pid_t pid =
        thread_create (stack, sizeof (stack), priority, flags, task, arg, name) ;

    return pid_is_valid (pid) ;
}

