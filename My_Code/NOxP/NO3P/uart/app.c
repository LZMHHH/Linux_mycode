
#include "s3c2440_soc.h"
#include "uart.h"

int app(void)
{
	unsigned char c;
	
	uart0_init();
	puts("Hello, world!\n\r");
	
	while(1)
	{
		c = getchar();
		if (c == '\r')
		{
			putchar('\n');
		}

		if (c == '\n')
		{
			putchar('\r');
		}

		putchar(c);
	}
	return 0;
}



