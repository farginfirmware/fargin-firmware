
APPLICATION = blinky

EXTERNAL_MODULE_DIRS ?= ../../modules/
EXTERNAL_BOARD_DIRS  ?= ../../boards/

BOARD ?= makerdiary-nrf52840-m2-kit

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
#DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

ifneq ($(BOARD),native)
  # This stack size is large enough to run Lua print() functions of
  # various lengths. Other functions untested.
# CFLAGS += -DTHREAD_STACKSIZE_MAIN='(THREAD_STACKSIZE_DEFAULT+2048)'
  CFLAGS += -DTHREAD_STACKSIZE_MAIN='(THREAD_STACKSIZE_DEFAULT+8192)'
endif

USEMODULE += stdio_cdc_acm        # can't use this when using stdio_nimble below
USEMODULE += shell
USEMODULE += shell_cmds_default
##EMODULE += ps

USEMODULE += periph_uart


# Use the default file system
USEMODULE += vfs_default
USEMODULE += constfs
# USEMODULE += devfs

# Enable to automatically format if mount fails
USEMODULE += vfs_auto_format                        ### commenting out lets the build run


## USEMODULE += stdio_nimble
## #You can change the default buffer sizes by adding (must be power of 2)
## CFLAGS += -DCONFIG_STDIO_NIMBLE_STDIN_BUFSIZE=1024
## CFLAGS += -DCONFIG_STDIO_NIMBLE_STDOUT_BUFSIZE=2048
## 
## USEMODULE += nimble_autoadv
## CFLAGS += -DCONFIG_NIMBLE_AUTOADV_DEVICE_NAME='"LCU device"'


USEMODULE += service_buffers

USEPKG += lua
USEPKG += littlefs2

# generate .lua.h header files of .lua files
BLOBS += $(wildcard *.lua)

include $(RIOTBASE)/Makefile.include

