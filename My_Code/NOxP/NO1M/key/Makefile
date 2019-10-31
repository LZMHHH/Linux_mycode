all:
	arm-linux-gcc -c -o app.o   app.c
	arm-linux-gcc -c -o start.o start.S
	arm-linux-ld  -Ttext 0 start.o app.o -o key_led.elf
	arm-linux-objcopy -O binary -S key_led.elf key_led.bin
	arm-linux-objdump -D key_led.elf > key_led.dis
clean:
	rm *.bin *.o *.elf *.dis
	
