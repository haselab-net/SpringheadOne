make clean
make
sh-elf-objcopy -O srec a.out test.mot
sh-elf-objdump -D a.out > test.txt
