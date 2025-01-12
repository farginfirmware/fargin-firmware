
#include "dotstar.h"    // _DOTSTAR_H_

#include "board.h"

#include "color.h"
#include "apa102.h"
#include "apa102_params.h"

#include <string.h>


static apa102_t dev ;   // LED strip device descriptor

#define NumberOfLeds  APA102_PARAM_LED_NUMOF

// allocate 2 structs for each LED on the strip
static color_rgba_t ledStrip_rgba [NumberOfLeds] ;
static color_hsv_t  ledStrip_hsv  [NumberOfLeds] ;

typedef enum {
    SetHue        = 0, 
    SetSaturation = 1, 
    SetBrightness = 2
} HSV_Setting ;



void dotstar_initialize (void)
{
    // initialize all LED color values to black (off)

    uint16_t ledNum = NumberOfLeds ;
    while (ledNum --)
    {
        color_rgba_t * rgbaPtr = & ledStrip_rgba [ledNum] ;
        color_hsv_t  * hsvPtr  = & ledStrip_hsv  [ledNum] ;

        rgbaPtr->color.r = rgbaPtr->color.g = rgbaPtr->color.b = 0 ;    // 0 <= rgb   <= 255
        rgbaPtr->alpha = 255 ;                                          // 0 <= alpha <= 255

        hsvPtr->h = 0.0 ;  // 0.0 <= hue < 360.0
        hsvPtr->s = 0.0 ;  // 0.0 <= saturation <= 1.0
        hsvPtr->v = 0.0 ;  // 0.0 <= brightness <= 1.0
    }

    // initialize the driver
    // apa102_params[] is declared in drivers/apa102/include/apa102_params.h
    apa102_init (& dev, & apa102_params [0]) ;
}



bool dotstar_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    (void) response ;

  #if ! defined APA102_PARAM_LED_NUMOF
    return false ;
  #endif

    bool fault = false ;

    uint8_t  function ;
    uint32_t ledIndex ;
    union {
        Real any ;
        Real hue ;
        Real saturation ;
        Real brightness ;
    } hsv ;

    fault |= ! serviceBuffer_getByte       (request, & function) ;
    fault |= ! serviceBuffer_getUnsigned32 (request, & ledIndex) ;
    fault |= ! serviceBuffer_getReal       (request, & hsv.any) ;

    fault |= ledIndex >= NumberOfLeds ;

    color_hsv_t  * hsvPtr  = & ledStrip_hsv  [ledIndex] ;
    color_rgba_t * rgbaPtr = & ledStrip_rgba [ledIndex] ;

    if (! fault)
        switch (function)
        {
            default            : fault = true ;                 break ;
            case SetHue        : hsvPtr->h = hsv.hue ;          break ;
            case SetSaturation : hsvPtr->s = hsv.saturation ;   break ;
            case SetBrightness : hsvPtr->v = hsv.brightness ;   break ;
        }

    // convert hsv to rgba
    color_hsv2rgb (hsvPtr, & rgbaPtr->color) ;

    // write to led array
    apa102_load_rgba (& dev, ledStrip_rgba) ;

    return ! fault ;
}

