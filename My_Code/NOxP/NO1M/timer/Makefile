all: start.o init.o my_printf.o lib1funcs.o uart.o led.o app.o exception.o interrupt.o timer.o
	arm-linux-ld  -T sdram.lds $^ -o timer.elf
	arm-linux-objcopy -O binary -S timer.elf timer.bin
	arm-linux-objdump -D timer.elf > timer.dis
clean:
	rm *.bin *.o *.elf *.dis
	
%.o: %.c
	arm-linux-gcc -c -o $@ $<
	
%.o : %.S
	arm-linux-gcc -c -o $@ $<