# "Source" files
PROJDIRS := kernel kernel/states
AUXFILES := Makefile README.md
SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")

# Generated files
OBJFILES := $(patsubst %.c,target/%.o,$(SRCFILES))
DEPFILES := $(patsubst %.c,target/%.d,$(SRCFILES))

# All files
ALLFILES := $(SRCFILES) $(HDRFILES) $(AUXFILES)

CC := x86_64-elf-gcc
QEMU := qemu-system-x86_64

.PHONY: run, debug_kernel, clean, compile_kernel, wrapper, grub_rescue

run: grub_rescue
	$(QEMU) target/grub-rescue.iso -monitor stdio

debug_kernel: grub_rescue
	$(QEMU) -s -S target/grub-rescue.iso

clean:
	rm -r ./target/
	mkdir -p ./target/kernel/states

compile_kernel: wrapper $(OBJFILES)
	$(CC) -g -T kernel/linker.ld -o target/kernel/kernel.bin -ffreestanding -Og -nostdlib target/kernel/*.o -lgcc -z max-page-size=0x1000

wrapper:
	nasm -felf64 kernel/wrapper.asm -o target/kernel/wrapper.o

target/%.o: %.c Makefile
	$(CC) -g -c $< -o $@ -std=gnu99 -ffreestanding -Og -Wall -Wextra -mno-80387 -mgeneral-regs-only -mno-red-zone -MMD -MP

todolist:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file; done; true

grub_rescue: compile_kernel
	cp target/kernel/kernel.bin grub_iso/boot/kernel.bin
	grub-mkrescue -o target/grub-rescue.iso grub_iso

-include $(DEPFILES)
