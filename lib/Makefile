ROOTDIR = ..
include $(ROOTDIR)/config.mk

OBJECTS += $(patsubst %.c,%.o,$(wildcard arch/$(ARCH)/*.c))

ASMSRC = $(wildcard arch/$(ARCH)/*.asm)
ASMOBJ = $(patsubst %.asm,%.o,$(ASMSRC))

.PHONY: clean

$(LIB_FILE): $(OBJECTS) $(ASMOBJ)
	$(AR) $(ARFLAGS) $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(ASMOBJ): %.o: %.asm
	$(ELFASM) -o $@ $<

clean:
	rm *.o arch/$(ARCH)/*.o $(LIB_FILE)

