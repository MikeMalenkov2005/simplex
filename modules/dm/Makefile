ROOTDIR = ../..
include $(ROOTDIR)/config.mk

MODULE_FILE = $(ROOTDIR)/dm.sys

LDFLAGS += -l$(LIB_NAME)

.PHONY: clean

$(MODULE_FILE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o $(MODULE_FILE)

