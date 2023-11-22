# Fargin Firmware

This repository is for the development (in progress) of the
[FarginFirmware](http://www.farginFirmware.com) framework
for microcontroller-based embedded systems.

FarginFirmware is built on top of [Lua](http://www.lua.org) and
[RIOT OS](http://www.riot-os.org) ("The friendly Operating System for the Internet of Things").

- application complexities are the domain of Lua
- mcu and board hardware are the domain of RIOT OS

What's so ~~great~~ ~~special~~ different about this firmware framework?

- the native mcu code
  - is minimal
  - with low error potential
  - and a high degree of testability
  - is written entirely in C
  - is highly portable
  - obviates the need for a bootloader

- the application is written in Lua
  - bytecode compiler and interpreter are built into the firmware
  - application source is stored in a file system on the target
  - may be modified on-the-fly (i.e. without having to do a reset)

- Lua app development is on a PC
  - Lua-based app connects to a serial interface on the target
  - write your Lua code in your editor
  - load your Lua code into the pc app
  - execute the code
    - service requests are sent to the target
    - the target returns service responses to the pc
  - download Lua source to the target for subsequent stand-alone running


examples in folder mcu-apps/
- blinky
- blinky-with-button
- blinky-with-more-gpio
- threads-and-coroutines
- etc (tbd ... in progress)

external references:
- [RIOT OS 2023.10](https://github.com/RIOT-OS/RIOT/tree/2023.10-branch) which includes
- a [Lua 5.3](https://www.lua.org/versions.html#5.3) package

