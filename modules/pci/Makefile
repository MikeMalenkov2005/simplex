ROOTDIR = ../..
include $(ROOTDIR)/config.mk

OBJECTS += $(patsubst %.c,%.o,$(wildcard arch/$(ARCH)/*.c))

MODULE_FILE = $(ROOTDIR)/pci.sys

LDFLAGS += -l$(LIB_NAME)

.PHONY: clean

$(MODULE_FILE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o arch/*/*.o $(MODULE_FILE)

