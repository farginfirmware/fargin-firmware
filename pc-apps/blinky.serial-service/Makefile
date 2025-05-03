
APPLICATION = blinky.serial-service

BOARD = native

EXTERNAL_MODULE_DIRS ?= ../../modules/

FEATURES_REQUIRED += periph_uart

#FarginFirmware modules
USEMODULE += fargin-essentials
USEMODULE += app-independent-utils

USEMODULE += tsrb
USEPKG    += lua

# generate .lua.h header files of .lua files
BLOBS += $(wildcard *.lua)

# this must be the last directive in this file
include $(RIOTBASE)/Makefile.include
