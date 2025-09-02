ROOTDIR = ..
include $(ROOTDIR)/config.mk

OBJECTS += $(patsubst %.c,%.o,$(wildcard arch/$(ARCH)/*.c))
LDFLAGS += -T arch/$(ARCH)/link.ld

ASMSRC = $(wildcard arch/$(ARCH)/*.asm)
ASMOBJ = $(patsubst %.asm,%.o,$(ASMSRC))

.PHONY: clean

$(KERNEL_FILE): $(OBJECTS) $(ASMOBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(ASMOBJ): %.o: %.asm
	$(ELFASM) -o $@ $<

clean:
	rm *.o arch/$(ARCH)/*.o $(KERNEL_FILE)

