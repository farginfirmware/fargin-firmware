
APPLICATION = blinky.serial-service


# build it
    # make BOARD = adafruit-itsybitsy-m4

# flash it
    # make BOARD = adafruit-itsybitsy-m4  flash

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

USEMODULE += stdio_cdc_acm  ## ??
USEMODULE += stdin
USEMODULE += usbus
USEMODULE += auto_init_usbus

# USB device vendor and product ID
USB_VID ?= $(USB_VID_TESTING)
USB_PID ?= $(USB_PID_TESTING)


# this must be the last directive in this file
include $(RIOTBASE)/Makefile.include
