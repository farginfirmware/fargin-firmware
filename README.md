# Fargin Firmware

This repository is for the development (in progress) of the
[FarginFirmware](http://www.farginFirmware.com) architecture
for microcontroller-based embedded systems.

FarginFirmware is built upon
[the RIOT operating system](http://www.riot-os.org) and
[Lua](http://www.lua.org).

What's so ~~great~~ ~~special~~ different about this firmware?

  * no bootloader
    *  firmware upgrade is never needed
    *  program the native mcu code once, then put away the programmer
  * modify code on-the-fly


versions:
  * [RIOT OS 2023.07](https://github.com/RIOT-OS/RIOT/tree/2023.07-branch) which includes
  * a [Lua 5.3](https://www.lua.org/versions.html#5.3) package
