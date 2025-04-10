
#if 0

float: 4,  double: 8

#endif


#include <stdio.h>
#include <string.h>


static char * hexChars = "0123456789abcdef" ;


int main (void)
{
    float  f = 0.1 ;
    double d = 0.1 ;

    printf ("float: %li,  double: %li\n", sizeof(f), sizeof(d)) ;

    return 0 ;
}

