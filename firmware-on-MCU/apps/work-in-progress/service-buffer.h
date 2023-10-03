#ifndef _SERVICE_BUFFER_H_
#define _SERVICE_BUFFER_H_

#include "project.h"


typedef struct {
    uint8_t  *  buffer ;
    uint16_t    bytesCapacity ;
    uint16_t    bytesWritten ;
    uint16_t    bytesRead ;
} ServiceBuffer ;


typedef enum {
    ServiceBuffer_End,          // no more tokens
    ServiceBuffer_Nil,
    ServiceBuffer_Boolean,
    ServiceBuffer_Unsigned32,
    ServiceBuffer_Signed32,
    ServiceBuffer_Real,
    ServiceBuffer_Bytes,        // includes byte arrays and strings - both are zero-terminated
    ServiceBuffer_numberOfEntryTypes
} ServiceBufferTokenType ;


typedef struct {
    uint8_t * ptr ;
    uint16_t  length ;  // number of bytes in a byte array or string not including 0-termination
} ServiceBuffer_ByteArray ;

typedef struct {
    ServiceBufferTokenType type ;
    union {
        uint8_t                   boolean ;
        uint32_t                  unsigned32 ;
         int32_t                    signed32 ;
        Real                      real ;
        ServiceBuffer_ByteArray   bytes ;
    } ;
} ServiceBufferToken ;


ServiceBufferToken serviceBuffer_getNextToken (ServiceBuffer *) ;


bool serviceBuffer_getByte       (ServiceBuffer *,  uint8_t * result) ;
bool serviceBuffer_getUnsigned32 (ServiceBuffer *, uint32_t * result) ;
bool serviceBuffer_getSigned32   (ServiceBuffer *,  int32_t * result) ;
bool serviceBuffer_getReal       (ServiceBuffer *,     Real * result) ;
bool serviceBuffer_getString     (ServiceBuffer *,   char * * addressOfCharPtr) ;


bool serviceBuffer_putNil        (ServiceBuffer *) ;
bool serviceBuffer_putBoolean    (ServiceBuffer *, uint8_t) ;
bool serviceBuffer_putUnsigned32 (ServiceBuffer *, uint32_t) ;
bool serviceBuffer_putSigned32   (ServiceBuffer *,  int32_t) ;
bool serviceBuffer_putReal       (ServiceBuffer *, Real) ;
bool serviceBuffer_putBytes      (ServiceBuffer *, uint8_t * bytes, uint16_t numberOfBytes) ;
bool serviceBuffer_putString     (ServiceBuffer *, char *) ;       // zero-terminated

void serviceBuffer_reset         (ServiceBuffer *) ;



// call once for each request buffer and once for each response buffer
void serviceBuffer_initialize (ServiceBuffer *, uint8_t * buffer, uint16_t capacity) ;


#endif

