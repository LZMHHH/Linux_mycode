
#include "s3c2440_soc.h"

#include "led.h"
#include "uart.h"
#include  "my_printf.h"
#include "init.h"


char g_Char = 'A';
char g_Char3 = 'a';
const char g_Char2 = 'B';
int g_A = 0;
int g_B;


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
	key_eint_init();   /* 初始化按键, 设为中断源 */
	timer_init();
	puts("Hello, world!\n\r");
	my_printf_test();

	
	
	lcd_test();
	 
	
	while(1)
	{
		putchar(g_Char);
		g_Char++;

		putchar(g_Char3);
		g_Char3++;
		Led_Con(1,eLedOn);
		delay(1000000);
		
		putchar(g_Char);
		g_Char++;

		putchar(g_Char3);
		g_Char3++;
		Led_Con(1,eLedOff);
		delay(1000000);
		
		
		
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






