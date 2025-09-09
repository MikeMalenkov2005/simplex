#### PROJECT FILES ####

SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

#### TARGET CONFIG ####

ARCH = x86

KERNEL_NAME = simplex
KERNEL_VERSION = 0.1
KERNEL = $(ARCH)-$(KERNEL_NAME)-$(KERNEL_VERSION)
KERNEL_FILE = $(ROOTDIR)/$(KERNEL).sys

LIB_NAME = simp
LIB_FILE = $(ROOTDIR)/lib$(LIB_NAME).a

MODULES = test

#### TOOLS CONFIG ####

CC = clang
CFLAGS = -I$(ROOTDIR)/include -ffreestanding -fno-builtin -nostdlib -nostdinc -fno-stack-protector -Wall -Wextra -Werror -ansi -Os -c

LD = ld.lld
LDFLAGS = -s -L$(ROOTDIR) -l$(LIB_NAME)

AR = ar
ARFLAGS = rcs

#### TARGET SPECIFIC ####

include $(ROOTDIR)/config/$(ARCH).mk

MODULE_FILES = $(patsubst %,$(ROOTDIR)/%.sys,$(MODULES))

