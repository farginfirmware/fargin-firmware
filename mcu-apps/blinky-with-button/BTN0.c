
#include "BTN0.h"

#include "board.h"
#include "periph/gpio.h"


bool BTN0_processRequest (ServiceBuffer * request, ServiceBuffer * response)
{
    // return button state

    (void) request ;

    uint8_t buttonPressed = gpio_read (BTN0_PIN) ;

    bool fault = ! serviceBuffer_putUnsigned32 (response, buttonPressed) ;

    return ! fault ;
}

