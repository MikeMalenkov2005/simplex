ROOTDIR = .
include $(ROOTDIR)/config.mk

.PHONY: boot kernel modules lib clean

boot: kernel modules
	$(MAKE) -C boot/$(ARCH)

kernel: lib
	$(MAKE) -C kernel

modules: lib
	$(MAKE) -C modules

lib:
	$(MAKE) -C lib

clean:
	$(MAKE) -C lib clean
	$(MAKE) -C kernel clean
	$(MAKE) -C modules clean
	$(MAKE) -C boot/$(ARCH) clean

