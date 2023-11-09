
APPLICATION = custom-nrf52840

EXTERNAL_MODULE_DIRS ?= ../../modules/
EXTERNAL_BOARD_DIRS  ?= ../../boards/

BOARD ?= custom-nrf52840

ifneq ($(BOARD),native)
  # This stack size is large enough to run Lua print() functions of
  # various lengths. Other functions untested.
  CFLAGS += -DTHREAD_STACKSIZE_MAIN='(THREAD_STACKSIZE_DEFAULT+2048)'
endif

USEMODULE += periph_uart
USEMODULE += periph_i2c

#FarginFirmware modules
USEMODULE += fargin_framework
USEMODULE += app_independent_utils

USEPKG += lua

# generate .lua.h header files of .lua files
BLOBS += $(wildcard *.lua)

include $(RIOTBASE)/Makefile.include

