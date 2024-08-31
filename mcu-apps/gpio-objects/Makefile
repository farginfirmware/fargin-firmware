
APPLICATION = gpio-objects


# un-comment one of the following BOARDs or specify the BOARD on the command line ;
# for example:
#   make BOARD=feather-m0 flash

    # the following are built into RIOT
    #   BOARD ?= adafruit-itsybitsy-m4
    #   BOARD ?= feather-m0

    # the following are not built into RIOT
    #   BOARD ?= trinket-m0
    #   BOARD ?= sparkfun-thing-plus-samd51
    #   BOARD ?= makerdiary-nrf52840-m2-kit
        BOARD ?= makerdiary-nrf52840-mdk-iot-kit


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
