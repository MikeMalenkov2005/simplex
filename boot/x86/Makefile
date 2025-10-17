ROOTDIR = ../..
include $(ROOTDIR)/config.mk

GRUBIMG = grub.img
GRUBCFG = grub.cfg
GRUBMOD = biosdisk part_msdos fat multiboot2 configfile ls cat help

ISO_DIR = $(ROOTDIR)/iso

FLOPPY_FILE	= $(ROOTDIR)/$(KERNEL).img
ISO_FILE = $(ROOTDIR)/$(KERNEL).iso

.PHONY: clean

# all: $(ROOTDIR)/$(KERNEL).img $(ROOTDIR)/$(KERNEL).iso

$(ISO_FILE): $(KERNEL_FILE) $(MODULE_FILES) $(GRUBCFG)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_FILE) $(ISO_DIR)/boot/simplex.sys
	cp $(MODULE_FILES) $(ISO_DIR)/boot/
	cp $(GRUBCFG) $(ISO_DIR)/boot/grub
	grub-mkrescue $(ISO_DIR) -o $@

#$(FLOPPY_FILE): $(KERNEL_FILE) $(MODULE_FILES) $(GRUBIMG) $(GRUBCFG)
#	mkdir -p $(dir $@)
#	dd if=/dev/zero of=$@ bs=512 count=2880
#	dd if=/usr/lib/grub/i386-pc/boot.img of=$@ conv=notrunc
#	dd if=$(GRUBIMG) of=$@ conv=notrunc seek=1
#	mformat -i $@ -kR $(shell echo $$(($(shell ls --block-size=512 -s $(GRUBIMG) | sed 's/\s.*$$//') + 2)))
#	mcopy -i $@ $(GRUBCFG) ::/
#	mcopy -i $@ $(KERNEL_FILE) ::/$(KERNEL_NAME).sys
#	mcopy -i $@ $(MODULE_FILES) ::/

#$(GRUBIMG):
#	mkdir -p $(dir $@)
#	grub-mkimage -p / -C auto -O i386-pc -o $@ $(GRUBMOD)

clean:
	rm -rf $(ISO_DIR) $(ROOTDIR)/$(KERNEL).img

