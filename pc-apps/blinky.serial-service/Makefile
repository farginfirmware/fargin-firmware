
APPLICATION = blinky.serial-service

BOARD = native

EXTERNAL_MODULE_DIRS ?= ../../modules/

# ifneq ($(BOARD),native)
#   # This stack size is large enough to run Lua print() functions of
#   # various lengths. Other functions untested.
#   CFLAGS += -DTHREAD_STACKSIZE_MAIN='(THREAD_STACKSIZE_DEFAULT+2048)'
# endif

FEATURES_REQUIRED += periph_uart

#FarginFirmware modules
USEMODULE += fargin-essentials
USEMODULE += app-independent-utils

USEPKG += lua

# generate .lua.h header files of .lua files
BLOBS += $(wildcard *.lua)

# this must be the last directive in this file
include $(RIOTBASE)/Makefile.include
