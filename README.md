# Fargin Firmware

This repository is for the development (in progress) of the
[FarginFirmware](http://www.farginFirmware.com) architecture
for microcontroller-based embedded systems.

FarginFirmware is built on the shoulders of [Lua](http://www.lua.org) and
[RIOT OS](http://www.riot-os.org) ("The friendly Operating System for the Internet of Things").

- RIOT OS handles the mcu and board hardware
- Lua handles application complexity

What's so ~~great~~ ~~special~~ different about this firmware?

- the firmware (native mcu code) is
  - minimal
  - with low error potential
  - and a high degree of testability
  - written entirely in C
  - highly portable
  - bootloader not needed

- the application is written in Lua
  - interpreter and bytecode compiler are built into the firmware
  - app source is stored in a file system on the target
  - may be modified on-the-fly (i.e. without having to do a reset)

- Lua app development is on a PC
  - Lua-based app connects to a serial interface on the target
  - write your Lua code in your editor
  - load your Lua code into the pc app
  - execute the code
    - service requests are sent to the target
    - service responses are returned to the pc
  - download Lua source to the target for subsequent stand-alone running


current branches
- blinky
- blinky-with-more-gpio
- tbd (in progress)

external references:
- [RIOT OS 2023.07](https://github.com/RIOT-OS/RIOT/tree/2023.07-branch) which includes
- a [Lua 5.3](https://www.lua.org/versions.html#5.3) package

