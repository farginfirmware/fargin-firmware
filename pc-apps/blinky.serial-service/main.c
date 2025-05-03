
/*
    invocation:
        $ ./bin/native64/blinky.serial-service.elf -c /dev/ttyACM0


    ttyACM0 is working ... now need to send serial commands from Lua

*/

#include "Lua.h"


#include "service.h"

#include "thread.h"

#include "board.h"
#include "periph/uart.h"
#include "tsrb.h"
#include "timex.h"
#include "ztimer.h"

#include <string.h>


static uart_t uart = 0 ;    // usually /dev/ttyACM0 on my pc

static struct {
    tsrb_t  ringBuffer ;
    uint8_t data [1024] ;   // must be a power of 2
} rx ;



static void time_delayMilliseconds (uint16_t milliseconds)
{
    const uint16_t MaxMilliseconds = 60 * 1000 ;

    if (milliseconds > MaxMilliseconds)
        milliseconds = MaxMilliseconds ;

    const uint32_t _1_msec = US_PER_SEC / 1000 ;
    ztimer_sleep (ZTIMER_USEC, _1_msec * milliseconds) ;
}


static void rxCallback (void * arg, uint8_t aByte)
{
    (void) arg ;
    tsrb_add_one (& rx.ringBuffer, aByte);
}

static char rxChar (void)
{
    while (1)
    {
        int getOne = tsrb_get_one (& rx.ringBuffer) ;
        if (getOne >= 0)
            return (char) getOne ;
        time_delayMilliseconds (1) ;
    }
}

static void txChar (char tx)
{
    uart_write (uart, (uint8_t *) & tx, sizeof(tx)) ;
}


int main (void)
{

    // init rx ring buffer
    tsrb_init  (& rx.ringBuffer, rx.data, sizeof (rx.data)) ;
    tsrb_clear (& rx.ringBuffer) ;


    printf ("UART_NUMOF = %d\r\n", UART_NUMOF) ;

    uint32_t     baud  = 115200 ;
    uart_rx_cb_t rx_cb = rxCallback ;
    void *       arg   = NULL ;

    int result = uart_init (uart, baud, rx_cb, arg) ;
    printf ("uart_init() returned %d\r\n", result) ;

    switch (result)
    {
        case 0          : break ;
        case UART_NODEV : puts ("uart_init() returned UART_NODEV\r\n") ; return result ;
        default         : return result ;
    }


#if 0
    int loopCount = 2 ;
    while (loopCount --)
    {
        LED0_ON  ;  time_delayMilliseconds ( 25) ;
        LED0_OFF ;  time_delayMilliseconds (975) ;
    }

    // sb data: #( 1 1 ) .  "led on"
    // sb serialOut   > u1 u1 c24D
    //
    // sb data: #( 1 0 ) .  "led off"
    // sb serialOut   > u1 u0 c24C

    char * ledOn  = "> u1 u1 c24D\r" ;
    char * ledOff = "> u1 u0 c24C\r" ;

    loopCount = 10 ;
    while (loopCount --)
    {
        uart_write (uart, (uint8_t *) ledOn,  strlen (ledOn)) ;
        time_delayMilliseconds (1000) ;

        uart_write (uart, (uint8_t *) ledOff, strlen (ledOff)) ;
        time_delayMilliseconds (2000) ;
    }


    // sb data: #( 2 1 'blink.count=1e7') .
    // sb serialOut        > u2 u1 b626C696E6B2E636F756E743D316537 c971
    //
    // sb data: #( 2 1 'blink.duty=0.05') .
    // sb serialOut    > u2 u1 b626C696E6B2E647574793D302E3035 c957

    char * count = "> u2 u1 b626C696E6B2E636F756E743D316537 c971\r" ;
    char * duty  = "> u2 u1 b626C696E6B2E647574793D302E3035 c957\r" ;

    time_delayMilliseconds (500) ;  uart_write (uart, (uint8_t *) count,  strlen (count)) ;
    time_delayMilliseconds (500) ;  uart_write (uart, (uint8_t *)  duty,  strlen ( duty)) ;

    return 0 ;
#endif


    // this is running on a PC, so there is plenty of ram
    const uint16_t LuaStackBytes = 10000 ;
    const uint32_t LuaHeapBytes  = 60000 ;
    Lua_initialize (LuaStackBytes, LuaHeapBytes, rxChar, txChar) ;


    thread_sleep () ;

    return 0 ;
}

