all:
	arm-linux-gcc -c -o app.o   app.c
	arm-linux-gcc -c -o start.o start.S
	arm-linux-ld  -Ttext 0 start.o app.o -o clk_led.elf
	arm-linux-objcopy -O binary -S clk_led.elf clk_led.bin
	arm-linux-objdump -D clk_led.elf > clk_led.dis
clean:
	rm *.bin *.o *.elf *.dis
	
