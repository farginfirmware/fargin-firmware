
APPLICATION = blinky

# the definition for the following BOARD is found in the RIOT/boards folder
BOARD ?= adafruit-itsybitsy-m4
# ... but this blinky app should run on just about any board which defines LED0

EXTERNAL_MODULE_DIRS ?= ../../modules/
EXTERNAL_BOARD_DIRS  ?= ../../boards/

ifneq ($(BOARD),native)
  # This stack size is large enough to run Lua print() functions of
  # various lengths. Other functions untested.
  CFLAGS += -DTHREAD_STACKSIZE_MAIN='(THREAD_STACKSIZE_DEFAULT+2048)'
endif

#FarginFirmware modules
USEMODULE += fargin-essentials
USEMODULE += RTOS-adapter
USEMODULE += app-independent-utils

USEPKG += lua

# generate .lua.h header files of .lua files
BLOBS += $(wildcard *.lua)


# this must be the last directive in this file
include $(RIOTBASE)/Makefile.include
