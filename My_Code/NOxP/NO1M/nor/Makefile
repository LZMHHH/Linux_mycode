all:
	arm-linux-gcc -c -o init.o  init.c
	arm-linux-gcc -c -o my_printf.o  my_printf.c
	arm-linux-gcc -c -o lib1funcs.o lib1funcs.S
	arm-linux-gcc -c -o uart.o  uart.c
	arm-linux-gcc -c -o led.o   led.c
	arm-linux-gcc -c -o app.o   app.c
	arm-linux-gcc -c -o start.o start.S
	arm-linux-ld  -Ttext 0 start.o app.o led.o uart.o lib1funcs.o my_printf.o init.o -o nor.elf
	arm-linux-objcopy -O binary -S nor.elf nor.bin
	arm-linux-objdump -D nor.elf > nor.dis
clean:
	rm *.bin *.o *.elf *.dis
	
