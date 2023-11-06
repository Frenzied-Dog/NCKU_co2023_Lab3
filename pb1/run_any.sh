RISCV=riscv64-unknown-elf

[ ! -d "./output" ] && mkdir ./output

${RISCV}-gcc -march=rv32i -mabi=ilp32 -ffreestanding -nostdlib -static -Wl,-Ttext=0x0 \
		$1.S user/user.c user/user.h user/call_to_emu.S \
		-o ./output/$1.elf -lgcc

${RISCV}-objcopy -S ./output/$1.elf -O binary ./output/$1.bin
${RISCV}-objdump -xsd -M no-aliases ./output/$1.elf > ./output/$1.dump

tmp=`echo $(${RISCV}-readelf -h ./output/$1.elf) | grep -oP '0x[a-z0-9]{1,5}'`

entry=$(echo ${tmp} | cut -d ' ' -f 2)

# echo ${entry}
./emu.elf ${entry} ./output/$1.bin