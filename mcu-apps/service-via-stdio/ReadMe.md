
"service-via-stdio"


THIS APP IS STILL IN DEVELOPMENT - i.e. it is not ready to run  tbd


minimal main.lua because this is about serial service

![INSERT BLOCK DIAGRAM HERE](/assets/images/tux.png)


nevertheless...

This app introduces a **major** feature of the Fargin Firmware framework
- namely the ability to invoke native mcu services via a serial interface.
While any serial interface may be used, this app uses stdio.

If you have the ability to invoke native mcu services via a serial interface,
then you have:
 - the ability to test native mcu firmware during development
 - the ability to develop Lua code on a pc and execute it on the target
 - the ability to test the hardware (believe it or not, the problem isn't
   always software)

Being able to test/debug native mcu code via a serial interface will be
invaluable when developing new native code services (e.g. new drivers).

Likewise, the ability to develop Lua code on a pc provides for rapid app
development.

Testing native mcu code via serial service can be done with:
 - a dumb terminal or
 - a test program written in any language (including Lua)

In this example app, serial service works concurrently with Lua service
requests.

But serial service could also be used without Lua - for example, a client mcu
could talk to a server mcu (particularly one which runs only native code - i.e.
no Lua) via any serial interface.

Serial service is no longer needed:
 - after the native code has been wrung out
 - after the Lua app has been developed

