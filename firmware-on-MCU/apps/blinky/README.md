
This "blinky" app is intended to run on any RIOT OS board which defines LED0.
(Most boards have at least LED0.) Some small MCUs might not have enough CODE or
DATA space to work (Lua takes a lot of both).

This is a small, but important, first step. In particular, it introduces the
concept of service buffers. Service request buffers are used to get service
from the OS. An associated response buffer will have a result indication and
may contain response data. In this example, Lua sends requests for LED0 and
time services, but does not yet do anything with service responses.

app info:
 - single-threaded
 - no Lua co-routines
 - tested on a
    [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)
    (which is not currently a stock RIOT OS board)

