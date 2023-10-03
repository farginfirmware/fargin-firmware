
#include "files.h"


typedef enum {

    // NOTE!! these are tightly coupled to variables in main.lua

    /* 0 */     File_Open,
    /* 1 */     File_Close,
    /* 2 */     File_Delete,
    /* 3 */     File_Rename,
    /* 4 */     File_Position,      // set and/or get
    /* 5 */     File_Read,
    /* 6 */     File_Write,
    /* 7 */     File_Size,
} SubCommand ;


bool files_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    bool fault = false ;

    (void) request ;
    (void) response ;

    fault = true ;



    return ! fault ;
}

