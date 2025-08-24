# TickTOS - A Real Time Operating System

## Overview
This project to make a Real Time Operating System. This kernel runs in **32-bit Protected Mode**, boots via **GRUB**, and includes basic timer and keyboard drivers, a custom heap allocator, and interrupt handling. Every feature of the kernel are the legacy features, assembly instructions, memory alignment, ELF structure and Multiboot spec.

---

## Features

1. **Boot**

- GRUB + Multiboot compliant

- Custom linker script for memory layout

- VGA text mode buffer

- Serial output for debugging


2. **Descriptor Tables**

- Global Descriptor Table (**GDT**)
- Interrupt Descriptor Table (**IDT**)

  
3. **Interrupts & PIC**

- CPU exceptions
- Hardware Interrupt (IRQ) handling
- PIC remapping

  

4. **Drivers**

- Keyboard driver to print pressed keys
- Timer driver to tick at 100 Hz

  

5. **Memory Management**

- Parse Multiboot memory map
- Print free blocks
- Initialize heap
- Implement `kalloc` and `kfree` with First-Fit approach
- Memory Alignment for heap and allocations


6. **Debugging**

- `GDB` with `QEMU`

- Inspect registers such as, `PIC` registers, `EFLAGS` and segment registers

  

---

## Project Structure

```

├── boot/ # Bootloader code (Multiboot header)
├── gdt/ # Global Descriptor Table setup
├── idt/ # Interrupt Descriptor Table & IRQs
│ ├── irq/ # IRQ handling
│ └── isr/ # Exception handling
├── drivers/ # Keyboard, Timer, Serial drivers
├── kernel/ # Core kernel logic & memory management
├── display/ # VGA text mode output
├── grub/ # GRUB config
├── include/ # Header files
├── iso/ # ISO build directory for GRUB boot
├── linker/ # Linker script
├── build/ # Compiled objects & ISO
└── Makefile # Build automation
```

  

---

## Requirements

- `qemu-system-i386` or `qemu-system-x86_64`
- `i686-elf-gcc` cross-compiler
- `make`
- `nasm`
- `gdb-multiarch` (for debugging)

---

## Build

```bash

make

```

This generates:


- `build/ticktos.elf` – Kernel ELF binary

- `build/ticktos.iso` – Bootable ISO (GRUB)

---

## Run

**Standard graphical mode:**

```bash

qemu-system-i386 -cdrom build/ticktos.iso

```

**Non-graphical with serial output:**

```bash

qemu-system-i386 -cdrom build/ticktos.iso -nographic -serial mon:stdio

```

**For older machine (to inspect PIC and disable APIC):**

```bash

qemu-system-x86_64 -cdrom build/ticktos.iso -serial stdio -machine isapc

```

**To log output to a file:**

```bash

qemu-system-x86_64 -cdrom build/ticktos.iso -serial stdio -machine isapc > qemu.log 2>&1

```
  
---

## Debug With `GDB`

Run `QEMU` in debug mode:

```bash

qemu-system-i386 -cdrom build/ticktos.iso -nographic -serial mon:stdio -s -S

```

Attach `GDB`:

```bash

gdb-multiarch build/ticktos.elf

(gdb) target remote localhost:1234

(gdb) break kernel_main

(gdb) continue

```

---

## What's Next

- Virtual memory and paging
- `printf` and basic `stdio`-like functions
- Multitasking between dummy tasks
- Porting my mini shell into the kernel

---
