RISCV=riscv64-unknown-elf

# make clean && make
if [ "$2" = "make" ]
then
	tmp=`echo $(make test) | grep -oP '0x[a-z0-9]{1,5}'`
else
	${RISCV}-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -static -T test/link.ld \
			test/$1.c test/user/user.c test/user/user.h test/user/start.S test/user/call_to_emu.S \
			-o test/$1.elf -lgcc

	${RISCV}-objcopy -S test/$1.elf -O binary test/$1.bin
	${RISCV}-objdump -xsd -M no-aliases test/$1.elf > test/$1.dump

	tmp=`echo $(${RISCV}-readelf -h test/$1.elf) | grep -oP '0x[a-z0-9]{1,5}'`
fi

entry=$(echo ${tmp} | cut -d ' ' -f 2)

# echo ${entry}
./main.elf ${entry} ./test/$1.bin