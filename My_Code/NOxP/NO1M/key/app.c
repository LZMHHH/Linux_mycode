
#include "s3c2440_soc.h"

void delay(volatile int d)
{
	while (d--);
}


int app(void)
{
	int val1, val2;
	
	/* 设置GPFCON让GPF4/5/6配置为输出引脚 */
	GPFCON &= ~((3<<8) | (3<<10) | (3<<12));
	GPFCON |=  ((1<<8) | (1<<10) | (1<<12));


	/* 配置3个按键引脚为输入引脚:
	 * GPF0(S2),GPF2(S3),GPG3(S4)
	 */
	GPFCON  &= ~((3<<0) | (3<<4));
	GPGCON  &= ~((3<<6));  /* gpg3 */
	
	 
	val1 = GPFDAT;
	val2 = GPGDAT;
	
	while(1)
	{
		val1 = GPFDAT;
	    val2 = GPGDAT;
	
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
	
		
	}
	
	return 0;
}