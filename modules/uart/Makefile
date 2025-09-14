ROOTDIR = ../..
include $(ROOTDIR)/config.mk

OBJECTS += $(patsubst %.c,%.o,$(wildcard arch/$(ARCH)/*.c))

ASMSRC = $(wildcard arch/$(ARCH)/*.asm)
ASMOBJ = $(patsubst %.asm,%.o,$(ASMSRC))

MODULE_FILE = $(ROOTDIR)/uart.sys

LDFLAGS += -e UART_Main

.PHONY: clean

$(MODULE_FILE): $(OBJECTS) $(ASMOBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(ASMOBJ): %.o: %.asm
	$(ELFASM) -o $@ $<

clean:
	rm *.o $(MODULE_FILE)

