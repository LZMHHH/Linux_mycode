
#include "s3c2440_soc.h"

#include "led.h"
#include "uart.h"
#include  "my_printf.h"
#include "init.h"

void delay(volatile int d)
{
	while (d--);
}


int app(void)
{
	int val1, val2;
	unsigned char c;
	
	/* 设置GPFCON让GPF4/5/6配置为输出引脚 */
	Led_Init();
	uart0_init();
	puts("Hello, world!\n\r");
	my_printf_test();

	sdram_init();
	
	/* 配置3个按键引脚为输入引脚:
	 * GPF0(S2),GPF2(S3),GPG3(S4)
	 */
	GPFCON  &= ~((3<<0) | (3<<4));
	GPGCON  &= ~((3<<6));  /* gpg3 */
	
	 
	
	while(1)
	{
		Led_Con(1,eLedOff);
		c = getchar();
		Led_Con(1,eLedOn);
		if (c >= '0' && c <= '7')
		{
			bank0_tacc_set(c - '0');
			led_test();
		}
		else
		{
			puts("Error, val should between 0~7\n\r");
			puts("Enter the Tacc val: \n\r");
		}

		putchar(c);
		
		
		
#if 0
	
		val1 = GPFDAT;
	    val2 = GPGDAT;

		Led_Con(1,eLedOn);
		Led_Con(2,eLedOff);
		Led_Con(3,eLedOff);
		
		delay(100000);
		
		Led_Con(1,eLedOff);
		Led_Con(2,eLedOn);
		Led_Con(3,eLedOff);
		
		delay(100000);
		
		Led_Con(1,eLedOff);
		Led_Con(2,eLedOff);
		Led_Con(3,eLedOn);
		
		delay(100000);
		
		
		if(val1 & (1<<0))
		{
			GPFDAT |= (1<<4);
		}
		else
		{
			GPFDAT &= ~(1<<4);
		}
		
		if(val1 & (1<<2))
		{
			GPFDAT |= (1<<5);
		}
		else
		{
			GPFDAT &= ~(1<<5);
		}
		
		if(val2 & (1<<3))
		{
			GPFDAT |= (1<<6);
		}
		else
		{
			GPFDAT &= ~(1<<6);
		}
#endif	
		
	}
	
	return 0;
}






