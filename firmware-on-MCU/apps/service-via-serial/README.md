
service-via-serial

With this app, a **major** feature of the FarginFirmware architecture is
introduced - namely the ability to invoke native mcu services via a serial
interface.

If you have the ability to invoke mcu services via a serial interface, then you
have:

 - the ability to test native mcu firmware during development
 - the ability to develop Lua code on a pc
 - the ability to test the hardware (believe it or not, the problem isn't
   always software - sometimes it's hardware)


Being able to test/debug native code via a serial interface will be invaluable
going forward when developing new native code services.

Likewise, the ability to develop Lua code on a pc provides for rapid app
development.



testing the firmware via serial service can be done with
 - a dumb terminal
 - a test program written in any language (including Lua)


this app does it via a uart


there is nothing new going on with Lua in this app - this is all about 
mcu service requests/responses via a serial interface


works concurrently with Lua service requests

this protocol could also be used without Lua
    a client mcu could talk to a server mcu




not needed
 - after the native code has been wrung out
 - after the Lua app has been developed


This app uses a uart for the serial interface, but it could easily be provided
via usb or bluetooth (e.g. via shell commands).





test platform
 - [Makerdiary nRF52840 M.2 developer kit](https://makerdiary.com/products/nrf52840-m2-developer-kit)

