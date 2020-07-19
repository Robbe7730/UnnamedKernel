SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES := $(patsubst %.c,target/%.o,$(SRCFILES))
DEPFILES    := $(patsubst %.c,target/%.d,$(SRCFILES))

.PHONY: run, debug_kernel, clean, compile_kernel, wrapper, linker

run: compile_kernel
	qemu-system-i386 -kernel target/kernel/kernel.bin -monitor stdio

debug_kernel: compile_kernel
	qemu-system-i386 -s -S -kernel target/kernel/kernel.bin

clean:
	rm -r ./target/
	mkdir -p ./target/kernel

compile_kernel: wrapper $(OBJFILES)
	i686-elf-gcc -g -T kernel/linker.ld -o target/kernel/kernel.bin -ffreestanding -Og -nostdlib target/kernel/*.o -lgcc

wrapper:
	nasm -felf32 kernel/wrapper.asm -o target/kernel/wrapper.o

target/%.o: %.c Makefile
	i686-elf-gcc -g -c $< -o $@ -std=gnu99 -ffreestanding -Og -Wall -Wextra -mno-80387 -mgeneral-regs-only -mno-red-zone -MMD -MP

-include $(DEPFILES)
