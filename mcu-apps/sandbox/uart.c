
#if 0
    this needs to be modified for multiple uarts
#endif

#include "uart.h"

#include "board.h"
#include "periph/uart.h"

#include "service-buffer.h"

#include "timex.h"
#include "ztimer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread.h>


static uart_t uart = UART_DEV (0) ;


bool uart_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    uint8_t subCommand ;
    char *  aString ;

    bool fault = ! serviceBuffer_getByte (request, & subCommand) ;

    if (! fault)
        switch (subCommand)
        {
            default :   fault = true ;  break ;

            case 0 :
            {
                while (! fault)
                {
                    fault |= ! serviceBuffer_getString (request, & aString) ;

                    if (! fault)
                    {
                        uart_write (uart, (uint8_t *) aString, strlen (aString)) ;

                        char * c = aString ;
                        while (* c)
                        {
                            switch (* c)
                            {
                                default  :               break ;
                                case 'A' :  * c = 'a' ;  break ;
                                case 'a' :  * c = 'A' ;  break ;
                            }
                            ++ c ;
                        }
                        fault |= ! serviceBuffer_putString (response, aString) ;
                    }
                }

                break ;
            }
        }

    return ! fault ;
}



static void * uart_thread (void * arg)
{
    (void) arg ;

    while (1)
    {
      #if 0
        char * testString = "hello - this is a test\r\n" ;
        uart_write (uart, (uint8_t *) testString, strlen (testString)) ;
      #endif

        uint16_t milliseconds = 3 * 1000 ;
        const uint32_t _1_msec = US_PER_SEC / 1000 ;
        ztimer_sleep (ZTIMER_USEC, _1_msec * milliseconds) ;
    }

    return NULL ;
}



int uart_initialize (void)
{
    uint32_t      baud   = 115200 ;
    uart_rx_cb_t  rx_cb  = NULL ;    // optional callback
    void *       arg_cb  = NULL ;    // call back arg
    uart_init (uart, baud, rx_cb, arg_cb) ;

    static char         stack [1024] ;
    uint8_t             priority =  THREAD_PRIORITY_IDLE - 1 ;  // lowest possible priority
    int                 flags    =  THREAD_CREATE_STACKTEST ;
    thread_task_func_t  task     =  uart_thread ;
    void *              arg      =  NULL ;
    const char *        name     = "UART" ;

    kernel_pid_t pid =
        thread_create (stack, sizeof (stack), priority, flags, task, arg, name) ;

    return pid_is_valid (pid) ;
}

