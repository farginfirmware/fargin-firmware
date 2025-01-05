
"blinky"

This app merely blinks an LED (LED0). Pressing a button (BTN0) doubles the
blink rate.

Most boards define at least LED0. Many boards also define BTN0.

This is a small, but significant, first step. In particular, it introduces
service buffers which are an essential component of the fargin firmware
framework. Service request buffers are used by Lua to request service from the
native mcu code. An associated response buffer returns a result indication and
may also include response data. In this example, Lua uses service buffers to
send requests for LED0, BTN0 and time services. LED0 and time services do not
return any data. BTN0 service returns the state of the button.

![fargin framework]()

This app should run on any
[RIOT OS defined board](https://github.com/RIOT-OS/RIOT/tree/master/boards)
which includes LED0 (and which has enough code and data space) and, optionally,
BTN0.

