
APPLICATION = blinky

EXTERNAL_BOARD_DIRS ?= ../../boards/    # you may have put your boards in some other folder

# This has to be the absolute path to the RIOT base directory.
# You should already have it defined if you set up RIOT OS correctly.
#RIOTBASE ?= $(CURDIR)/../..

ifneq ($(BOARD),native)
  # This stack size is large enough to run Lua print() functions of
  # various lengths. Other functions untested.
  CFLAGS += -DTHREAD_STACKSIZE_MAIN='(THREAD_STACKSIZE_DEFAULT+2048)'
endif

USEPKG += lua

# generate .lua.h header files of .lua files
BLOBS += $(wildcard *.lua)

include $(RIOTBASE)/Makefile.include

