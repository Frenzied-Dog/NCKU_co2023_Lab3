make clean && make
tmp=`echo $(make test) | grep -oP '0x[a-z0-9]{1,5}'`
entry=$(echo ${tmp} | cut -d ' ' -f 2)

./main.elf ${entry} ./test/hello.bin