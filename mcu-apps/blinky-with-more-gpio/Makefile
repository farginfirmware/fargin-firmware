
APPLICATION = blinky-with-more-gpio

EXTERNAL_BOARD_DIRS  ?= ../../boards/

BOARD ?= makerdiary-nrf52840-m2-kit

ifneq ($(BOARD),native)
  # This stack size is large enough to run Lua print() functions of
  # various lengths. Other functions untested.
  CFLAGS += -DTHREAD_STACKSIZE_MAIN='(THREAD_STACKSIZE_DEFAULT+2048)'
endif

USEPKG += lua

# generate .lua.h header files of .lua files
BLOBS += $(wildcard *.lua)

include $(RIOTBASE)/Makefile.include

