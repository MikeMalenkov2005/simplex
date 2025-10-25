ROOTDIR = ../..
include $(ROOTDIR)/config.mk

MODULE_FILE = $(ROOTDIR)/x86vga.sys

LDFLAGS += -l$(LIB_NAME)

.PHONY: clean

$(MODULE_FILE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o $(MODULE_FILE)

