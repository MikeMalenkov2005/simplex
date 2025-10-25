ROOTDIR = ../..
include $(ROOTDIR)/config.mk

MODULE_FILE = $(ROOTDIR)/x86ps2.sys

LDFLAGS += -l$(LIB_NAME)

.PHONY: clean

$(MODULE_FILE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o $(MODULE_FILE)

