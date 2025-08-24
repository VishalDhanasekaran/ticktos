# === Toolchain ===
CC        = i686-elf-gcc
OBJCOPY   = i686-elf-objcopy
NASM      = nasm
CFLAGS    = -ffreestanding -O0 -g -nostdlib
INCLUDES  = -Iinclude
LDFLAGS   = -T linker/linker.ld

# === Build dirs ===
BUILD_DIR = build
ISO_DIR   = iso

# === Sources ===
KERNEL_SRC      = kernel/kernel.c
BOOT_SRC        = boot/boot.asm
GDT_C_SRC       = gdt/gdt.c
GDT_ASM_SRC     = gdt/gdt_flush.asm
IDT_SRC         = idt/idt.c
ISR_ASM_SRC     = idt/ISR/isr.asm
EXCEPTIONS_SRC  = idt/ISR/exceptions.c
IRQ_ASM_SRC     = idt/IRQ/irq.asm
IRQ_SRC         = idt/IRQ/irq.c
PIC_SRC         = idt/PIC/pic.c
SERIAL_SRC      = drivers/serial.c
TIMER_SRC       = drivers/timer.c
KEYBOARD_SRC    = drivers/keyboard.c
VGA_SRC         = display/vga_display.c
PIT_SRC         = pit/pit.c
MANAGER_SRC     = kernel/memory-management/manager.c
ALLOCATOR_SRC   = kernel/memory-management/allocator.c

# === Objects ===
OBJS = \
	$(BUILD_DIR)/boot.o \
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/gdt.o \
	$(BUILD_DIR)/gdt_flush.o \
	$(BUILD_DIR)/idt.o \
	$(BUILD_DIR)/isr_asm.o \
	$(BUILD_DIR)/exceptions.o \
	$(BUILD_DIR)/irq_asm.o \
	$(BUILD_DIR)/irq.o \
	$(BUILD_DIR)/pic.o \
	$(BUILD_DIR)/serial.o \
	$(BUILD_DIR)/timer.o \
	$(BUILD_DIR)/keyboard.o \
	$(BUILD_DIR)/vga_display.o \
	$(BUILD_DIR)/pit.o \
	$(BUILD_DIR)/manager.o \
	$(BUILD_DIR)/allocator.o 

# === Default ===
all: $(BUILD_DIR)/ticktos.iso check

# === Create dirs ===
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# === Assemble ===
$(BUILD_DIR)/boot.o: $(BOOT_SRC) | $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/gdt_flush.o: $(GDT_ASM_SRC) | $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/isr_asm.o: $(ISR_ASM_SRC) | $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

$(BUILD_DIR)/irq_asm.o: $(IRQ_ASM_SRC) | $(BUILD_DIR)
	$(NASM) -f elf32 $< -o $@

# === Compile C ===
$(BUILD_DIR)/%.o: kernel/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/gdt.o: $(GDT_C_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/idt.o: $(IDT_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/exceptions.o: $(EXCEPTIONS_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/irq.o: $(IRQ_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/pic.o: $(PIC_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/serial.o: $(SERIAL_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/timer.o: $(TIMER_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/keyboard.o: $(KEYBOARD_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/vga_display.o: $(VGA_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/pit.o: $(PIT_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/manager.o: $(MANAGER_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/allocator.o: $(ALLOCATOR_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# === Link ELF ===
$(BUILD_DIR)/ticktos.elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(CFLAGS) $^ -lgcc

# === Flat binary ===
$(BUILD_DIR)/ticktos.bin: $(BUILD_DIR)/ticktos.elf
	$(OBJCOPY) -O binary $< $@

# === Build ISO ===
$(BUILD_DIR)/ticktos.iso: \
    $(BUILD_DIR)/ticktos.elf \
    $(BUILD_DIR)/ticktos.bin \
    grub/grub.cfg
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/ticktos.elf  $(ISO_DIR)/boot/ticktos.elf
	cp $(BUILD_DIR)/ticktos.bin  $(ISO_DIR)/boot/ticktos.bin
	cp grub/grub.cfg           $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $@ $(ISO_DIR) >/dev/null 2>&1

# === Multiboot check ===
check:
	@grub-file --is-x86-multiboot $(BUILD_DIR)/ticktos.elf && \
	  echo "[OK] Multiboot compliant" || \
	  (echo "[FAIL] Not Multiboot compliant" && exit 1)

# === Clean ===
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)

.PHONY: all check clean