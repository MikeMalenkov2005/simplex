ROOTDIR = ../..
include $(ROOTDIR)/config.mk

MODULE_FILE = $(ROOTDIR)/test.sys

LDFLAGS += -e Init

.PHONY: clean

$(MODULE_FILE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o $(MODULE_FILE)

