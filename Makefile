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

.PHONY: run, debug_kernel, clean, compile_kernel, wrapper, linker

run: compile_kernel
	$(QEMU) -kernel target/kernel/kernel.bin -monitor stdio

debug_kernel: compile_kernel
	$(QEMU) -s -S -kernel target/kernel/kernel.bin

clean:
	rm -r ./target/
	mkdir -p ./target/kernel/states

compile_kernel: wrapper $(OBJFILES)
	$(CC) -g -T kernel/linker.ld -o target/kernel/kernel.bin -ffreestanding -Og -nostdlib target/kernel/*.o -lgcc

wrapper:
	nasm -felf64 kernel/wrapper.asm -o target/kernel/wrapper.o

target/%.o: %.c Makefile
	$(CC) -g -c $< -o $@ -std=gnu99 -ffreestanding -Og -Wall -Wextra -mno-80387 -mgeneral-regs-only -mno-red-zone -MMD -MP

todolist:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file; done; true

-include $(DEPFILES)
