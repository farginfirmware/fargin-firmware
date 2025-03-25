#ifndef _SERVICE_BUFFER_H_
#define _SERVICE_BUFFER_H_

#include "project.h"


typedef struct {
    uint8_t  *  buffer ;
    uint16_t    bytesCapacity ;
    uint16_t    bytesWritten ;
    uint16_t    bytesRead ;
    uint16_t    lastTokenOffset ;   // offset to the start of the last token written ;
                                    // needed for appending byte arrays (including strings)
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


// includes byte arrays and strings - both are zero-terminated
typedef struct {
    uint8_t * ptr ;
    uint16_t  length ;      // NOT including 0-termination
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
bool serviceBuffer_getString     (ServiceBuffer *, char * * addressOfCharPtr) ;
bool serviceBuffer_getByteArray  (ServiceBuffer *, ServiceBuffer_ByteArray *) ;
bool serviceBuffer_getBytesCopy  (ServiceBuffer *,  uint8_t * dataPtr, uint16_t dataLength) ;


bool serviceBuffer_putNil        (ServiceBuffer *) ;
bool serviceBuffer_putBoolean    (ServiceBuffer *, uint8_t) ;
bool serviceBuffer_putUnsigned32 (ServiceBuffer *, uint32_t) ;
bool serviceBuffer_putSigned32   (ServiceBuffer *,  int32_t) ;
bool serviceBuffer_putReal       (ServiceBuffer *, Real) ;

bool serviceBuffer_putString     (ServiceBuffer *, char *) ;
bool serviceBuffer_appendString  (ServiceBuffer *, char *) ;
bool serviceBuffer_printf        (ServiceBuffer *, char * formatSpecifier, ...) ;

// like strings, byte arrays will be 0-terminated
// (append bytes is needed in particular for service via serial interface)
bool serviceBuffer_putBytes      (ServiceBuffer *, uint8_t * bytes, uint16_t numberOfBytes) ;
bool serviceBuffer_appendBytes   (ServiceBuffer *, uint8_t * bytes, uint16_t numberOfBytes) ;


void serviceBuffer_reset         (ServiceBuffer *) ;


// call once for each request buffer and once for each response buffer
void serviceBuffer_initialize (ServiceBuffer *, uint8_t * buffer, uint16_t capacity) ;


#endif

