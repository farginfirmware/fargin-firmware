
#include "usb.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>

#include "shell.h"

#include "service-buffer.h"
#include "time.h"


static ServiceBuffer request, response ;



static int cmd_text (int argc, char ** argv)
{
    char * usage = "text [length]\n";

    if (argc != 2) {
        puts(usage) ;
        return -1 ;
    }

    int n = atoi (argv[1]) ;

    if (n <= 0) {
        puts(usage) ;
        return -1 ;
    }

    for (int i = 0 ; i < n ; i ++) {
        if (i && (i % 80 == 0)) {
            puts("") ;
        }
        putc ('0' + (i % 10), stdout) ;
    }

    puts ("") ;

    return 0 ;
}


static const shell_command_t shell_commands[] = {
    { "svc",  "host service i/o",  cmd_text },
    { NULL, NULL, NULL }
} ;



static void * usb_thread (void * arg)
{
    (void) arg ;

//  (void) puts ("RIOT USB CDC ACM shell test") ;

    char line_buf [SHELL_DEFAULT_BUFSIZE] ;
    shell_run (shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE) ;

    return NULL ;
}


int usb_initialize (void)
{
    static uint8_t  requestBuffer [1000] ;      // tbd size
    static uint8_t responseBuffer [1000] ;      // tbd size

    serviceBuffer_initialize (&  request,  requestBuffer, sizeof ( requestBuffer)) ;
    serviceBuffer_initialize (& response, responseBuffer, sizeof (responseBuffer)) ;


    static char         stack [2048] ;
    uint8_t             priority =  THREAD_PRIORITY_IDLE - 1 ;  // lowest possible priority
    int                 flags    =  THREAD_CREATE_STACKTEST ;
    thread_task_func_t  task     =  usb_thread ;
    void *              arg      =  NULL ;
    const char *        name     = "USB" ;

    kernel_pid_t pid =
        thread_create (stack, sizeof (stack), priority, flags, task, arg, name) ;

    return pid_is_valid (pid) ;
}

