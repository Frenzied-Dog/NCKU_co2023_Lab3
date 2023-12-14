SRC = main.c emu.c interface.c
OBJ = $(patsubst %.c, %.o, $(SRC))
ELF = emu.elf
CFLAGS = -Wall -Werror -O2
# CFLAGS = -g -Wall -O0
RISCV := riscv64-unknown-elf
RCCFLAGS := -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -static -T ./test/link.ld
TEST_DIR := ./test


all: $(ELF)

$(ELF): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -lreadline

%.o: %.c %.h
	$(CC) $(CFLAGS) $< -c

test: hello.bin

hello.bin: hello.elf
	$(RISCV)-objcopy -S $(TEST_DIR)/$< -O binary $(TEST_DIR)/$@
	$(RISCV)-readelf -h $(TEST_DIR)/$<

hello.elf: $(TEST_DIR)/hello.c $(TEST_DIR)/user/user.c $(TEST_DIR)/user/user.h $(TEST_DIR)/user/start.S $(TEST_DIR)/user/call_to_emu.S
	$(RISCV)-gcc ${RCCFLAGS} $^ -o $(TEST_DIR)/$@ -lgcc



.PHONY = clean
clean:
	-@$(RM) -rvf $(OBJ) $(ELF)