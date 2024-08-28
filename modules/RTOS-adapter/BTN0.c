
#include "BTN0.h"

#include "board.h"
#include "periph/gpio.h"


bool BTN0_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    // return button state (pressed or not)

    (void) request ;

    bool    fault = false ;
    uint8_t buttonPressed ;

  #if defined BTN0_PIN
    buttonPressed = gpio_read (BTN0_PIN) ;
  #else
    buttonPressed = 1 ;     // would be 0 if pulled down
    fault = true ;
  #endif

    fault |= ! serviceBuffer_putUnsigned32 (response, buttonPressed) ;

    return ! fault ;
}

