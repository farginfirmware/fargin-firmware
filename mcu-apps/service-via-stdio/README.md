
service-via-stdio



With this app, a **major** feature of the FarginFirmware framework is
in place - namely the ability to invoke native mcu services via a serial
interface.

If you have the ability to invoke mcu services via a serial interface, then you
have:

 - the ability to test native mcu firmware during development
 - the ability to develop Lua code on a pc
 - the ability to test the hardware (believe it or not, the problem isn't
   always software - sometimes it's hardware)


Being able to test/debug native mcu code via a serial interface will be
invaluable going forward when developing new native code services.

Likewise, the ability to develop Lua code on a pc provides for rapid app
development.



testing the firmware via serial service can be done with
 - a dumb terminal
 - a test program written in any language (including Lua)


this app does it via stdio


there is nothing new going on with Lua in this app - this is all about 
mcu service requests/responses via a serial interface


works concurrently with Lua service requests

this protocol could also be used without Lua
    a client mcu could talk to a server mcu (particularly one which runs only
    native code - i.e. no Lua)



when serial service is no longer needed:
 - after the native code has been wrung out
 - after the Lua app has been developed




test platform
 - [Adafruit ItsyBitsy M4](https://www.adafruit.com/product/3800)


